#ifndef FRAMEGUI_H
#define FRAMEGUI_H

#include <wx/wx.h>
#include "PanelNetwork.h"

// IDs
enum
{
    ID_BTN_RUN,
    ID_BTN_SAVECSV,
    ID_BTN_LOADXML,
    ID_BTN_SAVEXML,
    ID_BTN_SAVEPNG,
    ID_TBAR_SENSOR,
    ID_TBAR_PATH,
    ID_TBAR_OBSTACLE,
    ID_TBAR_GRID,
    ID_TBAR_RANDOM,
    ID_TBAR_CLEAR,
    ID_TBAR_SIM,
    ID_TBAR_RL,
    ID_TBAR_RANDRL,
    ID_TBAR_COORD,
    ID_TBAR_LINES,
    ID_TBAR_ENERGY,
    ID_TBAR_ANIMATE,
    ID_TBAR_ABOUT,
    ID_TBAR_HELP,
};

class FrameGUI: public wxFrame
{
public:
    FrameGUI();
    int GetRL(void);
    float GetRange(void);
    void SetRange(float range);
    void SetTime(int time);
    void PrintOutput(wxString text);
    void StartCmdlineSim(wxString file_net, wxString file_out, int n_sinks, int init);

    DECLARE_EVENT_TABLE()

private:
    wxStaticText *lbSinks;
    wxStaticText *lbRange;
    wxStaticText *lbSensors;
    wxStaticText *lbRL;
    wxStaticText *lbInit;
    wxStaticText *lbTime;
    wxButton *btnRun;
    wxButton *btnSaveCSV;
    wxButton *btnLoadXML;
    wxButton *btnSaveXML;
    wxButton *btnSavePNG;
    wxTextCtrl *txtNodes;
    wxTextCtrl *txtSinks;
    wxTextCtrl *txtRange;
    wxTextCtrl *txtRL;
    wxTextCtrl *txtTime;
    wxTextCtrl *txtOutput;
    wxComboBox *comboInit;
    PanelNetwork *pnNet;

    // File dialogs paths
    wxString pathCSV;
    wxString pathXML;
    wxString pathPNG;

    wxToolBar *tbar1;
    wxToolBar *tbar2;

    void UpdateOpts(wxCommandEvent &event);
    void RandomRLClick(wxCommandEvent &event);
    void Clear(wxCommandEvent &event);
    void Run(wxCommandEvent &event);
    void RandomSensors(wxCommandEvent &event);
    void GridSensors(wxCommandEvent &event);
    void GridSinks(void);
    void Simulate(wxCommandEvent &event);
    void SaveCSV(wxCommandEvent &event);
    void LoadXML(wxCommandEvent &event);
    void SaveXML(wxCommandEvent &event);
    void SavePNG(wxCommandEvent &event);
    void OnToolBarClick(wxCommandEvent &event);
    vector<Node *> GridNodes(int n, int RL, int width, int height);
};

#endif // FRAMEGUI_H
