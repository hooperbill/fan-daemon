/* Jetson Nano Fan Control Daemon

 * MIT License
 *
 * Copyright (c) 2019 William Hooper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "fan-daemon.h"

using namespace std;

static unsigned pwmCap;

/**
* Exit handler. Shutoff the fan before leaving
*/
void exit_handler(int s)
{
    writeIntSysFs("/sys/devices/pwm-fan/target_pwm", pwmCap);
    exit(1);
}

/**
* Initialize the exit handler
*/
void init_exit_handler()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGTERM, &sigIntHandler, NULL);
}


/**
* Main
*/
int main(int argc, char *argv[])
{
    unsigned temp;
    unsigned pwmValue;

    pwmCap = getPwmCap();

    init_exit_handler();

    system("/usr/bin/jetson_clocks");
    writeIntSysFs("/sys/devices/pwm-fan/temp_control", 0);

    while (true)
    {
        temp = readAverageTemp();
        pwmValue = adjustFanSpeed( temp,  pwmCap);
        writeIntSysFs("/sys/devices/pwm-fan/target_pwm", pwmValue);
        this_thread::sleep_for(chrono::milliseconds(UPDATE_INTERVAL * MICRO_SECONDS));
    }

    return 0;
}

/**
* Read the average temperature. The function reads all thermal zones and returns the average.
*
*/
unsigned readAverageTemp()
{
    unsigned averageTemp;
    glob_t globResult;

    averageTemp = 0;
    glob("/sys/devices/virtual/thermal/thermal_zone*/temp", GLOB_TILDE, NULL, &globResult);
    for(unsigned i = 0; i < globResult.gl_pathc; ++i)
    {
        averageTemp += readIntSysFs(globResult.gl_pathv[i]);
    }

    return (averageTemp / globResult.gl_pathc) / 1000;
}

/**
* Get the PWM cap. This is maximum value that the fan PWM channel can support
*/
unsigned getPwmCap()
{
    return readIntSysFs("/sys/devices/pwm-fan/pwm_cap");
}

/**
* Return the adjusted PWM fan speed. Calculated from the provided temperature and the
* fan PWM cap value.
*/
unsigned adjustFanSpeed(unsigned temp, unsigned pwmCap)
{
    unsigned speed = pwmCap * (temp - FAN_OFF_TEMP) / (FAN_MAX_TEMP - FAN_OFF_TEMP);

    return min((unsigned)max((unsigned)0, speed), pwmCap);
}

/**
* Read an unsigned integer value from a sysfs path
*/
unsigned readIntSysFs(string path)
{
    unsigned value;

    ifstream infs(path);
    infs >> value;
    infs.close();

    return value;
}

/**
* Write an unsigned integer value to a sysfs path
*/
void writeIntSysFs(string path, unsigned value)
{
    ofstream outfs(path);
    outfs << value;
    outfs.close();
}

