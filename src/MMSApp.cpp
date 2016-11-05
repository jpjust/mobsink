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
        frame->StartCmdlineSim(this->netfile, this->csvfile, this->n_sinks, this->algorithm);

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
    bool sinks = parser.Found(wxT("s"), &this->n_sinks);

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
