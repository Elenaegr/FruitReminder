

#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
bool isConnectedToServer = false;

// Initialize the Wifi client library
WiFiClient client;

// server address:
IPAddress server(192, 168, 4, 1);

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 80L; // delay between updates, in milliseconds

bool isReconnecting() {
  return (millis() - lastConnectionTime > postingInterval);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void initWifi() {

  if (!isConnectedToServer) {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      isConnectedToServer = false;
      // don't continue
      while (true);
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);

      // wait 10 seconds for connection:
      delay(10000);
    }

    printWifiStatus();
    isConnectedToServer = true;
  }

}

void stopConnection() {
  client.stop();
}

void connectAndSendData(String url) {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println();
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println(String("GET ") + url + " HTTP/1.1");
    client.println("Host: 192.168.4.1");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
