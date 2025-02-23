#include <WiFi.h>  // Include Wi-Fi library for ESP32

// Wi-Fi Access Point credentials (ESP32 will create this network)
const char *ssid = "ESP32_Control";  // Name of the ESP32 Wi-Fi network
const char *password = "12345678";   // Password for ESP32 network

int bulbPin = 2; // GPIO pin where LED (or relay for bulb) is connected

WiFiServer server(80); // Create a web server on port 80

void setup() {
    Serial.begin(115200);  // Start serial communication for debugging
    pinMode(bulbPin, OUTPUT); // Set bulb pin as OUTPUT
    digitalWrite(bulbPin, HIGH); // Bulb initially ON

    // Start ESP32 in AP Mode
    WiFi.softAP(ssid, password);
    Serial.println("ESP32 Access Point Started");
    
    // Print the ESP32's IP Address (default: 192.168.4.1)
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.begin(); // Start the web server
}

void loop() {
    WiFiClient client = server.available(); // Check for client connection

    if (client) { // If a new client connects
        Serial.println("New Client Connected");

        String request = client.readStringUntil('\r'); // Read client request
        client.flush(); // Clear buffer

        // If the client requests "/OFF", turn OFF the bulb
        if (request.indexOf("/OFF") != -1) {
            digitalWrite(bulbPin, LOW); // Turn OFF the bulb
            Serial.println("Bulb Turned OFF");
        }

        // If the client requests "/OFF", turn OFF the bulb
        if (request.indexOf("/ON") != -1) {
            digitalWrite(bulbPin, HIGH); // Turn OFF the bulb
            Serial.println("Bulb Turned ON");
        }

        // Send a simple webpage response
        client.println("HTTP/1.1 200 OK"); // HTTP response header
        client.println("Content-Type: text/html"); // Specify HTML content
        client.println();
        client.println("<html><body><h1>ESP32 Bulb Control</h1>");
        client.println("<a href='/OFF'><button>Turn OFF</button></a>");
        client.println("<a href='/ON'><button>Turn ON</button></a>");

        client.println("</body></html>");
        client.stop(); // Close the connection
    }
}
