

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 
#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(4,5);
char buf[80];
WiFly wifly;

const char network_ssid[] = "xxx";
const char network_password[] = "xxx";

void   serial_start();
void join_network();
void server_start(char *server_name);
void default_digital_header(char *pin, char *pin_level);
void send_404();
void send_pin_status(int pin);
void on_pin(int pin);
void off_pin(int pin);

void setup(){
  serial_start();
  join_network();
  server_start("Cabureh_Slave");
  //default_setup();
}

void loop(){
/*Humm.. is somenone Requesting me?*/
  if (wifly.available() > 0) {
    if (wifly.gets(buf, sizeof(buf))) {
      /*Nice! what can i serve to you?*/
     router();
    }
  }
}

void serial_start(){
  Serial.begin(115200);
  wifiSerial.begin(9600);
  if (!wifly.begin(&wifiSerial, &Serial)) {
    Serial.println(F("[Serial] - Failed."));
    wifly.terminal();
  }
}

void join_network(){
  if (!wifly.isAssociated()) {
    wifly.setSSID(network_ssid);
    wifly.setPassphrase(network_password);
    wifly.enableDHCP();
    wifly.save();    
    if (wifly.join()) {
      Serial.println(F("[Network] Joined!"));
    } else {
      Serial.println(F("[Network] - Failed to join."));
      wifly.terminal();
    }    
  }else{
    Serial.println(F("[Network]Already joined.")); 
  }


  wifly.setBroadcastInterval(0);  // Turn off UPD broadcast
  Serial.print(F("[Network]MAC/IP:")); 
  Serial.print(wifly.getMAC(buf, sizeof(buf)));
  Serial.print(F(" / "));
  Serial.println(wifly.getIP(buf, sizeof(buf)));
}

void server_start(char *server_name){
  wifly.setDeviceID(server_name);
  if (wifly.isConnected()) {
    Serial.println(F("[Server] Old connection active. Closing"));
    wifly.close();
  }
  wifly.setProtocol(WIFLY_PROTOCOL_TCP);
  if (wifly.getPort() != 80) {
    wifly.setPort(80);
    wifly.save();
    Serial.println(F("[Server] Set port to 80."));
    wifly.reboot();
    delay(3000);
  }
    Serial.print(F("[Server] The "));  
    Serial.print(wifly.getDeviceID(buf, sizeof(buf)));
    Serial.println(F(" is ready"));  
}

void default_digital_header(char *pin, char *pin_level){
  wifly.flushRx();
  wifly.println(F("HTTP/1.1 200 OK"));
  wifly.println(F("Content-Type: application/json"));
  wifly.println(F("Transfer-Encoding: chunked"));
  wifly.println();    
  wifly.sendChunkln(F("{"));
  wifly.sendChunk(F("\"pin\": "));
  wifly.sendChunk(pin);        
  wifly.sendChunkln(F(" , "));       
  wifly.sendChunk(F("\"pin_level\": "));
  wifly.sendChunkln(pin_level);        
  wifly.sendChunkln(F("}"));
  wifly.sendChunkln();    
}

void send_pin_status(int pin){
  
  wifly.flushRx();
  wifly.println(F("HTTP/1.1 200 OK"));
  wifly.println(F("Content-Type: application/json"));
  wifly.println(F("Transfer-Encoding: chunked"));
  wifly.println();    
  wifly.sendChunkln(F("{"));
  wifly.sendChunk(F("\"pin\": "));
  String pin_str;
  char pin_char[16];
  pin_str= String(pin);
  pin_str.toCharArray(pin_char, 16);
  wifly.sendChunk(pin_char);        
  wifly.sendChunkln(F(" , "));       
  wifly.sendChunk(F("\"pin_level\": "));
  if(digitalRead(pin) ==LOW){
    wifly.sendChunkln("\"LOW\""); 
  }else{
    wifly.sendChunkln("\"HIGH\""); 
  }
  wifly.sendChunkln(F("}"));
  wifly.sendChunkln();


}


void send_404(){
  wifly.flushRx();
  wifly.println(F("HTTP/1.1 404 Not Found"));
  wifly.println(F("Content-Type: application/json"));
  wifly.println(F("Transfer-Encoding: chunked"));
  wifly.println();
  wifly.sendChunkln(F("{"));
  wifly.sendChunkln(F("\"status\": 400"));
  wifly.sendChunkln(F("}"));
  wifly.sendChunkln();
}

void on_pin(int pin){
    if(digitalRead(pin) ==LOW){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
    }  
}

void off_pin(int pin){
    if(digitalRead(pin) ==HIGH){
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }  
}
void router(){
        Serial.print(buf);
  
  if(strncmp_P(buf, PSTR("PUT /digital/02/1"), 17) == 0) {
                 Serial.println("on pin 02");
    on_pin(2); 
    default_digital_header("\"02\"","\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/02/0"), 17) == 0){
                 Serial.println("off pin 02");
    off_pin(2); 
    default_digital_header("\"02\"","\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/03/1"), 17) == 0) {
                Serial.println("on pin 03");
    on_pin(3); 
    default_digital_header("\"03\"","\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/03/0"), 17) == 0){
                Serial.println("off pin 03");
      off_pin(3); 
    default_digital_header("\"03\"","\"LOW\"");

////  On my WiflyHQ lib, this pins are reserved.
////
////  } else if(strncmp_P(buf, PSTR("PUT /digital/04/1"), 17) == 0) {
////                Serial.println("on pin 04");
////    on_pin(4); 
////    default_digital_header("\"04\"", "\"HIGH\"");
////  } else if (strncmp_P(buf, PSTR("PUT /digital/04/0"), 17) == 0){
////                Serial.println("off pin 04");
////    off_pin(4); 
////    default_digital_header("\"04\"", "\"LOW\"");
////
////  } else if(strncmp_P(buf, PSTR("PUT /digital/05/1"), 17) == 0) {
////                Serial.println("on pin 05");
////    on_pin(5); 
////    default_digital_header("\"05\"", "\"HIGH\"");
////  } else if (strncmp_P(buf, PSTR("PUT /digital/05/0"), 17) == 0){
////                Serial.println("off pin 05");
////    off_pin(5); 
////    default_digital_header("\"05\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/06/1"), 17) == 0) {
                Serial.println("on pin 06");
    on_pin(6); 
    default_digital_header("\"06\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/06/0"), 17) == 0){
                Serial.println("off pin 06");  
    off_pin(6); 
    default_digital_header("\"06\"","\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/07/1"), 17) == 0) {
                Serial.println("on pin 07");
    on_pin(7); 
    default_digital_header("\"07\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/07/0"), 17) == 0){
                Serial.println("off pin 07");
    off_pin(7); 
    default_digital_header("\"07\"","\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/08/1"), 17) == 0) {
                Serial.println("on pin 08");  
    on_pin(8); 
    default_digital_header("\"08\"","\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/08/0"), 17) == 0){
                Serial.println("off pin 08");  
    off_pin(8); 
    default_digital_header("\"08\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/09/1"), 17) == 0) {
                Serial.println("on pin 09");
    on_pin(9); 
    default_digital_header("\"09\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/09/0"), 17) == 0){
                Serial.println("off pin 09");  
    off_pin(9); 
    default_digital_header("\"09\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/10/1"), 17) == 0) {
                Serial.println("on pin 10");
    on_pin(10); 
    default_digital_header("\"10\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/10/0"), 17) == 0){
                Serial.println("off pin 10");  
    off_pin(10); 
    default_digital_header("\"10\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/11/1"), 17) == 0) {
                 Serial.println("on pin 11");
    on_pin(11); 
    default_digital_header("\"11\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/11/0"), 17) == 0){
                Serial.println("off pin 11");
    off_pin(11); 
    default_digital_header("\"11\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/12/1"), 17) == 0) {
                Serial.println("on pin 12");
      on_pin(12); 
    default_digital_header("\"12\"", "\"HIGH\"");
  } else if (strncmp_P(buf, PSTR("PUT /digital/12/0"), 17) == 0){
                Serial.println("off pin 12");
    off_pin(12); 
    default_digital_header("\"12\"", "\"LOW\"");

  } else if(strncmp_P(buf, PSTR("PUT /digital/13/1"), 17) == 0) {
                Serial.println("on pin 13");
    on_pin(13); 
    default_digital_header("\"13\"", "\"HIGH\"");
    
  } else if (strncmp_P(buf, PSTR("PUT /digital/13/0"), 17) == 0){
                Serial.println("off pin 13\"");
    off_pin(13); 
    default_digital_header("\"13\"", "\"LOW\"");
    
  } else if (strncmp_P(buf, PSTR("GET /digital/02"), 15) == 0){
    Serial.println("status pin 02");
    send_pin_status(2);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/03"), 15) == 0){
    Serial.println("status pin 03");
    send_pin_status(3);
    
//  } else if (strncmp_P(buf, PSTR("GET /digital/04"), 15) == 0){
//    Serial.println("status pin 04");
//    send_pin_status(4);
//    
//  } else if (strncmp_P(buf, PSTR("GET /digital/05"), 15) == 0){
//    Serial.println("status pin 05");
//    send_pin_status(5);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/06"), 15) == 0){
    Serial.println("status pin 06");
    send_pin_status(6);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/07"), 15) == 0){
    Serial.println("status pin 07");
    send_pin_status(7);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/08"), 15) == 0){
    Serial.println("status pin 08");
    send_pin_status(8);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/09"), 15) == 0){
    Serial.println("status pin 09");
    send_pin_status(9);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/10"), 15) == 0){
    Serial.println("status pin 10");
    send_pin_status(10);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/11"), 15) == 0){
    Serial.println("status pin 11");
    send_pin_status(11);
    
  } else if (strncmp_P(buf, PSTR("GET /digital/12"), 15) == 0){
    Serial.println("status pin 12");
    send_pin_status(12);  
    
  } else if (strncmp_P(buf, PSTR("GET /digital/13"), 15) == 0){
    Serial.println("status pin 13");
    send_pin_status(13);  
    
  } else{
    send_404();
  }

}
