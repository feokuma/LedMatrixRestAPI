//#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>
#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#define NUM_LEDS 12
#define DATA_PIN 13

CRGB leds[NUM_LEDS];

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

struct Led
{
    byte numLeds;
    byte status;
} led_resource;

const char *wifi_ssid = "[wifi_ssid]";
const char *wifi_passwd = "[wifi_passwd]";

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

void init_led_resource()
{
    led_resource.numLeds = NUM_LEDS;
    led_resource.status = LOW;
}

int init_wifi()
{
    int retries = 0;

    Serial.println("Connecting to WiFi AP..........");

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_passwd);

    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY))
    {
        retries++;
        delay(WIFI_RETRY_DELAY);
        Serial.print("#");
    }
    return WiFi.status();
}

void get_leds()
{
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &jsonObj = jsonBuffer.createObject();
    char JSONmessageBuffer[200];

    if (led_resource.numLeds == 0)
        http_rest_server.send(204);
    else
    {
        jsonObj["numLeds"] = led_resource.numLeds;
        jsonObj["status"] = led_resource.status;
        jsonObj.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        http_rest_server.send(200, "application/json", JSONmessageBuffer);
    }
}

void json_to_resource(JsonObject &jsonBody)
{
    int numLeds, status;

    numLeds = jsonBody["numLeds"];
    status = jsonBody["status"];

    Serial.println(numLeds);
    Serial.println(status);

    led_resource.numLeds = numLeds;
    led_resource.status = status;
}

void post_leds()
{
    StaticJsonBuffer<500> jsonBuffer;
    String post_body = http_rest_server.arg("plain");
    Serial.println(post_body);

    JsonObject &jsonBody = jsonBuffer.parseObject(http_rest_server.arg("plain"));

    Serial.print("HTTP Method: ");
    Serial.println(http_rest_server.method());

    if (!jsonBody.success())
    {
        Serial.println("error in parsin json body");
        http_rest_server.send(400);
    }
    else
    {
        json_to_resource(jsonBody);
        http_rest_server.sendHeader("Location", "/leds/" + String(led_resource.status));
        http_rest_server.send(201);
        setFastLED(led_resource.status);
    }
}

void setFastLED(byte led)
{
    CRGB color;

    switch (led)
    {
        case 1:
            color = 0x040000;//CRGB::Green;
            break;
        case 2:
            color = 0x000400;//CRGB::Red;
            break;
        case 3:
            color = 0x040400;//CRGB::Orange;
            break;
        default:
            color = CRGB::Black;
            break;
    }

    for(char i = 0; i < led_resource.numLeds; i++)
        leds[i] = color;

    FastLED.show();
}

void config_rest_server_routing()
{
    http_rest_server.on("/", HTTP_GET, []() {
        http_rest_server.send(200, "text/html",
                              "Welcome to the ESP8266 REST Web Server");
    });
    http_rest_server.on("/leds", HTTP_GET, get_leds);
    http_rest_server.on("/leds", HTTP_POST, post_leds);
}

void setup(void)
{
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

    Serial.begin(115200);

    init_led_resource();
    if (init_wifi() == WL_CONNECTED)
    {
        Serial.print("Connected to ");
        Serial.print(wifi_ssid);
        Serial.print(" --- IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.print("Error connecting to: ");
        Serial.println(wifi_ssid);
    }

    config_rest_server_routing();

    http_rest_server.begin();
    Serial.println("HTTP REST Server Started");
}

void loop(void)
{
    http_rest_server.handleClient();
}
