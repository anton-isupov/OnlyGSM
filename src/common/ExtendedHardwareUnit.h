#ifndef Extended_Hardware_Unit_h
#define Extended_Hardware_Unit_h

#include "HardwareUnit.h"

class ExtendedHardwareUnit : public HardwareUnit
{
  public:
    ExtendedHardwareUnit();
    ExtendedHardwareUnit(int *pinModesArray);
    ~ExtendedHardwareUnit();

    void setPinMode(int *pinModesArray);
    virtual void setup();
    virtual void run();

  protected:
    int *pinModes;
};

#endif