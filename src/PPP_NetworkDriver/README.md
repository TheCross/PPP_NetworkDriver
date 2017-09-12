# PPP-Network-Driver
Test-Project to develop a network-interface over a serial connection.
The physical-layer is usart.
The data-link-layer is PPP over HDLC (PPPoHDLC).

Included in this project is also a implementation of IPv4, UDP and TCP. Other Protocols may follow at a later time.

This project has been developed using the Arduino-IDE (1.6.9) but it should also run when compiled with any other Compiler/IDE.
The Arduino-IDE had been used because of the integrated bootloader-support so I had not to use an additional Programmer.

To establish a PPP-Connection in linux the following command has been used for testing:
sudo pppd /dev/ttyACM0 19200 noauth lock crtscts noipdefault defaultroute 192.168.1.1:192.168.1.2

# Status
All incomming PPP-Packets can bee received and will be placed into a buffer for later processing.
I am trying to handle LPC-Packets at the moment with the target that a PPP-Connection can be "established".
Then I will try to handle IP-Packets with the target to receive simple UDP-Messages and transmit a reply.