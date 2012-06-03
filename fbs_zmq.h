/*
 * $Id: $
 *
 * Module:  fbs_zmq -- description
 * Created: 04-MAR-2011 20:28
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

#ifndef _FBS_ZMQ_H_
#define _FBS_ZMQ_H_

#define FBS$ZMQ_SUCCESS      0
#define FBS$ZMQ_FAILURE     -100

/* Backend URI, a server-side socket of ZMQ broker */
#define FBS$ZMQ_BACK_URI   "ipc:///tmp/fbs_zmq_worker.sock"

#endif

// vim: fdm=syntax:fdn=1:tw=74:ts=2:syn=c
