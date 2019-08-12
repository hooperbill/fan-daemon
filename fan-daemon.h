#ifndef FAN_CONTROL_H_INCLUDED
#define FAN_CONTROL_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <glob.h>

// the following three defines can be modified to change the daemon behaviour
#define FAN_OFF_TEMP 45
#define FAN_MAX_TEMP 60
#define UPDATE_INTERVAL 2
// end of modifiable code


#define MICRO_SECONDS 1000
#define JETSON_CLOCKS "/usr/bin/jetson_clocks"
#define THERMAL_ZONE_GLOB "/sys/devices/virtual/thermal/thermal_zone*/temp"
#define PWM_CAP "/sys/devices/pwm-fan/pwm_cap"
#define TARGET_PWM "/sys/devices/pwm-fan/target_pwm"

unsigned readAverageTemp();
unsigned adjustFanSpeed(unsigned, unsigned);
unsigned getPwmCap();
unsigned readIntSysFs(std::string);
void writeIntSysFs(std::string path, unsigned value);

#endif // FAN_CONTROL_H_INCLUDED
