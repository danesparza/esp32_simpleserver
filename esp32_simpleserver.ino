#include <WiFi.h>
#include <FastLED.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ========================================
// LED Setup
// ========================================

#define DATA_PIN 48
#define NUM_LEDS 1

CRGB leds[NUM_LEDS];

// ========================================
// WiFi Setup
// ========================================

const char* ssid = "Nahual";
const char* password = "huaychivo";

// ========================================
// Async Web Server
// ========================================

AsyncWebServer server(80);

// ========================================
// HTML Page
// ========================================

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 LED Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin-top: 50px;
        }

        button {
            font-size: 24px;
            padding: 20px;
            margin: 10px;
            width: 200px;
        }
    </style>
</head>
<body>
    <h1>ESP32 LED Control</h1>

    <p>
        <a href="/H">
            <button>Green</button>
        </a>
    </p>

    <p>
        <a href="/L">
            <button>Blue</button>
        </a>
    </p>
</body>
</html>
)rawliteral";

// ========================================
// Setup
// ========================================

void setup() {

    Serial.begin(115200);

    // Initialize LED
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    leds[0] = CRGB::Black;
    FastLED.show();

    // ====================================
    // Connect WiFi
    // ====================================

    WiFi.begin(ssid, password);

    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Disable WiFi power save
    WiFi.setSleep(false);

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // ====================================
    // Web Routes
    // ====================================

    // Main page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(
            200,
            "text/html",
            index_html
        );
    });

    // Turn LED green
    server.on("/H", HTTP_GET, [](AsyncWebServerRequest *request) {

        leds[0] = CRGB::Green;
        FastLED.show();

        request->redirect("/");
    });

    // Turn LED blue
    server.on("/L", HTTP_GET, [](AsyncWebServerRequest *request) {

        leds[0] = CRGB::Blue;
        FastLED.show();

        request->redirect("/");
    });

    // Start server
    server.begin();

    Serial.println("HTTP server started");
}

// ========================================
// Loop
// ========================================

void loop() {

    // Empty!
    // Async server runs independently.
}