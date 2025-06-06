#include <WiFi.h>
#include <WebServer.h>

const char* wifiSSID = "raki";
const char* wifiPassword = "bembem01!";

WebServer webServer(80);

const int ledPinNumbers[] = {15, 2, 4, 16, 17};  // Pins for 5 LEDs
const String ledLabels[] = {"LED1", "LED2", "LED3", "LED4", "LED5"};

void handleRoot() {
  String htmlContent = "<html><body><h1>Navigate the 5 LEDs</h1>";
  htmlContent += "<p><a href=\"/allOn\"><button>Turn ALL LEDs ON</button></a></p>";
  htmlContent += "<p><a href=\"/alternateOn\"><button>Switch LEDs On Alternately</button></a></p>";
  htmlContent += "<p><a href=\"/centerToBottomOn\"><button>Light LEDs from Center to Edge</button></a></p>";
  htmlContent += "<p><a href=\"/allOff\"><button>Turn ALL LEDs OFF</button></a></p>";

  for (int i = 0; i < 5; i++) {
    htmlContent += "<p>" + ledLabels[i] + ": ";
    htmlContent += "<a href=\"/on" + String(i) + "\"><button>ON</button></a> ";
    htmlContent += "<a href=\"/off" + String(i) + "\"><button>OFF</button></a></p>";
}

  htmlContent += "</body></html>";
  webServer.send(200, "text/html", htmlContent);
}

void handleLEDOn(int ledIndex) {
  digitalWrite(ledPinNumbers[ledIndex], HIGH);
  webServer.send(200, "text/plain", ledLabels[ledIndex] + " is ON");
}

void handleLEDOff(int ledIndex) {
  digitalWrite(ledPinNumbers[ledIndex], LOW);
  webServer.send(200, "text/plain", ledLabels[ledIndex] + " is OFF");
}

void handleAllLEDsOn() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPinNumbers[i], HIGH);
  }
  webServer.send(200, "text/plain", "All LEDs are ON");
}

void handleAlternateLEDsOn() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPinNumbers[i], LOW);
  }

  for (int i = 0; i < 5; i += 2) { 
    digitalWrite(ledPinNumbers[i], HIGH);  
    delay(500);  
    digitalWrite(ledPinNumbers[i], LOW);   
    delay(500);  
  }

  webServer.send(200, "text/plain", "LEDs turned ON and OFF in sequence");
}

void handleCenterToBottomLEDsOn() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPinNumbers[i], LOW);
  }
  
  digitalWrite(ledPinNumbers[2], HIGH);  
  delay(500);  
  digitalWrite(ledPinNumbers[2], LOW);   
  delay(500);  
  
  digitalWrite(ledPinNumbers[1], HIGH);  
  digitalWrite(ledPinNumbers[3], HIGH);  
  delay(500);  
  digitalWrite(ledPinNumbers[1], LOW);   
  digitalWrite(ledPinNumbers[3], LOW);   
  delay(500);  
  
  digitalWrite(ledPinNumbers[0], HIGH);  
  digitalWrite(ledPinNumbers[4], HIGH);  
  delay(500);  
  digitalWrite(ledPinNumbers[0], LOW);   
  digitalWrite(ledPinNumbers[4], LOW);   
  
  webServer.send(200, "text/plain", "LEDs turned ON and OFF from center to bottom");
}

void handleAllLEDsOff() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPinNumbers[i], LOW);
  }
  webServer.send(200, "text/plain", "All LEDs are OFF");
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 5; i++) {
    pinMode(ledPinNumbers[i], OUTPUT);
    digitalWrite(ledPinNumbers[i], LOW);
  }

  WiFi.begin(wifiSSID, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  webServer.on("/", handleRoot);
  webServer.on("/allOn", handleAllLEDsOn);
  webServer.on("/alternateOn", handleAlternateLEDsOn);
  webServer.on("/centerToBottomOn", handleCenterToBottomLEDsOn);
  webServer.on("/allOff", handleAllLEDsOff);
  for (int i = 0; i < 5; i++) {
    webServer.on("/on" + String(i), [i]() { handleLEDOn(i); });
    webServer.on("/off" + String(i), [i]() { handleLEDOff(i); });
  }

  webServer.begin();
}

void loop() {
  webServer.handleClient();
}
