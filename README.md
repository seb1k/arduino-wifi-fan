# arduino-wifi-fan
An arduino project to add wifi to an infrared fan (base on wemos d1)

Add wifi to my infrared Fan


I got a rowenta VU5670F2, it’s a silent fan with a basic IR remote. I wanted to add wifi to control it from a web interface.

Used :
-	WEMOS D1
-	IR LED
-	220ohm resistor
-	IR receptor to get the code.

1 – Get the IR code of each button from the IR remote with this arduino example :
https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/IRrecvDumpV3/IRrecvDumpV3.ino

It gives me the protocol « Symphony » with the code
Button  time     0xD88 
Button  silent night  0xDCC
Button  turbo boost   0xDA0
Button  +/-       0xD82
Button decrescendo   0xD84
Button  on/off      0xD81

2 – My final code :

4 – Get an IP with arduino and try it before opening the fan.



!! Think about reserve the IP address in your router, and it will never change.


5 – Open the fan, and find a way to insert the board + IR LED




5v and GND are easy to find.
Here is how I di dit in mine :

Put the IR LED close to the IR receptor.
I used heat shrink tube to isolate the LED near the main board.

!!! I am not responsible for any broken board or fan

