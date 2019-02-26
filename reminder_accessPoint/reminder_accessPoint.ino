/* Create a WiFi access point and provide a web server on it. */

#include "wifi.h"

#include <Servo.h>

#define EMPTY_URL "/isEmpty"
#define REFILL_URL "/isRefill"
#define BROKEN_URL "/broken"
#define DEFAULT_URL "/default"



/*
  const int LED = ;
  const int servoMotor = ;
*/

//Declaration of variables to control the servo and the server
Servo servo;

void isEmpty() {
  //digitalWrite(LED, HIGH);
  Serial.println("EMPTY");
  server.send(200, "text/html", "Is Empty");
}

void isRefill() {
  //digitalWrite(LED, LOW);
  Serial.println("REFILL");
  server.send(200, "text/html", "Is Refill");
}

void breakPattern() {
  //servo.write(45);
  Serial.println("BROKEN");
  server.send(200, "text/html", "Broken");
}

void resetPattern() {
  //servo.write(0);
  Serial.println("DEFAULT");
  server.send(200, "text/html", "Reset");
}

void setup() {

  Serial.begin(115200);
  Serial.println("server hello");
  //Initialize servo and LED as OUTPUTS
  /*pinMode(LED, OUTPUT);
    pinMode(servoMotor, OUTPUT);*/

  //attach the pin to the servo
  /*
    servo.attach(servoMotor);
  */

  initWifi();
  initServer();

  server.on("/", handleRoot);

  server.on(EMPTY_URL, isEmpty);

  server.on(REFILL_URL, isRefill);

  server.on(BROKEN_URL, breakPattern);

  server.on(DEFAULT_URL, resetPattern);
}

void loop() {
  listenClient();
}
