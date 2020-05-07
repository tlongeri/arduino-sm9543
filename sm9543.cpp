/**
 * A note on datasheet issues:
 * 
 * - Page 3 says the following: 
 *   
 *   > The SM9543 has four I2C read commands: Read_DF2, Read_DF3 and Read_DF4.
 * 
 *   As far as I can tell there are only three commands (i.e. there is no unlisted command).
 * 
 * Document is marked "Doc# 40DS9543.02", I hope that can be used as a reference to the version.
 */
#include <assert.h>
#include <stdint.h>

#include <Wire.h>

#include "sm9543.h"

/* I2C address */
const uint8_t sm9543_address = 0x28;

const uint16_t pressure_bot_reading = 1638;
const uint16_t pressure_top_reading = 14745;
const double pressure_bot_pressure_mpa = -5.0;
const double pressure_top_pressure_mpa = 5.0;

static double mapLongToDouble(long x, long in_min, long in_max, double out_min, double out_max)
{
    if (x <= in_min) return out_min;
    if (in_max <= x) return out_max;
    
    double xx = (static_cast<double>(x) - in_min) / (static_cast<double>(in_max) - in_min);
    return out_min + (out_max - out_min) * xx;
}

double Sm9543::pressureReadingToMpa(uint16_t reading)
{
    return mapLongToDouble(reading, pressure_bot_reading, pressure_top_reading,
        pressure_bot_pressure_mpa, pressure_top_pressure_mpa);
}

double Sm9543::tempReadingToCelsius(uint16_t reading)
{
    /* TODO */
    return -1;
}

Sm9543Data Sm9543::readDf(int n)
{
    assert(2 <= n && n <= 4);

    assert(wire.available() == 0);
    uint8_t nreceived = wire.requestFrom(sm9543_address, 3);
    assert(nreceived == 3);

    uint8_t rx[4]; /* I would prefer an n-sized VLA here but it is sadly not standard C++ */
    for (int i = 0; i < n; ++i) rx[i] = wire.read();

    Sm9543Data ret;
    ret.status = static_cast<Sm9543Status>(rx[0] >> 6);
    ret.pressure = ((rx[0] & 0x3F) << 8) + rx[1];
    ret.temp = 0;
    if (n >= 3) ret.temp += rx[2] << 3;
    if (n >= 4) ret.temp += rx[3] >> 5;

    return ret;
}