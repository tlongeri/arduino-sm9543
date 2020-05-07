/**
 * A small driver to use SMI's SM9543 sensor.
 */

#include <Wire.h>

enum Sm9543Status {
    SM9543_STATUS_GOOD = 0,
    SM9543_STATUS_COMMAND_MODE = 1,
    SM9543_STATUS_STALE = 2,
    SM9543_STATUS_ERROR = 3
};

struct Sm9543Data {
    Sm9543Status status;
    uint16_t pressure;
    uint16_t temp;
};

class Sm9543 {
private:
    TwoWire& wire;
    Sm9543Data readDf(int n);

public:
    static double pressureReadingToMpa(uint16_t reading);
    static double tempReadingToCelsius(uint16_t reading);

    Sm9543(TwoWire& wire) : wire(wire) {};
    Sm9543Data readDf2() { return readDf(2); };
    Sm9543Data readDf3() { return readDf(3); };
    Sm9543Data readDf4() { return readDf(4); };
};