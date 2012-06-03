#
#  Copyright (C) 2011 Tomas Morstein, IDEA Systems <tmr&idea.cz>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as
#  published by the Free Software Foundation, either version 3 of the
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=gcc
CPP=g++
LDFLAGS+=`pkg-config --libs --cflags libzmq`
CFLAGS+=-ggdb -O0

BROKER=fbs_zmq_broker
DUMMY_CLIENT=fbs_zmq_dummyclient
DUMMY_SERVER=fbs_zmq_dummyserver

all: ${BROKER} ${DUMMY_CLIENT} ${DUMMY_SERVER}

${BROKER}:
	${CC} ${LDFLAGS} -o ${BROKER} ${BROKER}.c

${DUMMY_CLIENT}:
	${CC} ${LDFLAGS} -o ${DUMMY_CLIENT} ${DUMMY_CLIENT}.c

${DUMMY_SERVER}:
	${CC} ${LDFLAGS} -o ${DUMMY_SERVER} ${DUMMY_SERVER}.c

clean:
	rm -f ${BROKER} ${DUMMY_CLIENT} ${DUMMY_SERVER}

