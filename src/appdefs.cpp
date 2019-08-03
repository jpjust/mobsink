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

#include "appdefs.h"

// Time conversion from seconds to hour, minutes and seconds
void timesplit(unsigned int time_s, int &hours, int &minutes, int &seconds)
{
	hours = time_s / 3600;
	time_s = time_s % 3600;

	minutes = time_s / 60;
	seconds = time_s % 60;
}

// Get resources directory
wxString GetResDir(void)
{
#ifdef __WXMSW__	// Windows: return current executable dir
	return APP_DIR;
#else				// Linux: search for resource dir in prefix, otherwise, return executable dir
	wxDir resdir(wxStandardPaths::Get().GetResourcesDir());
	if (resdir.IsOpened())
		return resdir.GetName();
	else
		return APP_DIR;
#endif
}
