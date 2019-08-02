/*
 * Helper functions for MobSink.
 * Copyright (C) 2015-2019 João Paulo Just Peixoto <just1982@gmail.com>.
 *
 * This file is part of MobSink.
 *
 * MobSink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MobSink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MobSink.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef APPDEFS_H_INCLUDED
#define APPDEFS_H_INCLUDED

#include <wx/stdpaths.h>

// Application name and version
#ifdef DEBUG
# define APP_DEBUG		wxT(" (DEBUG)")
#else
# define APP_DEBUG		wxEmptyString
#endif

#define APP_NAME        wxT("MobSink Multiple Sinks Simulator")
#define APP_VERSION     wxT("1.6")
#define APP_VENDOR      wxT("LARA")
#define APP_FULL_NAME   wxString::Format(wxT("%s %s %s%s"), APP_VENDOR, APP_NAME, APP_VERSION, APP_DEBUG)

// Application path
#define APP_PATH	wxStandardPaths::Get().GetExecutablePath()
#define APP_DIR		wxPathOnly(APP_PATH)

// Time conversion from seconds to hour, minutes and seconds
void timesplit(unsigned int time_s, int &hours, int &minutes, int &seconds);

#endif // APPDEFS_H_INCLUDED
