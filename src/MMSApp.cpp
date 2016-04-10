#include "MMSApp.h"
#include "FrameGUI.h"
#include <stdlib.h>

// Program initialization
bool MMSApp::OnInit()
{
    SetAppName(wxT("Mobile Multi Sink Simulator"));
    SetVendorName(wxT("NetMedia"));
    srand(time(NULL)); // Seed the rand() for future use
    wxLocale app_locale(wxLANGUAGE_ENGLISH_US);
    wxInitAllImageHandlers();

    FrameGUI *frame = new FrameGUI();
    frame->Show();

    return true;
}
