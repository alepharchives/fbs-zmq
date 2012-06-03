/*
 * $Id: $
 *
 * Module:  fbs_zmq_server -- ZMQ driver for FIS PIP messaging
 * Created: 04-MAR-2011 20:18
 * Author:  tmr
 */

/*
 *  Copyright (C) 2011 Tomas Morstein, IDEA Systems <tmr&idea.cz>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zmq.h>
#include <signal.h>

#include "fbs_zmq.h"
#include "gtmxc_types.h"

#include <stdio.h>

typedef void SigFunc (int);

void * m_Context;
void * m_Receiver;

SigFunc * m_SigALRM, * m_SigBUS, * m_SigFPE, * m_SigSEGV;

void SrvConnect (int cnt, char * svc_type_name, long * id, long * res) {

  fbs_zmq_setupHandlers ();

  m_Context = zmq_init (1);
  m_Receiver = zmq_socket (m_Context, ZMQ_REP);
  zmq_connect (m_Receiver, FBS$ZMQ_BACK_URI);

  fbs_zmq_cancelHandlers ();
}

void SrvDisconnect (int cnt) {

  fbs_zmq_setupHandlers ();

  zmq_close (m_Receiver);
  zmq_term (m_Context);

  fbs_zmq_cancelHandlers ();
}

void SrvGetMsg (int cnt, xc_string_t * msg, long timeout, long * res) {

  fbs_zmq_setupHandlers ();

  zmq_msg_t message;
  zmq_msg_init (&message);
  /*
  while (zmq_recv (m_Receiver, &message, 0)) {
    if (zmq_errno () != EINTR) {
      *res = FBS$ZMQ_FAILURE;
      fprintf(stderr, "Error: %s\n", zmq_strerror (zmq_errno()));
      fbs_zmq_cancelHandlers ();
      return;
    }
  }
  */

  if (zmq_recv (m_Receiver, &message, 0)) {
    *res = FBS$ZMQ_FAILURE;
    fbs_zmq_cancelHandlers ();
    return;
  }

  msg->length = zmq_msg_size (&message);
  msg->str = malloc (msg->length + 1);
  memcpy (msg->str, zmq_msg_data (&message), msg->length);
  zmq_msg_close (&message);
  msg->str [msg->length] = 0;
  *res = FBS$ZMQ_SUCCESS;

  fbs_zmq_cancelHandlers ();
}

void SrvReply (int cnt, xc_string_t * msg, long * res) {

  fbs_zmq_setupHandlers ();

  zmq_msg_t message;
  zmq_msg_init_size (&message, msg->length);
  memcpy (zmq_msg_data (&message), msg->str, msg->length);
  if (zmq_send (m_Receiver, &message, 0)) {
    *res = FBS$ZMQ_FAILURE;
  }
  zmq_msg_close (&message);
  *res = FBS$ZMQ_SUCCESS;

  fbs_zmq_cancelHandlers ();
}

SigFunc * fbs_zmq_signal (int signo, SigFunc * fun) {

  struct sigaction act, oact;

  act.sa_handler = fun;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;

  return (sigaction (signo, &act, &oact) < 0)?
    SIG_ERR : oact.sa_handler;
}

void fbs_zmq_signalCatcher (int signo) { return; }

int fbs_zmq_setupHandlers () {

  int status = -1;

  do {
    if ((m_SigALRM = fbs_zmq_signal (SIGALRM, SIG_IGN)) == SIG_ERR)
      break;

    if ((m_SigBUS = fbs_zmq_signal (SIGBUS, fbs_zmq_signalCatcher)) == SIG_ERR)
      break;

    if ((m_SigFPE = fbs_zmq_signal (SIGFPE, fbs_zmq_signalCatcher)) == SIG_ERR)
      break;

    if ((m_SigSEGV = fbs_zmq_signal (SIGSEGV, fbs_zmq_signalCatcher)) == SIG_ERR)
      break;

    status = 0;
  } while (0);

  return (status);
}

int fbs_zmq_cancelHandlers () {

  int status = -1;

  do {
    if (fbs_zmq_signal (SIGALRM, m_SigALRM) == SIG_ERR)
      break;

    if (fbs_zmq_signal (SIGBUS, m_SigBUS) == SIG_ERR)
      break;

    if (fbs_zmq_signal (SIGFPE, m_SigFPE) == SIG_ERR)
      break;

    if (fbs_zmq_signal (SIGSEGV, m_SigSEGV) == SIG_ERR)
      break;

    status = 0;
  } while (0);

  return (status);
}

// vim: fdm=syntax:fdn=1:tw=74:ts=2:syn=c
