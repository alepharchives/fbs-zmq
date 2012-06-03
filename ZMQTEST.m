 ; Copyright (C) 2009 Tomas Morstein, IDEA Systems <tmr&idea.cz>
 ;
 ; This program is free software: you can redistribute it and/or
 ; modify it under the terms of the GNU Affero General Public
 ; License as published by the Free Software Foundation, either
 ; version 3 of the License, or (at your option) any later
 ; version.
 ;
 ; This program is distributed in the hope that it will be useful,
 ; but WITHOUT ANY WARRANTY; without even the implied warranty of
 ; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ; GNU Affero General Public License for more details.
 ;
 ; You should have received a copy of the GNU Affero General
 ; Public License along with this program. If not, see
 ; <http://www.gnu.org/licenses/>.

ZMQTEST N ID,MSG,ERR S (ID,MSG,ERR)=""
  D &zmq.SrvConnect("SCA$FBS",.ID,.ERR)
  F I=1:1 Q:$G(^ZTMR("ZMQ$STOP"))  D
  . D &zmq.SrvGetMsg(.MSG,10,.ERR)
  . W "Recv Error: "_ERR,!
  . Q:ERR<0
  . W "MUMPS Received this: >>>"_MSG_"<<< "_I,!
  . D &zmq.SrvReply("Fooo Booo",.ERR)
  . W "Send Error: "_ERR,!
  D &zmq.SrvDisconnect()
  Q

API N vzcsid,vzrole,vzpkt,vzpktid,vzreply
  S vzermsg=$$SVCNCT^%MTAPI("SCA$ZIBS",.vzcsid,.vzrole,"ZMQ")
  F I=1:1 Q:$G(^ZTMR("ZMQ$STOP"))  D
  . S vzermsg=$$GETMSG^%MTAPI(.vzpkt,.vzpktid,.vzcsid,10)
  . W "Recv Error: "_vzermsg,!
  . D:vzermsg'=""  Q
  . W "MUMPS Received this: >>>"_vzpkt_"<<< "_I,!
  . S vzreply="Fooo Booo via %MTAPI"
  . S vzermsg=$$REPLY^%MTAPI(.vzreply,vzpktid,vzcsid)
  . W "Send Error: "_vzermsg,!
  S vzermsg=$$SVDSCNCT^%MTAPI(vzcsid)
  Q

; vim: fdm=syntax:fdn=3:tw=74:ts=2:syn=mumps
