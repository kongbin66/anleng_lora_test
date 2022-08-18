

String apiKey = "SKUROA16O6ONTBTT"; // Enter your Write API key from ThingSpeak
const char *ssid = "anlengkj";     // replace with your wifi ssid and wpa2 key
const char *password = "al77776666";

// Import Wi-Fi library
#include <WiFi.h>

// Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// define the pins used by the LoRa transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define BAND 433E6 // 433E6 for Asia, 866E6 for Europe, 915E6 for North America

// Replace with your network credentials
String apiKey = "14K8UL2QEK8BTHN6"; // Enter your Write API key from ThingSpeak
const char *ssid = "Alexahome";     // replace with your wifi ssid and wpa2 key
const char *password = "12345678";
const char *server = "api.thingspeak.com";

WiFiClient client;

// Initialize variables to get and save LoRa data
int rssi;
String loRaMessage;
String temperature;
String humidity;
String readingID;

// Replaces placeholder with DHT values
String processor(const String &var)
{
    // Serial.println(var);
    if (var == "TEMPERATURE")
    {
        return temperature;
    }
    else if (var == "HUMIDITY")
    {
        return humidity;
    }
    else if (var == "RRSI")
    {
        return String(rssi);
    }
    return String();
}

void setup()
{
    Serial.begin(115200);
    int counter;

    // setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0); // setup LoRa transceiver module

    while (!LoRa.begin(BAND) && counter < 10)
    {
        Serial.print(".");
        counter++;
        delay(2000);
    }
    if (counter == 10)
    {
        // Increment readingID on every new reading
        Serial.println("Starting LoRa failed!");
    }
    Serial.println("LoRa Initialization OK!");
    delay(2000);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(2000);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// Read LoRa packet and get the sensor readings
void loop()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        Serial.print("Lora packet received: ");
        while (LoRa.available()) // Read packet
        {
            String LoRaData = LoRa.readString();
            Serial.print(LoRaData);

            int pos1 = LoRaData.indexOf('/');
            int pos2 = LoRaData.indexOf('&');
            readingID = LoRaData.substring(0, pos1);                    // Get readingID
            temperature = LoRaData.substring(pos1 + 1, pos2);           // Get temperature
            humidity = LoRaData.substring(pos2 + 1, LoRaData.length()); // Get humidity
        }

        rssi = LoRa.packetRssi(); // Get RSSI
        Serial.print(" with RSSI ");
        Serial.println(rssi);
    }

    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
    {
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(readingID);
        postStr += "&field2=";
        postStr += String(temperature);
        postStr += "&field3=";
        postStr += String(humidity);
        postStr += "&field4=";
        postStr += String(rssi);
        postStr += "\r\n\r\n\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
    }
    // delay(30000);
}