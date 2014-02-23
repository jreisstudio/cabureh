# Cabureh Arduino
A way to access the Arduino ports as in an api.

## How to use the Cabureh
 * Edit the wifly_hq_cabureh_slave.ino  to use your network infos (network_ssid e network_password)
 * Copy the wifly_hq_cabureh_slave.ino to your arduino.
 * Put a led into pin 13 (pleased use a resistor to connect it to the ground!)
 * Run the code:
  ruby test_cabureh_conn.rb

The Led on pin 13 will blink once, and you'll be ready to use your imagination to call your Arduino ports as in an API!

Example:

A PUT request to : 

  * http://192.168.1.101/digital/11/1 -> update the pin 11 status to HIGH
  * http://192.168.1.101/digital/11/0 -> update the pin 11 status to LOW

A GET request to : 

  * http://192.168.1.101/digital/11 -> returns the pin 11 status. 

By hour that's all! 

## ChangeLog

###  v0.2 (2014/02/23)
 * To use the method get to get the pin's info
 * To use the method PUT to change the pins status

###  v0.1 (2014/02/22)
 * Added the wiflyHQ Arduino code.
 * Added a class to test the Arduino connection.
 
## Backlog
 * To Create a way to send and get info from analogic pins.
 * To create a slave for arduino official  wifi shield 
 
