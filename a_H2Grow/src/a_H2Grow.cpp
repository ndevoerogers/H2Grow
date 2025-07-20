/* 
 * Project a_H2Grow
 * Author: Nicole DeVoe Rogers
 * Date: 20 July 2025
 * Description: OLED, BME and Dust Sensor
 */


#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_BME280.h"
#include "IoTClassroom_CNM.h"

bool status;
float tempc, tempf;
float humidRH;
const int OLED = -1;
const int ADDRESS = 0x76;
const int OLEDADDRESS = 0x3C;

Adafruit_BME280 bme;
IoTTimer timer;
Adafruit_SSD1306 display (OLED);


SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 5000);

  status = bme.begin(ADDRESS);
  if (status = false) {
    Serial.printf("BME@*) at address 0x%02X failed to start", ADDRESS);
  }
display.begin(SSD1306_SWITCHCAPVCC, OLEDADDRESS);
display.setTextSize(1);
display.setRotation (0);
display.setTextColor(WHITE);

timer.startTimer(500);
}


void loop() {
  display.clearDisplay();
  tempc = bme.readTemperature();
  humidRH = bme.readHumidity();


  tempf = (tempc *9/5.0) + 32;
  if (timer.isTimerReady()) {
    Serial.printf("Temperature %0.1f\nHumidity %0.1f\n", tempf, humidRH);
    display.setCursor (0,0);
    display.printf("Temperature %0.1f\nHumidity %0.1f\n", tempf, humidRH);
    display.display ();
    timer.startTimer(500);

  }
}
