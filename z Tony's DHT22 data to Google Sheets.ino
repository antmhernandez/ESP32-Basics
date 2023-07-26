//Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
// WiFi credentials
const char* ssid = "A";         // change SSID
const char* password = "password";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbygFOU2t2zmLH0OLfEHPqi_IL-qGFQGJBZ8t-PWOCPMZylGVCbv2iW61Hr8HzGNtNoM";    // change Gscript ID
int count = 0;


#include "DHT.h"
#define DHTPIN 13 
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    dht.begin();
}
void loop() {

// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  
  
   if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"time=" + asString + "&humidity=" + String(h) + "&tempc=" + String(t) + "&tempf=" + String(f);
    //String urlFinal = "https://script.google.com/macros/s/AKfycbygFOU2t2zmLH0OLfEHPqi_IL-qGFQGJBZ8t-PWOCPMZylGVCbv2iW61Hr8HzGNtNoM/exec?time=50&humidity=1&tempc=2&tempf=3";
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
  count++;
  delay(1000);


  
} 
