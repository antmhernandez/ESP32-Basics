#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "yourNetworkName";
const char* password =  "yourNetworkPass";
 
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Louisville,US,pt&APPID=";
//Example API Call:    https://api.openweathermap.org/data/2.5/weather?q=Louisville,US,pt&APPID=ded70c154c37972f77d0228456d87d16
const String key = "yourAPIkey";
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); //Specify the URL which joins the API call and your API Key
    int httpCode = http.GET();  //Make the request. Return the success value of a request made at the URL.
        /*To make the actual request, we simply need to call the GET method on our HTTPClient object. 
        This method takes no arguments and returns the HTTP response code, in case of successfully sending the request.
        Note that in case the returned value is lesser than zero, then it means an internal error has occurred on the ESP32 and thus it will not correspond to a HTTP response code. 
        In that case, the returned value will correspond to a specific internal error. 
        You can check here the possible internal error codes: https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h#L36 */
 
    if (httpCode > 0) { //Check for the returning code

          /*call the getString method of the HTTPClient object, which takes no arguments and returns the response in a String format.*/
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(30000);
 
}
