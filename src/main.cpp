#include <Arduino.h>
#include "GSM/GSM.h"
#include "sensorTemperature/sensorTemperature.h"

const uint8_t RELE_START_HEAT_PIN = 8;
const uint8_t RELE_POWER_HEAT_PIN = 9;
const uint8_t WASH_SYSTEM_PIN = 3;

const uint8_t SOFT_RESET_PIN = 4;

const PROGMEM String availableNumbers[1] = {
    "+79503422666"
};

void doActionForEachNumber(void (*callback)(String, String), String message);
static void sendMessage(String phone, String message);
void getStatus(String requestorPhone, String tempAir, String temp);
String buildResponseMessage(String airTemperature, String burnTemperature, String burnMachinePower, String burnMachineHeating);
void(* resetFunc) (void) = 0;

GSM gsm(10,11);
sensorTemperature sensor(6);
sensorTemperature sensorAir(2);
String heatingStarted;
bool washSystemStarted;
bool burnPowerOn;
unsigned long WASH_SYSTEM_TIMEOUT = 300000;
unsigned long washSystemStartedTime = 0;
const PROGMEM String controllerPassword = "PSWDValki0602199132";
const PROGMEM String commandSign = "&CMD=";

void setup() {
    delay(200);
    Serial.begin(9600);
    sensor.setup();
    sensorAir.setup();
    pinMode(RELE_START_HEAT_PIN, OUTPUT);
    pinMode(RELE_POWER_HEAT_PIN, OUTPUT);
    pinMode(WASH_SYSTEM_PIN, OUTPUT);
    digitalWrite(WASH_SYSTEM_PIN, LOW);
    digitalWrite(RELE_POWER_HEAT_PIN, LOW);

    washSystemStarted = false;
    burnPowerOn = false;
    
    heatingStarted = "OFF";
    gsm.setup();

    if (gsm.onReady()) {
        doActionForEachNumber(sendMessage, F("Controller are ready"));
    }
}

void loop() {
    gsm.run();
    if(washSystemStarted && (millis() - washSystemStartedTime > WASH_SYSTEM_TIMEOUT)) {
        digitalWrite(WASH_SYSTEM_PIN,LOW);
        washSystemStarted = false;
    }

    if (gsm.onResponse()) {
        const String result = gsm.getResponse();
        String requestPhone = gsm.getNumber();
        if(requestPhone.length() == 0) {
            requestPhone = availableNumbers[0];
        }
        if(result.length() > 0) {
            const String temp = (String) sensor.getTemperature();
            const String tempAir = (String) sensorAir.getTemperature();

            if (result.equals(controllerPassword + commandSign + "burnPowerOn")) {
                digitalWrite(RELE_POWER_HEAT_PIN,HIGH);
                delay(500);
                burnPowerOn = true;
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "washSystemOn")) {
                digitalWrite(WASH_SYSTEM_PIN,HIGH);
                delay(500);
                washSystemStartedTime = millis();
                washSystemStarted = true;
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "washSystemOff")) {
                digitalWrite(WASH_SYSTEM_PIN,LOW);
                delay(500);
                washSystemStarted = false;
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "burnPowerOff")) {
                digitalWrite(RELE_POWER_HEAT_PIN,LOW);
                delay(500);
                heatingStarted = "OFF";
                burnPowerOn = false;
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "burnStartHeating")) {
                digitalWrite(RELE_START_HEAT_PIN,HIGH);
                delay(500);
                digitalWrite(RELE_START_HEAT_PIN, LOW);
                heatingStarted = "ON";
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "getStatus")) {
                getStatus(requestPhone, tempAir, temp);
            }
            if (result.equals(controllerPassword + commandSign + "softReset")) {
                resetFunc();
            }
        }
    }
}

void getStatus(String requestorPhone, String tempAir, String temp) {
    String powerStatus;
    if(burnPowerOn) {
        powerStatus = "ON";
    } else {
        powerStatus = "OFF";
    }
    const String response = buildResponseMessage(tempAir, temp, powerStatus, heatingStarted);
    sendMessage(requestorPhone, response);
}

void doActionForEachNumber(void (*callback)(String, String), String message) {
    for(unsigned int i=0; i<1; i++) {
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
        "\", \"washSystemPower\": \""+ (washSystemStarted ? "ON" : "OFF") +
        "\", \"burnMachineHeating\": \""+ burnMachineHeating +"\"}";
    return controllerResponse;
}