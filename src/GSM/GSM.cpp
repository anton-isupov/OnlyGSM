
#include "GSM.h"

GSM::GSM(uint8_t Rx, uint8_t Tx) {
  this->setRxTx(Rx, Tx);
  this->serialInterface = new SoftwareSerial(Rx, Tx);
}

void GSM::setRxTx(uint8_t Rx, uint8_t Tx) {
  this->Rx = Rx;
  this->Tx = Tx;
}

void GSM::setup()
{
  this->serialInterface->begin(38400);
}

bool GSM::onReady() {
  bool val = false;
  String response;
  response = sendATCommand("AT+CMGF=1;&W");
  while(response.indexOf("OK") < 0) {
     response = sendATCommand("AT+CMGF=1;&W"); 
     Serial.println(response);    
  }
  val = true;
  return val;
}

bool GSM::onResponse() {
  bool val = false;
  if (this->serialInterface->available()) {
    val = true;
  }
  return val;
}

String GSM::getResponse() {
  return resultResponse;
}

String GSM::getNumber() {
  return number;
}

void GSM::run()
{
  if (this->serialInterface->available())
  {
    String response = waitResponse();
    response.trim();
    if (response.indexOf("+CM") > -1) {
    number = response.substring(response.indexOf("+7"), response.indexOf("+7") + 12);
    }

    if (response.startsWith("+CIEV:"))
    {

      int index = response.lastIndexOf("\"");
      resultResponse = response.substring(index + 1, response.length());
      resultResponse.trim();
      // if (result.equals("testred")) digitalWrite(8,!digitalRead(8)); Делаем что-то в зависимости от сообщения

      sendATCommand("AT+CMGD=4"); // Удалить все сообщения
    }
  }
}
String GSM::sendATCommand(String value)
{
  String resp = "";
  this->serialInterface->println(value);
  resp = waitResponse();
  if (resp.startsWith(value))
  {
    resp = resp.substring(resp.indexOf("\r", value.length()) + 2);
  }

  return resp;
}

String GSM::waitResponse()
{
  String resp = "";
  unsigned long timeout = millis() + 10000;
  while (!this->serialInterface->available() && millis() < timeout)
  {
  };
  if (this->serialInterface->available())
  {
    resp = this->serialInterface->readString();
  }
  return resp;
}
void GSM::sendSMS(String phone, String message)
{
  //String balance = this->sendATCommand("ATD#100#");
  this->sendATCommand("AT+CMGS=\"" + phone + "\"");
  this->sendATCommand(message + "\r\n" + (String)((char)26));
}


GSM::~GSM() {
  Serial.print("Bye bye");
}