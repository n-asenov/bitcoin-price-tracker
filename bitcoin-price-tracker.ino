#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "wifi-network";
const char* password = "wifi-password";

const char* host = "api.coingecko.com";
const int port = 443;
String url = "/api/v3/simple/price?ids=bitcoin&vs_currencies=usd";
String request = String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n";
               
// Use web browser to view and copy SHA1 fingerprint of the certificate
const char* fingerprint PROGMEM = "89 25 60 5D 50 44 FC C0 85 2B 98 D7 D3 66 52 28 68 4D E6 E2";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  connectToWiFiNetwork();

  String response = connectToAPI();
  Serial.println(response);
}

void connectToWiFiNetwork() {
  Serial.print("Connecting to ");
  Serial.print("ssid");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

String connectToAPI() {
  Serial.print("Connecting to ");
  Serial.println(host);

  Serial.print("Using fingerprint: ");
  Serial.println(fingerprint);
  
  WiFiClientSecure client;
  client.setFingerprint(fingerprint);

  if (!client.connect(host, port)) {
    Serial.print("Failed to connect to ");
    Serial.println(host);
    return "";
  }

  Serial.print("Sending request to ");
  Serial.println(url);

  client.print(request);

  Serial.println("Request sent");
  
  while (client.connected()) {
    String responseHeader = client.readStringUntil('\n');
    
    if (responseHeader == "\r") {
      Serial.println("Headers received");
      break;
    }
    
    Serial.println(responseHeader);
  }

  client.readStringUntil('\n');
  String body = client.readStringUntil('\n');

  Serial.println(body);
  Serial.println("Response body received");
  Serial.println("Closing connection");
  
  return body;
}

void loop() {
  // put your main code here, to run repeatedly:
}
