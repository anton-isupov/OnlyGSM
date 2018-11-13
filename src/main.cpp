#include <Arduino.h>
#include "GSM/GSM.h"
#include "sensorTemperature/sensorTemperature.h"

const uint8_t RELE_START_HEAT_PIN = 8;
const uint8_t RELE_STOP_HEAT_PIN = 9;

const String availableNumbers[2] = {
    "+79503422666", 
    "+79202979541"
};

boolean checkNumberPermission(String number);
void doActionForEachNumber(void (*callback)(String, String), String message);
static void sendMessage(String phone, String message);

GSM gsm(10,11);
sensorTemperature sensor(6);

void setup() {
    Serial.begin(38400);
    sensor.setup();
    gsm.setup();
    pinMode(RELE_START_HEAT_PIN, OUTPUT);
    Serial.println("Setuping...");

    if (gsm.onReady()) {
        Serial.println("GSM ready");
        doActionForEachNumber(sendMessage, "Ready to start");
    }
}

bool lowTemp = true;
void loop() {
    gsm.run();
    int currentTemperature = sensor.getTemperature();
    if (currentTemperature < 3 && lowTemp == true) {
        doActionForEachNumber(sendMessage, "current temperature < 3");
        lowTemp = false;
    }

    if (currentTemperature > 3 && lowTemp == false) {
        doActionForEachNumber(sendMessage, "current temperature > 3");
        lowTemp = true;
    }

    if (gsm.onResponse()) {
        String number = gsm.getNumber();
        String result = gsm.getResponse();
        Serial.println(result);
        Serial.println(gsm.getResponse());

        if (result.equals("startHeat") && checkNumberPermission(number)) {
            digitalWrite(RELE_START_HEAT_PIN,HIGH);
            delayUpgrate(500);
            digitalWrite(RELE_START_HEAT_PIN, LOW);
            gsm.sendSMS(number,"Pressed startHeat");
        }
        if (result.equals("getTemperature") && checkNumberPermission(number)) {
            String temp = (String) sensor.getTemperature();
            gsm.sendSMS(number,"Temperature: " + temp);
        }
        if (result.equals("stopHeat") && checkNumberPermission(number)) {
            digitalWrite(RELE_STOP_HEAT_PIN,!digitalRead(RELE_STOP_HEAT_PIN));
            gsm.sendSMS(number,"Pressed stopHeat");
        }
    }
}

boolean checkNumberPermission(String number) {
    for(unsigned int i=0; i<2; i++) {
        if(number.equals(availableNumbers[i])) {
            return true;
        }
    }
    return false;
}

void doActionForEachNumber(void (*callback)(String, String), String message) {
    Serial.println("Starting do for each");
    for(unsigned int i=0; i<2; i++) {
        Serial.println("Do callback for " + availableNumbers[i]);
        callback(availableNumbers[i], message);
    }
}

static void sendMessage(String phone, String message) {
    gsm.sendSMS(phone, message);
    delayUpgrate(3000);
}
void delayUpgrate(int time) {
int borderTime = millis();
while (millis() - borderTime < time) {}
}