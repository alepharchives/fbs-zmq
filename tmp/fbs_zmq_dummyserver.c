/*
 * $Id: $
 *
 * Module:  client -- description
 * Created: 21-FEB-2011 20:36
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

#include "zhelpers.h"

static void * workerFun (void * uri) {
  void *context = zmq_init (1);

  //  Socket to talk to server
  void * receiver = zmq_socket (context, ZMQ_REP);
  zmq_connect (receiver, (char *) uri);

  long reqNo = 0;
  char resp [256];
  while (1) {
    char *string = s_recv (receiver);
    //printf ("Received request: [%s]\n", string);
    if ((++reqNo % 1000) == 0)
      printf ("Thread #%lu, Req #%d\n", pthread_self (), reqNo);
    free (string);

    resp [0] = '\0';
    snprintf (resp, sizeof (resp),
        "World of the thread #%lu", pthread_self ());

    //  Send reply back to client
    s_send (receiver, resp);
  }

  zmq_close (receiver);
  zmq_term (context);
}

#define MAX_THREADS 5

int main (int argc, char * argv []) {

  if (argc < 2) {
    fprintf (stderr, "Usage: %s <WorkerBrokerURI>\n", argv [0]);
    fprintf (stderr, "\nWorkerBrokerURI can be something like this:\n"
                     "  tcp://localhost:5556\n"
                     "  ipc:///tmp/fbs_zmq_worker.sock\n");
    return (10);
  }

  int i;
  pthread_t worker [MAX_THREADS];
  for (i = 0; i < MAX_THREADS; i++)
    pthread_create (&worker [i], NULL, workerFun, argv [1]);

  while (1) {
    // Some monitoring and notification stuff
    sleep (10);
  }

  return 0;
}

// vim: fdm=syntax:fdn=1:tw=74:ts=2:syn=c
