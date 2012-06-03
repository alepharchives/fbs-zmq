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
LDFLAGS+=`pkg-config --libs --cflags libzmq`
CFLAGS+=-ggdb -O0
SED=sed
PWD!=pwd

SERVER=fbs_zmq_server
XCFIL=fbs_zmq

all: ${SERVER}

${SERVER}:
	@echo "Building $@..."
	$(CC) $(CFLAGS) -c -fPIC $@.c
	@echo "Linking $@..."
	$(CC) $@.o $(LDFLAGS) -o lib$@.so -shared
	@echo "Creating XC index..."
	@$(SED) -e 's,@@SHRLIB@@,$(PWD)/lib$@.so,g' $(XCFIL).xc.in > $(XCFIL).xc

clean:
	rm -f ${SERVER}.o lib${SERVER}.so

