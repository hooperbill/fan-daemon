#ifndef FAN_CONTROL_H_INCLUDED
#define FAN_CONTROL_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <glob.h>

#define MICRO_SECONDS 1000

// the following three defines can be modified to change the daemon behaviour
#define FAN_OFF_TEMP 20
#define FAN_MAX_TEMP 60
#define UPDATE_INTERVAL 2

unsigned readAverageTemp();
unsigned adjustFanSpeed(unsigned, unsigned);
unsigned getPwmCap();
unsigned readIntSysFs(std::string);
void writeIntSysFs(std::string path, unsigned value);

#endif // FAN_CONTROL_H_INCLUDED
