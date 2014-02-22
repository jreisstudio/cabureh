# Cabureh Arduino
A way to access the Arduino ports as in an api.

## How to use the Cabureh
 * Copy the wifly_hq_cabureh_slave.ino to your arduino.
 * Put a led into pin 13 (pleased use a resistor to connect it to the ground!)
 * Run the code:
  ruby test_cabureh_conn.rb

The Led on pin 13 will blink once, and you'll be ready to use your imagination to call your Arduino ports as in an API!

Example:
 * http://192.168.1.101/digital/11/1 -> turns the pin 11 HIGH
 * http://192.168.1.101/digital/11/0 -> turns the pin 11 LOW

By hour that's all! 

## ChangeLog
###  v1.0 (2014/02/22)
 * Added the wiflyHQ Arduino code.
 * Added a class to test the Arduino connection.
 
## Backlog
 * To create a slave for arduino official  wifi shield 
 * To Create a way to send and get info from analogic pins.
 
