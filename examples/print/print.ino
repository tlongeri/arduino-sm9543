#include <assert.h>
#include <inttypes.h>

#include <Wire.h>

#include "sm9543.h"

/* Pass whichever TwoWire instance you want to use to communicate with the sensor (Wire, Wire1,
 * Wire2, etc. - available wires depend on your board). */
Sm9543 sm9543(Wire);

void setup()
{
    Wire.begin();
    /* If you need to change the default clock speed, do so here.
     * The SM9543 supports frequencies from 100 kHz to 400 kHz.
     * Tipically, the default initial value will be 100 kHz. */
    // Wire.setClock(100000);

    Serial.begin(9600);
}

static const char * getStatusString(Sm9543Status status)
{
  switch (status)
  {
    case SM9543_STATUS_GOOD:            return "Good";
    case SM9543_STATUS_COMMAND_MODE:    return "Command mode";
    case SM9543_STATUS_STALE:           return "Stale";
    case SM9543_STATUS_ERROR:           return "Error";
    default: assert(false);
  }
}

void loop()
{
  Sm9543Data read_data = sm9543.readDf4();
  char output[256];
  int written = snprintf(output, 256, "Status: %10s\tPressure: %8" PRIu16 " (%3.2f MPa)\tTemperature: %8" PRIu16,
    getStatusString(read_data.status), read_data.pressure, Sm9543::pressureReadingToMpa(read_data.pressure),
    read_data.temp);
  assert(0 <= written && written < 256);
  Serial.println(output);

  delay(500);
}
