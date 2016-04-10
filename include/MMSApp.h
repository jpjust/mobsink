#ifndef MMSAPP_H
#define MMSAPP_H

#include <wx/wx.h>

class MMSApp: public wxApp
{
private:
    bool OnInit();
};

IMPLEMENT_APP(MMSApp)

#endif // MMSAPP_H
