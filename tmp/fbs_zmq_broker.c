/*
 * $Id: $
 *
 * Module:  fbs_zmq_broker
 * Created: 21-FEB-2011 20:17
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

int main (int argc, char * argv []) {

  if (argc < 3) {
    fprintf (stderr,
        "Usage: %s <WorkerBrokerURI> <ClientBrokerURI>\n", argv [0]);
    fprintf (stderr,
        "\nWorkerBrokerURI can be something like this:\n"
        "  tcp://*:5556\n"
        "  ipc:///tmp/fbs_zmq_worker.sock\n"
        "\nClientBrokerURI can be something like this:\n"
        "  tcp://*:5555\n"
        "  ipc:///tmp/fbs_zmq_client.sock\n");
    return (10);
  }

  //  Prepare our context and sockets
  void *context = zmq_init (1);

  //  Socket to talk to clients
  void *clients = zmq_socket (context, ZMQ_XREP);
  zmq_bind (clients, argv [2]);

  //  Socket to talk to workers
  void *workers = zmq_socket (context, ZMQ_XREQ);
  zmq_bind (workers, argv [1]);

  //  Connect work threads to client threads via a queue
  zmq_device (ZMQ_QUEUE, clients, workers);

  //  We never get here but clean up anyhow
  zmq_close (clients);
  zmq_close (workers);
  zmq_term (context);

  return 0;
}

// vim: fdm=syntax:fdn=1:tw=74:ts=2:syn=c
