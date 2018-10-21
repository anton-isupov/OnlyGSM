#include <Arduino.h>
#include "GSM/GSM.h"
#include "sensorTemperature/sensorTemperature.h"

GSM gsm(10,11);
sensorTemperature sensor(6);

void setup() {
    sensor.setup();
    gsm.setup();
    pinMode(8, OUTPUT);

    if (gsm.onReady()) gsm.sendSMS("+79202979541","Ready to start");
}
void loop() {
    if (gsm.onResponse()) {
        String result = gsm.getResponse();
        Serial.println(result);
        Serial.println(gsm.getResponse());

        if (result.equals("startHeat")) {
            digitalWrite(8,HIGH);
            delay(500);
            digitalWrite(8, LOW);
            gsm.sendSMS("+79202979541","Pressed");
        }
        if (result.equals("getTemperature")) {
            digitalWrite(12, HIGH);
            String temp = (String) sensor.getTemperature();
            gsm.sendSMS("+79202979541","Temperature: " + temp);
        }
    }
}