#ifndef GSM_H
#define GSM_H

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "../common/ExtendedHardwareUnit.h"


class GSM : public ExtendedHardwareUnit {
    public:
        GSM(uint8_t Rx, uint8_t Tx);   
        void setup();
        void run();
        void sendSMS(String phone, String message);
        bool onResponse();
        bool onReady();
        String getResponse();
        String getNumber();
    private:
        bool responseAvailable;
        String resultResponse;
        String number;
        String sendATCommand(String value);
        String waitResponse();
        void setRxTx(uint8_t Rx, uint8_t Tx);
        uint8_t Rx, Tx;
        SoftwareSerial *serialInterface;

};

#endif