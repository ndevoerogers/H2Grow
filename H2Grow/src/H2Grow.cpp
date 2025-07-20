/* 
 * Project H2Grow
 * Author: Nicole DeVoe Rogers
 * Date: 18 July 2025
 * Description: Midterm 2- plant watering system
 */


#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);


void setup() {
  // Put initialization like pinMode and begin functions here
}
/ loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!
}
