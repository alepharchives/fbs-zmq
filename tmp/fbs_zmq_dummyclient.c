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
  void *requester = zmq_socket (context, ZMQ_REQ);
  zmq_connect (requester, (char *) uri);

  int request_nbr; long reqNo = 0;
  for (request_nbr = 0; request_nbr < 10000; request_nbr++) {
     s_send (requester, "FBWS$RPC/MSQL-RPC\r\nFBWS_HANDLER: MSQL-RPC\r\n\r\nselect * from scau\r\n");
     char *string = s_recv (requester);
     //printf ("Received reply %d [%s]\n", request_nbr, string);
     if ((++reqNo % 1000) == 0)
       printf ("Client: Thread #%lu, Req #%d\n", pthread_self (), reqNo);
     free (string);
  }
  zmq_close (requester);
  zmq_term (context);
}

#define MAX_THREADS 10

int main (int argc, char * argv []) {

  if (argc < 2) {
    fprintf (stderr, "Usage: %s <ClientBrokerURI>\n", argv [0]);
    fprintf (stderr, "\nClientBrokerURI can be something like this:\n"
                     "  tcp://localhost:5555\n"
                     "  ipc:///tmp/fbs_zmq_client.sock\n");
    return (10);
  }

  int i;
  pthread_t worker [MAX_THREADS];
  for (i = 0; i < MAX_THREADS; i++)
    pthread_create (&worker [i], NULL, workerFun, argv [1]);

  for (i = 0; i < MAX_THREADS; i++)
    pthread_join (worker [i], NULL);

  return (0);
}

// vim: fdm=syntax:fdn=1:tw=74:ts=2:syn=c
