
#include "wifi.h"
#include "scale.h"
#include <Servo.h>

#define EMPTY_URL "/isEmpty"
#define REFILL_URL "/isRefill"
#define BROKEN_URL "/broken"
#define DEFAULT_URL "/default"

Servo servo;

unsigned long previousMillis = 0;        // will store last time motor was updated
long OnTime = 0.5 * 60000;           // milliseconds of on-time (min * 60000)

//pattern status
//broken - 1
//default - 0
int patternStatus = 0;

//pattern status
//full - 1
//empty - 0
int fillStatus = 1;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //delay(10);
 
  // pinMode(inputpin, INPUT);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  initScale();
}

void loop() {

  //Connect to Wifi
  initWifi();
  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (isReconnecting()) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the Nina module
  stopConnection();

  bowlUpdate();

  //connectAndSendData("/");
}

void bowlUpdate() {
  unsigned long currentMillis = millis();
    
  if (isEmpty()) {
    if (fillStatus == 1) {
      fillStatus = 0;
      //notify user is empty
      connectAndSendData(EMPTY_URL);
      Serial.println("isEmpty");
    }
    return;
    
  } else if (isRefill()) {
    fillStatus = 1;
    //stop notifiying user is empty
    connectAndSendData(REFILL_URL);
    Serial.println("isRefill");
  }

  if (didUserEatFruit()) {
    //remember last time
    previousMillis = currentMillis;

    if (patternStatus == 1) {
      Serial.println("pattern default state");
      Serial.println("pattern status: " + String(patternStatus));
      patternStatus = 0;
      //reset patern
      connectAndSendData(DEFAULT_URL);
      //servo.write(0);
    }
  } else if (currentMillis - previousMillis >= OnTime) {
    if (patternStatus == 0) {
      Serial.println("pattern is broken");
      Serial.println("pattern status: " + String(patternStatus));
      patternStatus = 1;
      //time to eat fruit
      connectAndSendData(BROKEN_URL);
      //servo.write(45);
    }
  }


}
