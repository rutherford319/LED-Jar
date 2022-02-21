#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "LedControl.h"

// #ifndef DATASTORAGE
// #define DATASTORAGE
// #include "DataStorage.h"
// #endif

String mainBody = "<!DOCTYPE html><html><style>.flex-container{display: flex; align-items: stretch;}.flex-container>div{text-align: center; font-size: 30px;}</style><body> <h1 style=\"text-align: center;\">LED JAR</h1> <div class=\"flex-container\"> <form action=\"/hex\" method=\"POST\"> <label for=\"hex\">Select colour </label><input type=\"color\" id=\"hex\" name=\"hex\" value=\"#e66465\" onchange=\"this.form.submit()\"> </form> </div><div style=\"padding-top: 50px;\"> <h2 style=\"text-align: center; \">Random</h2> <div class=\"flex-container\"> <div style=\"flex-grow: 1\">1ms</div><div class=\"slidecontainer\" style=\"flex-grow: 6\"> <form action=\"/editRandomSpeed\" method=\"POST\"> <input type=\"range\" min=\"1\" max=\"10000\" value=\"5000\" class=\"slider\" id=\"speed\" name=\"speed\" style=\"width: 100%;\" onchange=\"this.form.submit()\"> </form> </div><div style=\"flex-grow: 1\">10s</div></div></div><div class=\"flex-container\"> <div style=\"flex-grow: 6\"> <form action=\"/toggleRandom\"> <input type=\"submit\" value=\"Toggle random\" style=\"width: 100%; height: 100px; font-size: large;\"/></form> </div></div>";
String endHtml = "</body></html>";

DataStorage storage;
ESP8266WebServer server(80);

bool softRandomToggle = false;
bool fixedRandomToggle = false;

int randomSpeed = 500;

int ledPinR = 4;  // the number of the LED R pin - D2
int ledPinG = 5;  // the number of the LED G pin - D1
int ledPinB = 16; // the number of the LED B pin - D0

LedControl led(ledPinR, ledPinG, ledPinB, &storage);

void handleRootMsg(String msg)
{
  String formattedMsg = "";
  if (msg != "")
  {
    formattedMsg = "<div class=\"flex-container\" style=\"background-color: lightpink;\"> <h3 style=\"flex-grow: 6; text-align: center; \">" + msg + "</h3></div>";
  }

  server.send(200, "text/html", mainBody + formattedMsg + endHtml);
}

char *getCharArrayFromString(String str)
{
  char *chr = (char *)malloc(str.length() + 1);
  str.toCharArray(chr, str.length() + 1);
  return chr;
}

void loadUserConfiguration()
{
  softRandomToggle = storage.getSoftRandom();
  fixedRandomToggle = storage.getFixedRandom();
  randomSpeed = storage.getRandomSpeed();

  led.setRGB(storage.getR(), storage.getG(), storage.getB(), false);
}

void handleRoot()
{
  handleRootMsg("");
}

void handleRandomToggle()
{
  softRandomToggle = !softRandomToggle;
  storage.writeSoftRandom(softRandomToggle);

  String status = softRandomToggle ? "enabled" : "disabled";
  handleRootMsg("Random now " + status);
}

bool isBadRequest(String requiredArg)
{
  if (!server.hasArg(requiredArg) || server.arg(requiredArg) == NULL)
  {
    server.send(400, "text/plain", "400: Invalid Request. " + requiredArg + " missing. "); // The request is invalid, so send HTTP status 400
    return true;
  }
  return false;
}
void handleRandomSpeed()
{
  if (isBadRequest("speed")) return;

  randomSpeed = server.arg("speed").toInt();
  storage.writeRandomSpeed(randomSpeed);

  handleRootMsg("Speed set!");
}

void handleHex()
{
  if (isBadRequest("hex")) return;

  led.setHex(server.arg("hex"));

  handleRootMsg("Success! </br>");
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("ENTER-SSID", "ENTER-PASSWORD");

  Serial.begin(9600);
  storage = DataStorage();
  loadUserConfiguration();

  server.on("/", handleRoot);
  server.on("/hex", handleHex);
  server.on("/editRandomSpeed", handleRandomSpeed);
  server.on("/toggleRandom", handleRandomToggle);
  server.onNotFound(handleNotFound);

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(20);
    Serial.print(".");
  }

  if (MDNS.begin("esp8266"))
  { // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  }

  server.begin();
}

void loop(void)
{
  server.handleClient();

  if (fixedRandomToggle)
  {
    led.iterateFixedRandom(randomSpeed);
  }
  else if (softRandomToggle)
  {
    led.iterateGradualRandom(randomSpeed);
  }
  else
  {
    delay(200);
  }
}
