#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>                             // do karty sd
#include <ESPAsyncWebSrv.h>
#include <AsyncTCP.h>
#include <SPI.h>                            //karta sd
#include <SD.h>
#include <DHT.h>
#include <SPI.h>
#include <DS3231.h>                          //obsługa RTC
#include <Wire.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <DHTesp.h>
#include <NTPClient.h>                        //do ntp
#include <WiFiUdp.h>                           //do ntp
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "certs.h"

#define STASSID "Test"
#define STAPSK "12345678"


//////test Api//////////////////
//#include <ESP8266WiFi.h>


IPAddress staticIp (192, 168, 137, 210);
IPAddress gateway (192, 168, 137, 1);
IPAddress subnet (255, 255, 255, 0);

AsyncWebServer server(80);

unsigned long Glob = 0;
int dhtSensors = 2;
byte pinSd = 5;
String temp1 = "--";
String temp2 = "--";
String tempOut = "--";
String hudmi1 = "--";
String hudmi2 = "--";
String hudmiOut = "--";
String apiStation = "--";
String apiTemp = "--";
String apiHudmi = "--";
String apiPressure = "--";
String ipAdress;
String ipSubnet;
String ipGateway;
String ipDns0;
String ipDns1;
String webSetting1 = "15";                             //temperatura div
String webSetting2 = "15";
String hysteresis = "0.5";
String heating1 = "true";
String heating2 = "true";
String autoManual = "false";                            //zezwolenie na zmiane true-auto wartośc true tryb auto
int LED_BUILTIN = 2;


void serverRestart() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {                //na otrzymane od klienta zapytanie pod adresem "/" typu GET,
    SPIFFS.begin();
    request->send(SPIFFS, "/web/index.html", "text/html");
    SPIFFS.end();
  });

  // server.serveStatic("/", SPIFFS, "/");// szuka Css i js

  server.on("/style", HTTP_GET, [](AsyncWebServerRequest * request) {                //na otrzymane od klienta zapytanie pod adresem "/" typu GET,
    SPIFFS.begin();
    request->send(SPIFFS, "/web/style.css", "text/css");
    SPIFFS.end();
  });

  // server.serveStatic("/", SPIFFS, "/");

  server.on("/scripts", HTTP_GET, [](AsyncWebServerRequest * request) { //na otrzymane od klienta zapytanie pod adresem "/" typu GET,
    SPIFFS.begin();
    request->send(SPIFFS, "/web/scripts.js", "text/text");
    SPIFFS.end();
  });

  // server.serveStatic("/", SPIFFS, "/");

  server.on("/scriptss", HTTP_GET, [](AsyncWebServerRequest * request) { //na otrzymane od klienta zapytanie pod adresem "/" typu GET,
    SPIFFS.begin();
    request->send(SPIFFS, "/web/scriptss.js", "text/text");
    SPIFFS.end();
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {      //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);                                      //zapal diodę
    request->send(200);                                                 //odeślij odpowiedź z kodem 200 OK
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {     //na otrzymane od klienta zapytanie pod adresem "/off" typu GET,
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);                                        //zgaś diodę
    request->send(200);                                                 //odeślij odpowiedź z kodem 200 OK
  });

  server.on("/adc", HTTP_GET, [](AsyncWebServerRequest * request) {    //na otrzymane od klienta zapytanie pod adresem "/off" typu GET,
    String wartosc = String(analogRead(A0));                           //wykonaj pomiar ADC i zapisz do Stringa
    request->send(200, "text/plain", wartosc);                        //odeślij odpowiedź z kodem 200 OK i odczytem z wartością
  });

  server.on("/dht1", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", temp1);                              //pobiera ze zmiennej glob
  });

  server.on("/hudmi1", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", hudmi1);                              //pobiera ze zmiennej glob
  });

  server.on("/dht2", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  temp2);                              //pobiera ze zmiennej glob
  });

  server.on("/hudmi2", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  hudmi2);                                   //pobiera ze zmiennej glob
  });

  server.on("/tempOut", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  tempOut);
  });

  server.on("/hudmiOut", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  hudmiOut);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest * request) {
    char tempChar [6];
    String tempStr;
    tempStr = showDay();
    tempStr.toCharArray(tempChar, tempStr.length() + 1);
    tempStr = "";
    tempStr += tempChar[0]  ;
    tempStr += tempChar[1];
    tempStr += "-";
    tempStr += tempChar[2];
    tempStr += tempChar[3]  ;
    tempStr += "-";
    tempStr += "20";
    tempStr += tempChar[4]  ;
    tempStr += tempChar[5];
    request->send(200, "text/plain",  tempStr);
  });

  server.on("/czas", HTTP_GET, [](AsyncWebServerRequest * request) {
    char tempChar [6];
    String tempStr;
    tempStr = showTime();
    tempStr.toCharArray(tempChar, tempStr.length() + 1);
    tempStr = "";
    tempStr += tempChar[0]  ;
    tempStr += tempChar[1];
    tempStr += ":";
    tempStr += tempChar[2];
    tempStr += tempChar[3]  ;
    tempStr += ":";
    tempStr += tempChar[4]  ;
    tempStr += tempChar[5];
    request->send(200, "text/plain",  tempStr);
  });

  server.on("/czasNtp", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  showTime());// nie ma sensu z serwera NTP brać
  });

  server.on("/apiStation", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  apiStation);
  });

  server.on("/apiTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  apiTemp);
  });

  server.on("/apiHudmi", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  apiHudmi);
  });

  server.on("/apiPressure", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  apiPressure);
  });

  server.on("/ethernetInfo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain",  (ipAdress + "  maska: " + ipSubnet + "  brama domyślna: " + ipGateway + " dns 1: " + ipDns0 + " dns 2: " + ipDns1));
  });

  server.on("/button1Up", HTTP_GET, [](AsyncWebServerRequest * request) {      //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    float temp;
    temp = webSetting1.toFloat();
    if (temp <= 29.50) {
      temp = temp + hysteresis.toFloat();
      webSetting1 = String(temp);
      Serial.println("Web button on click UP " + webSetting1);
    }
    request->send(200);
  });

  server.on("/button1Down", HTTP_GET, [](AsyncWebServerRequest * request) {     //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    float temp;
    temp = webSetting1.toFloat();
    if ( temp >= 5.5) {
      temp = temp - hysteresis.toFloat();
      webSetting1 = String(temp);
      Serial.println("Web button on click DOWN " + webSetting1);
    }
    request->send(200);
  });

  server.on("/button2Up", HTTP_GET, [](AsyncWebServerRequest * request) {     //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    float temp;
    temp = webSetting2.toFloat();
    if (temp <= 29.5) {
      temp = temp + hysteresis.toFloat();
      webSetting2 = String(temp);
      Serial.println("Web button 2 on click UP  " + webSetting2);
    }
    request->send(200);
  });

  server.on("/button2Down", HTTP_GET, [](AsyncWebServerRequest * request) {     //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    float temp;
    temp = webSetting2.toFloat();
    if (temp >= 5.5) {
      temp = temp - hysteresis.toFloat();
      webSetting2 = String(temp);
      Serial.println("Web button 2 on click DOWN" + webSetting2);
    }
    request->send(200);
  });

  server.on("/webTemp1", HTTP_GET, [](AsyncWebServerRequest * request) {    //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    request->send(200, "text/plain",  webSetting1);                                 //odeślij odpowiedź z kodem 200 OK
  });


  server.on("/webTemp2", HTTP_GET, [](AsyncWebServerRequest * request) {    //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    request->send(200, "text/plain",  webSetting2);                                               //odeślij odpowiedź z kodem 200 OK
  });

  //  server.on("/heating1", HTTP_GET, [](AsyncWebServerRequest * request) { //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
  //    //zapal diodę
  //    request->send(200, "text/plain",  "ok");                                               //odeślij odpowiedź z kodem 200 OK
  //  });


  server.on("/heatOut1", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (autoManual == "true") {                                                //jeśli jest w auto
      if (temp1.toFloat() <= webSetting1.toFloat()) {                         // warunek na grzanie
        heating1 = "true";
        digitalWrite(26, LOW);
        request->send(200, "text/plain",  "Auto 1 grzej");
      }
      else {
        heating1 = "false";
        digitalWrite(26, HIGH);
        request->send(210, "text/plain",  "1Off");
      }
    }
    else {
      if (heating1 == "true") {
        request->send(200);
      }
      else
      {
        request->send(210);
      }
    }
  });

  server.on("/heatOut2", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (autoManual == "true") {                                              //jeśli jest w auto
      if (temp2.toFloat() <= webSetting2.toFloat()) {                        // warunek na grzanie
        heating2 = "true";
        digitalWrite(27, LOW);
        request->send(200, "text/plain",  "Auto 2 grzej");
      }
      else {
        heating2 = "false";
        digitalWrite(27, HIGH);
        request->send(210, "text/plain",  "2Off");
      }
    }
    else {
      if (heating2 == "true") {
        request->send(200);
      }
      else
      {
        request->send(210);
      }
    }
  });

  server.on("/autoManual", HTTP_GET, [](AsyncWebServerRequest * request) {   //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    Serial.println(autoManual);
    if (autoManual == "true") {
      autoManual = "false";
      Serial.println(autoManual);
      request->send(200);
    }
    else {
      autoManual = "true";
      Serial.println(autoManual);
      request->send(210);
    }
    request->send(202);                                                          //odeślij odpowiedź z kodem 200 OK
  });


  server.on("/refresh", HTTP_GET, [](AsyncWebServerRequest * request) {                //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    if (autoManual == "false") {
      request->send(200);
    }
    if (autoManual == "true") {
      request->send(210);
    }
  });

  server.on("/onOff1", HTTP_GET, [](AsyncWebServerRequest * request) {       //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    if (autoManual == "false") {
      if (heating1 == "true") {
        heating1 = "false";
        digitalWrite(26, HIGH);
        request->send(200);
      }
      else {
        heating1 = "true";
        digitalWrite(26, LOW);
        request->send(210);
      }
    }
    request->send(202);
  });

  server.on("/onOff2", HTTP_GET, [](AsyncWebServerRequest * request) {          //na otrzymane od klienta zapytanie pod adresem "/on" typu GET,
    if (autoManual == "false") {
      if (heating2 == "true") {
        heating2 = "false";
        digitalWrite(27, HIGH);
        request->send(200);
      }
      else {
        heating2 = "true";
        digitalWrite(27, LOW);
        request->send(210);
      }
    }
    request->send(202);
  });

  server.begin();
}

void setup() {
  Wire.begin();
  SD.begin(pinSd);
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {                                                            //poczekajmy aż ESP połączy się z naszą seicią
  }
  Serial.printf("\nAdres IP:");
  Serial.println(WiFi.localIP());                                                                        //wypisz adres IP naszego ESP przez port szeregowy

  webSetting1 = jsonRead(spiffsRead("/temp/temp.json"), "temp1_nastawa");                               //odczyt danych z pamięci
  webSetting2 = jsonRead(spiffsRead("/temp/temp.json"), "temp2_nastawa");
  hysteresis = jsonRead(spiffsRead("/temp/temp.json"), "histereza");
  heating1 = jsonRead(spiffsRead("/temp/temp.json"), "obieg1");
  heating2 = jsonRead(spiffsRead("/temp/temp.json"), "obieg2");
  autoManual = jsonRead(spiffsRead("/temp/temp.json"), "zezwolenie");
  temp1 = String( sensorTemperature(32, 22));                                                                   //pierwsze zaczytanie czujników// potem co 5 minut
  hudmi1 = String(sensorHumidity(32, 22));
  temp2 = String( sensorTemperature(33, 22));
  hudmi2 = String(sensorHumidity(33, 22));
  tempOut = String(sensorTemperature(25, 22));
  hudmiOut = String(sensorHumidity(25, 22));
  ipAdress = WiFi.localIP().toString();
  ipSubnet = WiFi.subnetMask().toString();
  ipGateway = WiFi.gatewayIP().toString();
  ipDns0 = WiFi.dnsIP(0).toString();
  ipDns1 = WiFi.dnsIP(1).toString();
  apiStation = jsonRead(apiContent("torun"), "stacja");
  apiTemp = jsonRead(apiContent("torun"), "temperatura");
  apiHudmi = jsonRead(apiContent("torun"), "wilgotnosc_wzgledna");
  apiPressure = jsonRead(apiContent("torun"), "cisnienie");
  setTime(timeFromNtp());
  //  delay(2000); // dht zaczytanie

  if (heating1 == "true") {
    digitalWrite(26, LOW);                                                                            // hrzanie obieg 1
  }
  else {
    digitalWrite(26, HIGH);
  }
  if (heating2 == "true") {
    digitalWrite(27, LOW);                                                                             // grzanie obieg 2
  }
  else {
    digitalWrite(27, HIGH);
  }


  /////////////////////////
  ;//aktualizacja czasu z ntp

  /////////////////////////////////////////////////////////////////
  serverRestart();                                                                                               // uruchomienie serwera asynchronicznego
  //////////////////////////////////////////////////////////////////


}
void loop() {
  if (delayForRead(50)) {                                                                                                      //warunek zaczecia czytania temperatury, odczekanie czasu z millis() 300s
    server.reset();
    server.end();
    temp1 = String( sensorTemperature(32, 22));
    hudmi1 = String(sensorHumidity(32, 22));
    temp2 = String( sensorTemperature(33, 22));
    hudmi2 = String(sensorHumidity(33, 22));
    tempOut = String(sensorTemperature(25, 22));
    hudmiOut = String(sensorHumidity(25, 22));
    Serial.println(timeFromNtp());
    Serial.println(showTime());
    Serial.println(showDay());
    Serial.println(tempOut);
    Serial.println(hudmiOut);
    Serial.println(temp1);
    Serial.println(hudmi1);
    Serial.println(temp2);
    Serial.println(hudmi2);
    Serial.println("koniec");

    fileWriteSd("0", 5, showDay(), showTime(), sensorTemperature(25, 22), sensorHumidity(25, 22), true);
    fileWriteSd("1", 5, showDay(), showTime(), sensorTemperature(32, 22), sensorHumidity(32, 22), true);
    fileWriteSd("2", 5, showDay(), showTime(), sensorTemperature(33, 22), sensorHumidity(33, 22), true);

    apiStation = jsonRead(apiContent("torun"), "stacja");
    apiTemp = jsonRead(apiContent("torun"), "temperatura");
    apiHudmi = jsonRead(apiContent("torun"), "wilgotnosc_wzgledna");
    apiPressure = jsonRead(apiContent("torun"), "cisnienie");

    spiffsWrite("/temp/temp.json", webSetting1, webSetting2, hysteresis, heating1, heating2, autoManual);                        // zapis do pamięci urządzenia

    webSetting1 = jsonRead(spiffsRead("/temp/temp.json"), "temp1_nastawa");                                                      //odczyt danych z pamięci
    webSetting2 = jsonRead(spiffsRead("/temp/temp.json"), "temp2_nastawa");
    hysteresis = jsonRead(spiffsRead("/temp/temp.json"), "histereza");
    heating1 = jsonRead(spiffsRead("/temp/temp.json"), "obieg1");
    heating2 = jsonRead(spiffsRead("/temp/temp.json"), "obieg2");
    autoManual = jsonRead(spiffsRead("/temp/temp.json"), "zezwolenie");

    Serial.println(webSetting1);
    Serial.println(webSetting2);
    Serial.println(spiffsRead("/temp/temp.json"));

    if (heating1 == "true") {
      digitalWrite(26, LOW);                                                                                                      // hrzanie obieg 1
    }
    else {
      digitalWrite(26, HIGH);
    }

    if (heating2 == "true") {
      digitalWrite(27, LOW);                                                                                                        // grzanie obieg 2
    }
    else {
      digitalWrite(27, HIGH);
    }

    serverRestart();


  }
 


}

//String fileReadSd (String nameFile, byte pinSd) {
//  File dataFile;
//  String fileReadSd;
//  if (!SD.begin(pinSd)) {
//    Serial.println("initialization failed!");
//    // while (1);
//  }
//  //Serial.println("initialization done.");
//  dataFile = SD.open(nameFile);
//  if (dataFile) {
//    //Serial.println(nameFile);
//
//    // read from the file until there's nothing else in it:
//    while (dataFile.available()) {
//
//      fileReadSd = fileReadSd + (dataFile.readString().c_str());
//      //Serial.print(fileReadSd);
//    }
//
//    // close the file:
//    dataFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }
//  //Serial.print(fileReadSd);
//  return fileReadSd;
//}

bool fileWriteSd (String id, byte pinSd, String nameFile, String timeMeasure, float dataTemp, float dataHudmi, bool flag) { //opisana
  if (flag == true) {
    if (!SD.begin(pinSd)) {
      Serial.println("Card failed");
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("No SD card");
      return false;
    }
    Serial.println("card initialized.");
    String dataString = "";
    dataString += "{\"id_czujnika\":\"";
    dataString += id + "\",\"" + "czas_pomiaru" + "\":\"";
    dataString += timeMeasure + "\",\"temperatura\":\"";
    dataString += dataTemp;
    dataString += "\",\"wilgotnosc\":\"";
    dataString += dataHudmi;
    dataString += "\"}";
    String temp = "";
    temp = "/data/";
    temp += id;
    temp = temp + nameFile;
    char temp2[13];
    temp.toCharArray(temp2, temp.length() + 1);
    Serial.println (temp2);
    File dataFile = SD.open(temp2, FILE_APPEND);                                                                                            //nie obsłujuje nazwy dłuższej niż 8 znaków
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
    }

    return true;
  }
  else {
    return false;
  }
}

/////////////////////////////////////////////////////////////////
//String sensorTemperatureEsp(int pin, int type) {
//  DHTesp dht;
//  dht.setup(pin, DHTesp::DHT22);
//  //delay(600);
//  float temperature = dht.getTemperature();
//  return String (temperature);
//}
//String sensorHumidityEsp(int pin, int type) {
//  DHTesp dht;
//  dht.setup(pin, DHTesp::DHT22);
//  //delay(600);
//  float humidity = dht.getHumidity();
//  return String (humidity);
//}
/////////////////////////////////////////////////////////////////

float sensorTemperature (int pin, int type) {    //opisana             //pin- złącze w adtuino, type- typ sensora DHT
  DHT dht (pin, type);
  dht.begin();
  float temp;
  temp = dht.readTemperature();                        // false w stopniach C true w Fareahitach
  delay(600);
  return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
float sensorHumidity(int pin, int type) { //opisana
  DHT dht (pin, type);
  dht.begin();
  float humi;
  humi = (dht.readHumidity());
  delay(600);
  return humi;
}
///////////////////////////////////////////////////////////////////////////////////////////////
String showDay() {    //opisana                            //DDMMRR
  //Wire.begin();
  DS3231 rtc;
  String showDay = "";
  int year = 2000;
  bool month = false;
  if (rtc.getDate() < 10) {
    showDay += "0";
    showDay += rtc.getDate();
  }
  else {
    showDay += rtc.getDate();
  }
  if (rtc.getMonth(month) < 10) {
    showDay += "0";
    showDay += rtc.getMonth(month);
  }
  else {
    showDay += rtc.getMonth(month);
  }
  showDay += rtc.getYear();
  //  Wire.end();
  return showDay;
}
///////////////////////////////////////////////////////////////////////////////////////////
String showTime() {    //opisana                                 //172853 HHMMSS
  // Wire.begin();
  DS3231 rtc;
  String showTime = "";
  bool t12hour;
  bool pmFlag;
  if (rtc.getHour(t12hour, pmFlag) < 10) {
    showTime += "0";
    showTime += rtc.getHour(t12hour, pmFlag);
  }
  else {
    showTime += rtc.getHour(t12hour, pmFlag);
  }
  if (rtc.getMinute() < 10) {
    showTime += "0";
    showTime += rtc.getMinute();
  }
  else {
    showTime += rtc.getMinute();
  }
  if (rtc.getSecond() < 10) {
    showTime += "0";
    showTime += rtc.getSecond();
  }
  else {
    showTime += rtc.getSecond();
  }
  // Wire.end();
  return showTime;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void setTime(String dateTime) {     //opisana                                                            //// format DD-MM-RR HH:MM:SS //format DDMMRRHHMMSS  // format 123456789111213
  DS3231 rtc;
  bool boolDay = false;
  bool boolMonth = false;
  bool boolYear = false;
  bool boolHour = false;
  bool boolMinutes = false;
  bool boolSeconds = false;
  char ch[12];
  String str;
  int day;
  int month;
  int year;
  int hour;
  int minutes;
  int seconds;
  str = dateTime;
  str.toCharArray(ch, 13);
  Serial.println (ch);
  if ((ch[0] >= '0' && ch[0] <= '3') && (ch[1] >= '0' && ch[1] <= '9')) {
    Serial.println (ch[0]);
    Serial.println (ch[1]);
    day = 0;
    day = ch[0] - '0';
    day = 10 * day + ch[1] - '0';
    Serial.println (day);
    boolDay = true;
  }
  else {
    boolDay = false;
  }
  if ((ch[2] >= '0' && ch[2] <= '1') && (ch[3] >= '0' && ch[3] <= '9')) {
    month = 0;
    month = ch[2] - '0';
    month = 10 * month + ch[3] - '0';
    Serial.println (month);
    boolMonth = true;
  }
  else {
    boolMonth = false;
  }
  if ((ch[4] >= '0' && ch[4] <= '9') && (ch[5] >= '0' && ch[5] <= '9')) {
    year = 0;
    year = ch[4] - '0';
    year = 10 * year + ch[5] - '0';
    Serial.println (year);
    boolYear = true;
  }
  else {
    boolYear = false;
  }
  if ((ch[6] >= '0' && ch[6] <= '2') && (ch[7] >= '0' && ch[7] <= '9')) {
    hour = 0;
    hour = ch[6] - '0';
    hour = 10 * hour + ch[7] - '0';
    Serial.println (hour);
    boolHour = true;
  }
  else {
    boolHour = false;
  }
  if ((ch[8] >= '0' && ch[8] <= '5') && (ch[9] >= '0' && ch[9] <= '9')) {
    minutes = 0;
    minutes = ch[8] - '0';
    minutes = 10 * minutes + ch[9] - '0';
    Serial.println (minutes);
    boolMinutes = true;
  }
  else {
    boolMinutes = false;
  }
  if ((ch[10] >= '0' && ch[10] <= '5') && (ch[11] >= '0' && ch[11] <= '9')) {
    seconds = 0;
    seconds = ch[10] - '0';
    seconds = 10 * seconds + ch[11] - '0';
    Serial.println (seconds);
    boolSeconds = true;
  }
  else {
    boolSeconds = false;
  }
  if (boolDay && boolMonth && boolYear && boolHour && boolMinutes && boolSeconds == true) {
    rtc.setClockMode(false);                                                                   // format 24
    rtc.setDate (day);
    Serial.println(day);
    rtc.setMonth (month);
    Serial.println(month);
    rtc.setYear (year);
    Serial.println(year);
    rtc.setHour (hour);
    Serial.println(hour);
    rtc.setMinute (minutes);
    Serial.println(minutes);
    rtc.setSecond (seconds);
    Serial.println(seconds);
    rtc.setDoW (3);
    Serial.println("Time and date set");                                                               // rtc.setClockMode(true); // format 12
  }
  else {
    Serial.println("Bad format data");
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool delayForRead(int interval) { //opisana
  unsigned long temp;
  temp = interval * 1000;                       //interwał czasowy
  if (millis() - Glob >= temp) {
    Glob = millis();                             //zmienna globalna
    return true;
  }
  else {
    return false;
  }

}
//////////////////////////////////////////////////////////////////////////////////////////////////
String timeFromNtp() { // opisana
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  timeClient.begin();
  timeClient.setTimeOffset(2 * 3600);                // utc+2
  timeClient.update();
  String timeFromNtp = "";
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  if (ptm->tm_mday < 10) {
    timeFromNtp = "0" + String(ptm->tm_mday);
  }
  else {
    timeFromNtp = String(ptm->tm_mday);
  }
  Serial.println(timeFromNtp);
  if ((ptm->tm_mon + 1) < 10) {
    timeFromNtp = timeFromNtp + "0" + String(ptm->tm_mon + 1);
  }
  else {
    timeFromNtp = timeFromNtp + String(ptm->tm_mon + 1);
  }
  Serial.println(timeFromNtp);
  timeFromNtp = timeFromNtp + String(ptm->tm_year - 100);
  Serial.println(timeFromNtp);
  if (timeClient.getHours() < 10) {
    timeFromNtp = timeFromNtp + "0" + String(timeClient.getHours());
  }
  else {
    timeFromNtp = timeFromNtp + String(timeClient.getHours());
  }
  Serial.println(timeFromNtp);
  if (timeClient.getMinutes() < 10) {
    timeFromNtp = timeFromNtp + "0" + String(timeClient.getMinutes());
  }
  else {
    timeFromNtp = timeFromNtp + String(timeClient.getMinutes());
  }
  Serial.println(timeFromNtp);
  if (timeClient.getSeconds() < 10) {
    timeFromNtp = timeFromNtp + "0" + String(timeClient.getSeconds());
  }
  else {
    timeFromNtp = timeFromNtp + String(timeClient.getSeconds());
  }
  Serial.println(timeFromNtp);
  return timeFromNtp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
String apiContent(String town) { //opisana
  String apiContent;
  WiFiClientSecure client;
  client.setCACert(cert_DigiCert_Global_Root_CA);                      //certyfikat CA
  client.setPrivateKey(fingerprint___imgw);
  if (!client.connect(imgw_host, imgw_port)) {
    Serial.println("Connection failed");
  }
  else {
    String url = "/api/data/synop/station/" + town;
    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.println("GET " + String(url)  + " HTTP/1.0");
    Serial.println("GET " + String(url)  + " HTTP/1.0");
    client.println("Host: " + String(imgw_host));
    Serial.println("Host: " + String(imgw_host));
    client.println("User-Agent: curl/7.52.1");
    Serial.println("User-Agent: curl/7.52.1");
    client.println("Connection: close");
    Serial.println("Connection: close");
    client.println();
    Serial.println("");
    while (client.connected()) {
      String headers = client.readStringUntil('\n');
      Serial.println(headers);
      if (headers == "\r") {
        Serial.println("headers received");
        Serial.println(headers);
        break;
      }
    }
    while (client.available()) {
      apiContent = client.readStringUntil('\n');                              // pobieranie danych zwrotnych
    }
    client.stop();
  }
  return apiContent;
}
//  apiContent = client.readStringUntil('\n');
//  if (apiContent.startsWith("{\"state\":\"success\"")) {
//    Serial.println("Imgw successful!");
//  } else {
//    Serial.println("Imgw has failed");
//  }
//
//  Serial.println(apiContent);
//  return apiContent;
//  //    server.begin();
//  //  server.reset();//ponowne uruchomienie serwera
//client.stop();
//  delay(10);
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String jsonRead(String inputData, String findData) {    //opisana                              //asystent do obliczania pamieci potrzebnej do alokacji https://arduinojson.org/v6/assistant/#/step1
  const size_t capacity = JSON_OBJECT_SIZE(9) + JSON_ARRAY_SIZE(1) + 60 + 120 + 50;            //JSON_OBJECT_SIZE 9 ilosc parametrów, JSON_ARRAY_SIZE(1) ilość danych w parametrze
  DynamicJsonDocument doc(capacity);
  Serial.println("wielkośc buforu: ");
  Serial.print(capacity);
  DeserializationError error = deserializeJson(doc, inputData);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  String temp;
  temp = doc[findData].as<String>();
  return (temp);
}

/////////////////////////////////////////////////////////////////////////////////////////
String spiffsRead(String path) {            //opisana
  String temp;
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  File file = SPIFFS.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
  }
  Serial.println("File Content:");
  while (file.available()) {
    temp = file.readString().c_str();
  }
  file.close();
  SPIFFS.end();
  return temp;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void spiffsWrite(String path, String temp1, String temp2, String hist, String ob1, String ob2, String permit) {
  if (!SPIFFS.begin(true)) {
    Serial.println("Error SPIFFS mounting");
  }
  File file = SPIFFS.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for reading");
  }
  file.print("{\n");
  file.print("\"temp1_nastawa\":\"" + temp1 + "\",\n");
  file.print("\"temp2_nastawa\":\"" + temp2 + "\",\n");
  file.print("\"histereza\":\"" + hist + "\",\n");
  file.print("\"obieg1\":\"" + ob1 + "\",\n");
  file.print("\"obieg2\":\"" + ob2 + "\",\n");
  file.print("\"zezwolenie\":\"" + permit + "\"\n");
  file.print("}");
  file.close();
  SPIFFS.end();
}
