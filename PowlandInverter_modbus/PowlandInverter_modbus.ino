#include "Powland_data_modbus.h"
#include "Powland_data_modbus_settings.h"
#include "Credentials.h"
#include <ESP8266WebServer.h>
#define RX D5
#define TX D6
#include <SoftwareSerial.h>
ESP8266WebServer server(80);
boolean access_point = false;

Credentials cred=Credentials();
SoftwareSerial myserial(RX,TX);
Powland_data_class powland=Powland_data_class(&myserial);
Powland_data_class_settings powland_settings=Powland_data_class_settings(&myserial);
volatile uint32_t access_point_start = 0;
long last_poll;
void handleRestart() {
  server.send(200, "text/plain", "restart");
  delay(2000);
  ESP.restart();
}

void handlechangecredentials() {
  String message = "";
  String s = server.arg("SSID"), p = server.arg("PSK"),polling_duration=server.arg("polling_duration");
  char ssid[20],password[20];
  s.toCharArray(ssid, 20);
  p.toCharArray(password, 20);
  cred.setCredentials(ssid,password,polling_duration.toInt());
  message = s + "  " + p;
  server.send(200, "text/plain", message);
  cred.saveCredentials();
  delay(2000);
  ESP.restart();
}

void handleRoot() {
  Serial.println("scan start");
  int n = 0;
  n = WiFi.scanNetworks();
  String mess = "<!DOCTYPE HTML>\r\n<html>";
  mess += "<head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta content=\"utf-8\" http-equiv=\"encoding\"></head>\r\n";
  mess += "<body>";
  mess += n;
  mess += " networks found </br>";
  for (int q = 0; q < n; ++q) {
    mess += (q + 1);
    mess += ": ";
    mess += WiFi.SSID(q);
    mess += " <-> ";
    mess += WiFi.RSSI(q);
    mess += " ";
    mess += WiFi.encryptionType(q);
    mess += "</br>";
    delay(10);
  }
  mess += "<form method='get' action='change'><label>SSID: </label><input name='SSID' length=32><label>Paswoord: </label><input name='PSK' length=64><label>polling_duration:</label><input name=\"polling_duration\" type=\"number\" step=\"0.1\" value=\""+(String)cred.getpolling_duration()+"\"><input type='submit'></form>";
  mess += "</body></html>";
  server.send(200, "text/html", mess);
}

void handlemodbus(){
  String message = powland.read_data();
  server.send(200, "application/json", message);
}

void handlemodbussettings(){
  String message = powland_settings.read_data();
  server.send(200, "application/json", message);
}


void setup(){
  delay(1000);
  Serial.begin(115200);

  cred.loadCredentials();
  
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(cred.getssid(), cred.getpassword());
  
  boolean connect = false;
  
  int count = 0;
  while (WiFi.status() != WL_CONNECTED && count++ < 50) {
    delay(2000);
    Serial.print(".");}
    
  Serial.println();
  if (WiFi.status() != WL_CONNECTED)
  { Serial.println("Not connected creating acces point : ");
    Serial.println(APSSID);
    WiFi.softAP(APSSID);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("AP IP address: ");
    Serial.println(myIP);
    access_point = true;
    access_point_start = millis();
  } 
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  //server.on("/powmr", handlePowMr);
  
  server.on("/modbus", handlemodbus);
  
  server.on("/modbussettings", handlemodbussettings);

  server.on("/change", handlechangecredentials);
  //server.on("/scanonewire", handleFindDevices);
  server.on("/restart", handleRestart);
  server.begin();
  Serial.println("HTTP server started");
  // Start up the library
  
  last_poll = millis();
}

void loop() {
 if (!access_point && WiFi.status() != WL_CONNECTED)
    ESP.restart();
  if (access_point){
    volatile uint32_t access_point_interval = millis() - access_point_start;
    if(access_point_interval > 300000 || access_point_interval <0)
        ESP.restart();
  }
    server.handleClient();
  /*if( cred.getpolling_duration()>0 && ( (millis()/1000-powmr.pm_energy.last_record) >= cred.getpolling_duration() || (millis()/1000< powmr.pm_energy.last_record) ) ){
      powmr.readEnergy();
    }  */
}
