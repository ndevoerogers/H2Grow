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
#include "math.h"

bool status;
float tempc, tempf;
float humidRH;
float dust;

const int OLED = -1;
const int ADDRESS = 0x76;
const int OLEDADDRESS = 0x3C;

int pin = A1; 
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

const int PUMP = D19;
const int dry = 500;//soil
const int wet = 239; //soil
int soilVal;//soil


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

  pinMode(pin,INPUT); //dust sensor
  starttime = millis(); //dust sensor

  pinMode(soilVal, OUTPUT);



void loop() {
  display.clearDisplay();
  tempc = bme.readTemperature();
  humidRH = bme.readHumidity();


  tempf = (tempc *9/5.0) + 32;
  duration = pulseIn(pin, LOW); //dust
  lowpulseoccupancy = lowpulseoccupancy+duration; //dust

  if ((millis()-starttime) > sampletime_ms) {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  //dust
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; //dust
    lowpulseoccupancy = 0; //dust
    starttime = millis();
    } 
  if (timer.isTimerReady()) {
    display.clearDisplay();
    display.setCursor (0,0);
    Serial.printf("Temperature %0.1f\nHumidity %0.1f\nDust Concen %0.1f\n", tempf, humidRH, concentration);
    display.printf("Temperature %0.1f\nHumidity %0.1f\nDust Concen %0.1f\n", tempf, humidRH, concentration);
    display.display ();
    timer.startTimer(1500);
    }

  analogRead(soilVal);
  Serial.printf("Soil M %0.1f\n", soilVal);
  }
// digitalWrite(PUMP, HIGH);
  






  }





  