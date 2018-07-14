/*
   dacom command processing for ESP8266 Virtual Modem
   Original Source Copyright (C) 2016 Jussi Salin <salinjus@gmail.com>
   Additions (C) 2018 Daniel Jameson, Stardot Contributors

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**

   Commands all end with C/R = Carriage Return

   Command     Response        Action
   V21T        ++              300bps originate
   V21C        ++              300bps answer
   V23T        ++              viewdata originate
   V23C        ++              viewdata answer
   V23P        ++              pseudo duplex mode
   V23H        ++              half duplex mode
   BBC         ++              pseudo duplex mode

                               A phone number can be appended to any of the above
                               delimited by a space, or simply just entered on its own.

   EP          ++              Even Parity
   OP          ++              Odd Parity
   NP          ++              No Parity

   4x CTRL'A'  DISCONNECTED    Disconnects Modem if connected
   C           CONNECTED       Connects in answer mode
   T           CONNECTED       Connects in originate mode

   anything    WHAT?           Command not recognised

   Call Progress Messages:

   DIALLING                    Modem is waiting before dialing number
   01...etc                    Modem is dialing number
   WAITING                     Modem is looking for an answer tone
   CONNECTED                   Modem is connected to line
   RINGING                     Incoming call detected

   Abandoned Call Messages:
   NO DIAL TONE                Modem has not detected dial tone
   NO ANSWER                   Modem has not detected answer tone
   NO DCD                      Modem has lost data carrier detect
   DISCONNECTED                Modem DTE interface is disconnected from the line

   Dialing strings may be up to 18 digits long and can contain pauses as required.
   ';' will cause a 2 second pause, more than one delimiter may be used. If ';' is used
   before the telephone number the modem will pause and not look for the dial tone.
   If secondary dial tone is required use a ',' as a delimiter


*/

void dacomCommand() {
  cmd.trim();
  if (cmd == "") return;
  Serial.println();
  String upCmd = cmd;
  upCmd.toUpperCase();

  /**** Parity commands -acknowledge but don't do anything ****/
  if (cmd == "EP" || cmd == "OP" || cmd == "NP") {
    Serial.println("++");
  }

  /**** VxxT ****/
  else if ((cmd.length()==4 && (cmd.charAt(0)=='V' && cmd.charAt(3)!='C')) || cmd=="BBC") {
    Serial.println("++");
    dacomAutoAnswer==false;
  }

  /**** VxxC ****/
  else if ((cmd.length()==4 && (cmd.charAt(0)=='V' && cmd.charAt(3)=='C'))|| cmd=="C") { //C should just pick up
    Serial.println("++");
    dacomAutoAnswer==true;
  }



  /**** Return to Hayes compatible mode ****/
  else if (cmd == "HAYES" ) {
    Serial.println("HAYES COMMAND MODE, 1200 8N1");
    dacomMode = false;
  }

  
}

/**** Answer the phone ****/
void dacomAnswer() {
  if (tcpServer.hasClient())
  {
    tcpClient = tcpServer.available();
    tcpClient.setNoDelay(true); // try to disable naggle
    tcpServer.stop();
    digitalWrite(ESP_RING, HIGH); // we've picked up so ringing stops
    Serial.println("CONNECTED");
    digitalWrite(ESP_DCD, LOW); // we've got a carrier signal
    cmdMode = false;
    Serial.flush();
  }
}

