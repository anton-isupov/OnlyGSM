
#include "GSM.h"

GSM::GSM(uint8_t Rx, uint8_t Tx)
{
  this->setRxTx(Rx, Tx);
  this->serialInterface = new SoftwareSerial(Rx, Tx);
}

void GSM::setRxTx(uint8_t Rx, uint8_t Tx)
{
  this->Rx = Rx;
  this->Tx = Tx;
}

void GSM::setup()
{
  this->serialInterface->begin(9600);
  pinMode(7, OUTPUT);
  delay(100);
  digitalWrite(7, HIGH);
}

bool GSM::onReady()
{
  bool val = false;
  String response;
  response = sendATCommand("AT+CMGF=1;&W");
  while (response.indexOf("OK") < 0)
  {
    response = sendATCommand("AT+CMGF=1;&W");
  }
  val = true;
  return val;
}

bool GSM::onResponse()
{
  return responseAvailable;
}

String GSM::getResponse()
{
  return resultResponse;
}

String GSM::getNumber()
{
  return number;
}

void GSM::run()
{
  responseAvailable = false;
  resultResponse = "";
  number = "";
  if (this->serialInterface->available())
  {
    String response = waitResponse();
    response.trim();
    if (response.startsWith("+CIEV: \"MESSAGE\""))
    {
      responseAvailable = true;
      int phoneIndex = response.indexOf("+CMT:");
      response = response.substring(phoneIndex + 5, response.length());
      String phoneRow = response.substring(response.indexOf("+"), response.indexOf("+") + 12);
      phoneRow.trim();
      number = phoneRow;
      int index = response.lastIndexOf("\"");
      resultResponse = response.substring(index + 1, response.length());
      resultResponse.trim();
      sendATCommand("AT+CMGD=4");
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