#ifndef APPDEFS_H_INCLUDED
#define APPDEFS_H_INCLUDED

// Application name and version
#define APP_NAME        wxT("Multi Mobile Sinks Simulator")
#define APP_VERSION     wxT("1.1")
#define APP_VENDOR      wxT("NetMedia")
#define APP_FULL_NAME   wxString::Format(wxT("%s %s %s"), APP_VENDOR, APP_NAME, APP_VERSION)

// Time conversion from seconds to hour, minutes and seconds
void timesplit(unsigned int time_s, int &hours, int &minutes, int &seconds);

#endif // APPDEFS_H_INCLUDED
