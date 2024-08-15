#include "Powland_data_class.h"
#include "Credentials.h"
#include <ESP8266WebServer.h>
#define RX D5
#define TX D6


ESP8266WebServer server(80);
boolean access_point = false;

Credentials cred=Credentials();
SoftwareSerial myserial(RX,TX);
Powland_data_class powmr=Powland_data_class(RX,TX,&myserial);

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

/*void handlePowMr() {
  String message = powmr.read_data();
  server.send(200, "application/json", message);
}*/

void handlePowlandQPIGS() {
  String message = powmr.read_data_QPIGS();
  server.send(200, "text/html", message);
}
void handlePowlandQMOD() {
  String message = powmr.read_data_QMOD();
  server.send(200, "text/html", message);
}
void handlePowlandQPIWS() {
  String message = powmr.read_data_QPIWS();
  server.send(200, "text/html", message);
}

void handlePowlandQTEST() {
  String message = powmr.read_data_QTEST();
  server.send(200, "text/html", message);
}

/*void handlePowMrEnergy() {
  String message = powmr.convertToJSON(powmr.readEnergy());
  server.send(200, "application/json", message);
}

void handlePowMrEnergyClean() {
  String message = powmr.convertToJSON(powmr.readEnergyClean());
  server.send(200, "application/json", message);
}*/

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

  //temp=Temperature_data(D8,9);//4 now 15
  //volt=Voltage_data(A0,50); //5
  //ac=AC_data(D6,D5); // 12 14
  
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  //server.on("/powmr", handlePowMr);
  
  server.on("/QPIGS", handlePowlandQPIGS);
  server.on("/QMOD", handlePowlandQMOD);
  server.on("/QPIWS", handlePowlandQPIWS);
  server.on("/QTEST", handlePowlandQTEST);
  //server.on("/powmr_energy", handlePowMrEnergy);
  //server.on("/powmr_energy_clean", handlePowMrEnergyClean);
  /*server.on("/voltage", handleVoltage);
  server.on("/ac", handleAC);*/
  server.on("/change", handlechangecredentials);
  //server.on("/scanonewire", handleFindDevices);
  server.on("/restart", handleRestart);
  server.begin();
  Serial.println("HTTP server started");
  // Start up the library
  
  /*Serial.println(temp.read_data());
  Serial.println(volt.read_data());
  Serial.println(ac.read_data());*/
  //powmr.initEnergy();
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
