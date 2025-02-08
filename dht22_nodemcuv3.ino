#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  Serial.println("Attempting to connect to WiFi...");
  WiFi.begin("TurkTelekom_TP33BE_2.4GHz", "vsA9tjdPj9sC"); 

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 60) { // 60 * 500ms = 30 seconds
      Serial.println("Failed to connect to WiFi");
      return;
    }
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    server.send(500, "text/plain", "Sensor error");
    return;
  }

  String message = "Temperature: " + String(t) + " C\n";
  message += "Humidity: " + String(h) + " %\n";
  server.send(200, "text/plain", message);
}
