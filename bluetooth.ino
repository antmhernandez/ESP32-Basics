/*
  Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
  by Daniel Carrasco -> https://www.electrosoftcloud.com/
*/
#include "BluetoothSerial.h" // We will include the Serial Bluetooth header
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#define LED 23
BluetoothSerial BT; // Bluetooth Object
void setup() {
  Serial.begin(9600); // Initializing serial connection for debugging
  BT.begin("ESP32_LED_Control"); // Name of your Bluetooth Device and in slave mode
  Serial.println("Bluetooth Device is Ready to Pair");
  pinMode (LED, OUTPUT); // Change the LED pin to OUTPUT
}
void loop() {
  if (BT.available()) // Check if we receive anything from Bluetooth
  {
    int incoming = BT.read(); // Read what we recevive 
    Serial.print("Received: ");
    Serial.println(incoming);
    if (incoming == 49){ // 1 in ASCII
      digitalWrite(LED, HIGH); // LED On
      BT.println("LED turned ON"); // Send the text via BT Serial
    }
    if (incoming == 48){ // 0 in ASCII
      digitalWrite(LED, LOW); // LED Off
      BT.println("LED turned OFF"); // Send the text via BT Serial
    }
  }
  delay(20);
}
