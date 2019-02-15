#include <Arduino.h>
#include "GSM/GSM.h"
#include "sensorTemperature/sensorTemperature.h"

const uint8_t RELE_START_HEAT_PIN = 8;
const uint8_t RELE_POWER_HEAT_PIN = 9;

const String availableNumbers[2] = {
    "+79503422666",
    "+79036009099"
};

void doActionForEachNumber(void (*callback)(String, String), String message);
static void sendMessage(String phone, String message);
void getStatus();
String buildResponseMessage(String airTemperature, String burnTemperature, String burnMachinePower, String burnMachineHeating);

GSM gsm(10,11);
sensorTemperature sensor(6);
sensorTemperature sensorAir(5);
String heatingStarted;

void setup() {
    Serial.begin(38400);
    sensor.setup();
    sensorAir.setup();
    gsm.setup();
    pinMode(RELE_START_HEAT_PIN, OUTPUT);
    pinMode(RELE_POWER_HEAT_PIN, OUTPUT);
    heatingStarted = "OFF";
    Serial.println("Setuping...");

    if (gsm.onReady()) {
        Serial.println("GSM ready");
        doActionForEachNumber(sendMessage, "Controller are ready");
    }
}
void loop() {
    if (gsm.onResponse()) {
        String result = gsm.getResponse();
        Serial.println(result);
        Serial.println(gsm.getResponse());

        if (result.equals("PSWD06021991&COM=heatPowerOn")) {
            digitalWrite(RELE_POWER_HEAT_PIN,HIGH);
            getStatus();
        }
        if (result.equals("PSWD06021991&COM=heatPowerOff")) {
            digitalWrite(RELE_POWER_HEAT_PIN,LOW);
            getStatus();
        }
        if (result.equals("PSWD06021991&COM=startBurn")) {
            digitalWrite(RELE_START_HEAT_PIN,HIGH);
            delay(500);
            digitalWrite(RELE_START_HEAT_PIN, LOW);
            heatingStarted = "ON";
            getStatus();
        }
        if (result.equals("PSWD06021991&COM=getStatus")) {
            getStatus();
        }
    }
}

void getStatus() {
    String powerStatus;
    String tempAir = (String) sensorAir.getTemperature();
    String temp = (String) sensor.getTemperature();
    int power = digitalRead(RELE_POWER_HEAT_PIN);
    if(power == HIGH) {
        powerStatus = "ON";
    } else if(power == LOW) {
        powerStatus = "OFF";
    }
    String response = buildResponseMessage(tempAir, temp, powerStatus, heatingStarted);
    doActionForEachNumber(sendMessage, response);
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
    delay(3000);
}

String buildResponseMessage(String airTemperature, String burnTemperature, String burnMachinePower, String burnMachineHeating) {
    String controllerResponse = "{\"airTemperature\": \""+ airTemperature +
        "\", \"burnTemperature\": \""+ burnTemperature +
        "\", \"burnMachinePower\": \""+ burnMachinePower +
        "\", \"burnMachineHeating\": \""+ burnMachineHeating +"\"}";
    return controllerResponse;
}