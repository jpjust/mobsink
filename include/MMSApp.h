/*
 * LARA MobSink Multiple Sinks Simulator.
 * Copyright (C) 2015-2018 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef MMSAPP_H
#define MMSAPP_H

#include <wx/wx.h>
#include <wx/cmdline.h>
#include "FrameGUI.h"

class MMSApp: public wxApp
{
private:
    bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    FrameGUI *frame;
    bool use_gui;
    bool use_traffic;
    long n_sinks;
    long algorithm;
    long traffic_opt = 0;
    wxString netfile;
    wxString csvfile;
};

// Command line arguments
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
    {
        wxCMD_LINE_SWITCH, ("h"), ("help"), ("displays help on the command line parameters"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
    },
    { wxCMD_LINE_OPTION, ("n"), ("network"), ("load network from XML file") },
    { wxCMD_LINE_OPTION, ("o"), ("output"), ("print results to output file") },
    { wxCMD_LINE_OPTION, ("s"), ("sinks"), ("how many sinks to use"), wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, ("a"), ("algorithm"), ("which positioning algorithm to use: 0=static, 1=fixed_v, 2=fixed_h, 5=just"), wxCMD_LINE_VAL_NUMBER  },
	{ wxCMD_LINE_OPTION, ("t"), ("traffic"), ("use traffic control: 0=no (default), 1=yes"), wxCMD_LINE_VAL_NUMBER  },

    { wxCMD_LINE_NONE }
};

IMPLEMENT_APP(MMSApp)

#endif // MMSAPP_H
