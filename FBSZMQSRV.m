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

FBSZMQSRV N ID,MSG,ERR S (ID,MSG,ERR)=""
  D &zmq.SrvConnect("SCA$FBS",.ID,.ERR)
  F I=1:1 Q:$G(^ZTMR("ZMQ$STOP"))  D
  . D &zmq.SrvGetMsg(.MSG,10,.ERR)
  . W "Recv Error: "_ERR,!
  . Q:ERR<0
  . W "MUMPS Received this: >>>"_MSG_"<<< "_I,!
  . D &zmq.SrvReply($$PROCESS(MSG),.ERR)
  . W "Send Error: "_ERR,!
  D &zmq.SrvDisconnect()
  Q

PROCESS(MSG) N BODY,HDR,TYPE,DLM,CALL,RES
  S DLM=$C(13,10),HDR=$P(MSG,DLM_DLM,1),BODY=$P(MSG,HDR_DLM_DLM,2)
  S TYPE=$P($P(HDR,"FBWS_HANDLER: ",2),DLM,1)
  W "Handler: >>>"_TYPE_"<<<",!
  W "Body: >>>"_BODY_"<<<",!
  ;Q $$FBWSPROC^PBSSRV(BODY,TYPE)
  S CALL=$P(^FBSTBL("RPC",TYPE),"|",1)
  X "S RES=$$"_CALL_"("""","""",BODY)"
  Q RES

; vim: fdm=syntax:fdn=3:tw=74:ts=2:syn=mumps
