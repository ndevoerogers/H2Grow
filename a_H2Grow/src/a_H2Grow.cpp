/* 
 * Project a_H2Grow
 * Author: Nicole DeVoe Rogers
 * Date: 20 July 2025
 * Description: OLED, BME and Dust Sensor
 */


#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
 #include "Adafruit_BME280.h"
#include "IoTClassroom_CNM.h"
#include "math.h"
#include "credentials.h"

TCPClient TheClient; //global state

Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

Adafruit_MQTT_Subscribe subFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/h20button"); 
Adafruit_MQTT_Publish pubFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dust");
Adafruit_MQTT_Publish pubFeed2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish pubFeed3 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/soilmoisture");
Adafruit_MQTT_Publish pubFeed4 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

float Dust;
float Humidity;
float soilMoisture;
float Temperature;
bool h20Button;
void MQTT_connect();
bool MQTT_ping();

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
const int DRY = 2300;//soil
int soilVal;//soil sensor


Adafruit_BME280 bme;
IoTTimer timer, soilTimer, pumpTimer;
Adafruit_SSD1306 display (OLED);


SYSTEM_MODE(SEMI_AUTOMATIC);


void setup() {

  WiFi.on();
  WiFi.connect();
  while(WiFi.connecting()) {
  Serial.printf(".");
  
  Serial.printf("\n\n");
  }
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
  soilTimer.startTimer(30000);

  pinMode(pin,INPUT); //dust sensor
  starttime = millis(); //dust sensor

  pinMode(A0, INPUT);
  pinMode(PUMP, OUTPUT);

  mqtt.subscribe(&subFeed);
  
}


void loop() {
  display.clearDisplay();
  tempc = bme.readTemperature();
  humidRH = bme.readHumidity();
  soilVal = analogRead(A0);

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
    Serial.printf("Temperature %0.1f\nHumidity %0.1f\nDust Concen %0.1f\nSoil Moisture %i\n", tempf, humidRH, concentration, soilVal);
    display.printf("Temperature %0.1f\nHumidity %0.1f\nDust Concen %0.1f\nSoil Moisture %i\n", tempf, humidRH, concentration, soilVal);
    display.display ();
    timer.startTimer(1500);
  }
  if(soilTimer.isTimerReady()) {

    Serial.printf("Soil Moisture %i\n", soilVal);
    soilTimer.startTimer(30000);

    if (soilVal >= DRY) {
      pumpTimer.startTimer(500);
      digitalWrite(PUMP,HIGH);
      
      

    }
  }  
  if(pumpTimer.isTimerReady()){
    digitalWrite(PUMP, LOW);
  }
}
// digitalWrite(PUMP, HIGH);
  







  