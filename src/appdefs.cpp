#include "appdefs.h"

// Time conversion from seconds to hour, minutes and seconds
void timesplit(unsigned int time_s, int &hours, int &minutes, int &seconds)
{
    hours = time_s / 3600;
    time_s = time_s % 3600;

    minutes = time_s / 60;
    seconds = time_s % 60;
}

