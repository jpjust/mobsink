/*
 * MobSink Frame GUI.
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

#ifndef FRAMEGUI_H
#define FRAMEGUI_H

#include <wx/wx.h>
#include "PanelNetwork.h"

// IDs
enum
{
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
	ID_TBAR_RUN,
	ID_TBAR_SAVECSV,
	ID_TBAR_SAVEDAT,
	ID_TBAR_LOADXML,
	ID_TBAR_SAVEXML,
	ID_TBAR_SAVEPNG,
};

class FrameGUI: public wxFrame
{
public:
    FrameGUI();
    int GetRL(void);
    float GetRange(void);
    void SetRange(float range);
    int GetTime(void);
    void SetTime(int time);
    void PrintOutput(wxString text);
    void StartCmdlineSim(wxString file_net, wxString file_out, int n_sinks, int init, bool use_traffic);

    DECLARE_EVENT_TABLE()

private:
    wxStaticText *lbSinks;
    wxStaticText *lbRange;
    wxStaticText *lbSensors;
    wxStaticText *lbRL;
    wxStaticText *lbInit;
    wxStaticText *lbTime;
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
    wxString pathDAT;
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
    void SaveDAT(wxCommandEvent &event);
    void LoadXML(wxCommandEvent &event);
    void SaveXML(wxCommandEvent &event);
    void SavePNG(wxCommandEvent &event);
    void OnToolBarClick(wxCommandEvent &event);
    vector<Node *> GridNodes(int n, int RL, int width, int height);
};

#endif // FRAMEGUI_H
