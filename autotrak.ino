#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define LEFT_MOTOR D1
#define RIGHT_MOTOR D2

#define WIFI_SSID "DITO ANG WIFI NAME"
#define WIFI_PASSWORD "DITO ANG WIFI PASSWORD"

#define API_KEY "AIzaSyALdciRWibe5zTWarsi0yuzvMIvAkE7yww"
#define DATABASE_URL "autotrak-31c44-default-rtdb.firebaseio.com/"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;


void setup() {
  Serial.begin(115200);

  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  digitalWrite(LEFT_MOTOR, HIGH);
  digitalWrite(RIGHT_MOTOR, HIGH);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase connection successful");
    signupOK = true;
  
  } else {
    Serial.printf("Firebase sign-up error: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Firebase.RTDB.setBool(&fbdo, "isConnected", true);

}

void loop() {
  if (Firebase.ready() && signupOK &&
      (millis() - sendDataPrevMillis > 200 || sendDataPrevMillis == 0)) {

    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getBool(&fbdo, "forward")) {
      if (fbdo.dataType() == "boolean"){
      bool forwardStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + forwardStateStr + "(" + fbdo.dataType() + ")");
      bool forward = (forwardStateStr == false) ? HIGH : LOW;
      digitalWrite(LEFT_MOTOR, forward);
      digitalWrite(RIGHT_MOTOR, forward);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "left")) {
      if (fbdo.dataType() == "boolean"){
      bool leftStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + leftStateStr + "(" + fbdo.dataType() + ")");
      bool left = (leftStateStr == false) ? HIGH : LOW;
      digitalWrite(RIGHT_MOTOR, left);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "right")) {
      if (fbdo.dataType() == "boolean"){
      bool rightStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + rightStateStr + "(" + fbdo.dataType() + ")");
      bool right = (rightStateStr == false) ? HIGH : LOW;
      digitalWrite(LEFT_MOTOR, right);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }

  }
}
