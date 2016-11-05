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
    long n_sinks;
    long algorithm;
    wxString netfile;
    wxString csvfile;
};

// Command line arguments
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
    {
        wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("displays help on the command line parameters"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
    },
    { wxCMD_LINE_OPTION, wxT("n"), wxT("network"), wxT("load network from XML file") },
    { wxCMD_LINE_OPTION, wxT("o"), wxT("output"), wxT("print results to output file") },
    { wxCMD_LINE_OPTION, wxT("s"), wxT("sinks"), wxT("how many sinks to use"), wxCMD_LINE_VAL_NUMBER },
    { wxCMD_LINE_OPTION, wxT("a"), wxT("algorithm"), wxT("which positioning algorithm to use: 0=static, 1=fixed_v, 2=fixed_h, 5=just"), wxCMD_LINE_VAL_NUMBER  },

    { wxCMD_LINE_NONE }
};

IMPLEMENT_APP(MMSApp)

#endif // MMSAPP_H
