#include <U8g2lib.h>
#include <SPI.h>
U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


#include <Adafruit_BME280.h>
#include <Wire.h>
Adafruit_BME280 bme;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();


#include <ezTime.h>
#include <WiFi.h>
Timezone tz;


void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox4hb_tf);
  
  drawTextCenter(0, 0, "loading...");
  u8g2.sendBuffer();
  bme.begin(0x76);
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
  u8g2.clearBuffer();

  drawTextCenter(0, 0, "Connecting to WiFi...");
  WiFi.begin("ssid", "password");
  u8g2.sendBuffer();
  while (WiFi.status() != WL_CONNECTED) {}
  u8g2.clearBuffer();


  drawTextCenter(0, 0, "syncing time...");
  u8g2.sendBuffer();
  waitForSync();
  tz.setLocation("Europe/Berlin");
  tz.setDefault();
  u8g2.clearBuffer();
}


void loop() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);

  u8g2.setFont(u8g2_font_luBS18_tn);
  char buffer[16];
  sprintf(buffer, "%.1f°C", temp_event.temperature);
  drawTextCenter(32, 48, buffer);
  sprintf(buffer, "%.1f%%", humidity_event.relative_humidity);
  drawTextCenter(96, 48, buffer);


  u8g2.setFont(u8g2_font_luBS24_tn);
  String now = tz.dateTime("HH:ii");
  drawTextCenter(64, 16, now.c_str());

  u8g2.sendBuffer();
}


void drawTextCenter(int x, int y, const char* text) {
  int width = u8g2.getStrWidth(text);
  int height = u8g2.getAscent();

  int posX = x - width / 2;
  int posY = y + height / 2;

  u8g2.drawStr(posX, posY, text);
}
