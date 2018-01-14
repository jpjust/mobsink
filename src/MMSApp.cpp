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

#include "MMSApp.h"
#include <stdlib.h>

// Program initialization
bool MMSApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    SetAppName(wxT("Mobile Multi Sink Simulator"));
    SetVendorName(wxT("NetMedia"));
    srand(time(NULL)); // Seed the rand() for future use
    wxLocale app_locale(wxLANGUAGE_ENGLISH_US);
    wxInitAllImageHandlers();

    // Instantiate the frame and start the GUI or run via command line
    frame = new FrameGUI();
    if (use_gui)
        frame->Show();
    else
        frame->StartCmdlineSim(this->netfile, this->csvfile, this->n_sinks, this->algorithm, this->use_traffic);

    return true;
}

// Command line init
void MMSApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}

// Command line parser
bool MMSApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    // Get command line arguments
    bool net = parser.Found(wxT("n"), &this->netfile);
    bool csv = parser.Found(wxT("o"), &this->csvfile);
    bool alg = parser.Found(wxT("a"), &this->algorithm);
    parser.Found(wxT("t"), &this->traffic_opt);
    bool sinks = parser.Found(wxT("s"), &this->n_sinks);

    // Set default value for traffic control parameter
    this->traffic_opt == 1 ? this->use_traffic = true : this->use_traffic = false;

    // Verify if everything is OK
    if (net && csv && alg && sinks)
    {
        // All parameters were set. Start simulation via command line.
        this->use_gui = false;
        wxPrintf(wxT("Network file: %s\n"), this->netfile.c_str());
        wxPrintf(wxT("Output file: %s\n"), this->csvfile.c_str());
        wxPrintf(wxT("Algorithm: %d\n"), this->algorithm);
        wxPrintf(wxT("Sinks: %d\n"), this->n_sinks);

        this->use_gui = false;
    }
    else if (!net && !csv && !alg && !sinks)
    {
        // No parameter was set. Just start GUI.
        this->use_gui = true;
    }
    else
    {
        // Some parameter is missing. Display an error message.
        wxPrintf(wxT("If you want to use command line, you must set all parameters! Use -h option to get help.\n"));
        return false;
    }


    return true;
}
