Virtual modem for ESP8266
=========================

Copyright (C) 2016 Paul Rickards <rickards@gmail.com> under GPLv3 license.

based on https://github.com/jsalin/esp8266_modem
Copyright (C) 2015 Jussi Salin <salinjus@gmail.com> under GPLv3 license.

Overview
--------

ESP8266 is a tiny MCU module with WIFI. It already contains a virtual modem firmware by factory but I wanted to make one myself to support a wider range of baud rates. For example Commodore 64 requires 2400 or lower. Now it is also possible to add additional features in the future because this is open source. For example, translation tables for different character sets or file transfer protocol conversions on the fly with help of a buffer in MCU memory.

This was originally modified for use on the Commodore 64. The easiest device to use in the Sparkfun ESP8266 WiFi Shield. Originally designed for use with an Arduino, it already contains the 3.3v <-> 5v level shifters needed on TX and RX but also the 3.3v voltage regulator. https://www.sparkfun.com/products/13287

Additional features added include storing settings in NVRAM, managing active settings and stored settings, speed dial management, auto answer capabilities, automatic busy messages when in use for incoming connections, hardware flow control support, support for Commodore PET MCTerm (by Madison Computer) bit 8 ASCII translation, time in call on disconnect, web server on port 80 supporting mDNS.

Wiring
------

Connect the SparkFun ESP8266 WiFi Shield as follows to your C64.

* ESP8266 HW_RX pin to C64 TX User port pin M
* ESP8266 HW_TX pin to C64 RX User port pins B & C
* ESP8266 5V pin to C64 +5v User port pin 2
* ESP8266 GND pin to C64 GND User port pin A & N

Additionally, one wire jumper on the ESP8266 shield:

* ESP8266 RST pin to ESP8266 GPIO pin 0

Slide the UART switch to "HW".

AT command examples
-------------------

* Change baud rate: AT$SB=N (N=300,1200,2400,4800,9600,19200,38400,57600,115200)
* Set WiFi SSID: AT$SSID=YourWiFiSSID
* Set WiFi Password: AT$PASS=YourWiFiPassword
* Connect to WiFi: ATC1
* Save current settigns to NVRAM: AT&W
* Connect by TCP: ATDTsome.bbs.com:23
* Disable telnet command handling: ATNET0
* Get my IP: ATIP
* Make a HTTP GET request: ATGEThttp://host:80/path
* Answer a RING: ATA
* Disconnect: +++ (following a delay of a second)
* Speed Dial: ATDSN (N=0-9)
* Set Speed Dial: AT&ZN=HOST:PORT (N=0-9)
* Complete commands can be found with AT? or ATHELP

Note that the key and port are optional parameters. Port defaults to 23. All parameters are case sensitive, the command itself not. You must always connect to an access point before dialing, otherwise you get an error. When you connect to WIFI you get either OK or ERROR after a while, depending on if it succeeded. If you get ERROR the connection might still occur by itself some time later, in case you had a slow AP or slow DHCP server in the network. When dialing, you get either CONNECT when successfully connected or ERROR if the connection couldn't be made. Reasons can be that the remote service is down or the host name is mistyped.

Default Baud rate is defined in the code as 300. 2400 is safe for C64 and 19200 for any PC and Amiga. 115200 for PC's with "new" 16550 UART.  You must always have that default rate on the terminal when powering on. After giving a command for a higher rate nothing is replied, just switch to the new baud rate in your terminal as well. Then you can give next command in the new baud rate. Note that the first command after switching baud rate might fail because the serial port hardware is not fully synchronized yet, so it might be good idea to simply give "AT" command and wait for "ERROR" or "OK" before giving an actual command.

You can always default the baud rate back to 300 baud by pressing and holding the button (GPIO 0 to ground) for 5 seconds until the LED (GPIO 5) flashes fast.

Example communication
---------------------
    
    OK
    at$ssid=MyWiFiNetwork
    
    OK
    at$pass=MyPassword
    
    OK
    atc1
    
    CONNECTING TO SSID MyWiFiNetwork......
    CONNECTED TO MyWiFiNetwork
    IP ADDRESS: 192.168.2.3
    
    OK
    atdtbat.org:23
    DIALING bat.org:23
    
    CONNECT 2400
    
    
               ^_                                   _^
              /  \__________   /\___/\   __________/  \
             /              \  \     /  /              \
            /      \   \     \_/ ` ' \_/     /   /      \
           /   _/\  \   \        ---        /   /  /\_   \      Logo by:
          /  _/   \  \   \                 /   /  /   \_  \        Saruman
         / _/      \          _       _          /      \_ \
        /_/         \/\/\/\/\/ \_____/ \/\/\/\/\/         \_\
       //                                                   \\
    .       Welcome to BatMUD, Online since 14th April 1990!
    .     EMail: batry@bat.org about problems concerning BatMUD
    
         This machine is owned by B.A.T. ry, a non-profit organization, 
                        and is hosted at Nebula, Finland.


