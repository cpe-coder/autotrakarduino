#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Servo.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define LEFT_ACTUATOR D1
#define RIGHT_ACTUATOR D2 

#define DOWN_ACTUATOR D3
#define UP_ACTUATOR D4
#define power D6

Servo m1;
Servo servo;

#define WIFI_SSID "FERRYTALE 3782"
#define WIFI_PASSWORD "mamamoblue"

#define API_KEY "AIzaSyALdciRWibe5zTWarsi0yuzvMIvAkE7yww"
#define DATABASE_URL "autotrak-31c44-default-rtdb.firebaseio.com/"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;
unsigned long sendDataPrevMillis = 0;


void setup() {
  Serial.begin(115200);
  servo.attach(D5);
  pinMode(LEFT_ACTUATOR, OUTPUT);
  pinMode(RIGHT_ACTUATOR, OUTPUT);
  pinMode(DOWN_ACTUATOR, OUTPUT);
  pinMode(UP_ACTUATOR, OUTPUT);
  pinMode(power, OUTPUT);
  digitalWrite(LEFT_ACTUATOR, HIGH);
  digitalWrite(RIGHT_ACTUATOR, HIGH);
  digitalWrite(DOWN_ACTUATOR, HIGH);
  digitalWrite(UP_ACTUATOR, HIGH);
  digitalWrite(power, HIGH);
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

    if (Firebase.RTDB.getInt(&fbdo, "forward")) {
      if (fbdo.dataType() == "int"){
        int powerValue = fbdo.intData();
        powerValue = constrain(powerValue, 0, 180);
        Serial.println("Seccess: " + fbdo.dataPath() + ":" + powerValue);
        servo.write(powerValue);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.getBool(&fbdo, "left")) {
      if (fbdo.dataType() == "boolean"){
      bool leftStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + leftStateStr + "(" + fbdo.dataType() + ")");
      bool left = (leftStateStr == false) ? HIGH : LOW;
      digitalWrite(LEFT_ACTUATOR, left);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "right")) {
      if (fbdo.dataType() == "boolean"){
      bool rightStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + rightStateStr + "(" + fbdo.dataType() + ")");
      bool right = (rightStateStr == false) ? HIGH : LOW;
      digitalWrite(RIGHT_ACTUATOR, right);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.getBool(&fbdo, "suyodDown")) {
      if (fbdo.dataType() == "boolean"){
      bool downStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + downStateStr + "(" + fbdo.dataType() + ")");
      bool down = (downStateStr == false) ? HIGH : LOW;
      digitalWrite(DOWN_ACTUATOR, down);
      }
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "suyodUp")) {
      if (fbdo.dataType() == "boolean"){
      bool upStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + upStateStr + "(" + fbdo.dataType() + ")");
      bool up = (upStateStr == false) ? HIGH : LOW;
      digitalWrite(UP_ACTUATOR, up);
      }  
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "power")) {
      if (fbdo.dataType() == "boolean"){
      bool powerStateStr = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + powerStateStr + "(" + fbdo.dataType() + ")");
      bool POWER = (powerStateStr == false) ? HIGH : LOW;
      digitalWrite(power, POWER);
      }  
      
    } else {
      Serial.println("Failed to read Auto: " + fbdo.errorReason());
    }

  }
}
