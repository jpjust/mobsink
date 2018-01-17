/*
 * MobSink Frame GUI.
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

#include "FrameGUI.h"
#include "Node.h"
#include "appdefs.h"
#include <stdlib.h>
#include <time.h>
#include <wx/utils.h>

#ifndef __WXMSW__
# include "../res/app_icon.xpm"
#endif // __WXMSW__

// Events table
BEGIN_EVENT_TABLE(FrameGUI, wxFrame)
    EVT_TOOL_RANGE(ID_TBAR_SENSOR, ID_TBAR_SAVEPNG, FrameGUI::OnToolBarClick)
END_EVENT_TABLE()

// Constructor
FrameGUI::FrameGUI()
    :wxFrame(NULL, wxID_ANY, APP_FULL_NAME)
{
    // Initialize default paths
    pathCSV = wxEmptyString;
    pathXML = wxEmptyString;
    pathPNG = wxEmptyString;

    // Sets the toolbars
    tbar1 = new wxToolBar(this, wxID_ANY);
    tbar2 = new wxToolBar(this, wxID_ANY);

    tbar1->SetWindowStyleFlag(wxTB_FLAT);
    tbar2->SetWindowStyleFlag(wxTB_FLAT);

    lbSensors = new wxStaticText(tbar1, wxID_ANY, wxT("Sensors:"));
    lbRL = new wxStaticText(tbar1, wxID_ANY, wxT("RL:"));
    lbTime = new wxStaticText(tbar1, wxID_ANY, wxT("Time (s):"));
    lbSinks = new wxStaticText(tbar2, wxID_ANY, wxT("Sinks:"));
    lbRange = new wxStaticText(tbar2, wxID_ANY, wxT("Tx range (px):"));
    lbInit = new wxStaticText(tbar2, wxID_ANY, wxT("Sink positioning:"));

    txtNodes = new wxTextCtrl(tbar1, wxID_ANY);
    txtRL = new wxTextCtrl(tbar1, wxID_ANY, wxT("1"));
    txtTime = new wxTextCtrl(tbar1, wxID_ANY, wxT("240"));
    txtSinks = new wxTextCtrl(tbar2, wxID_ANY, wxT("1"));
    txtRange = new wxTextCtrl(tbar2, wxID_ANY, wxT("100"));

    comboInit = new wxComboBox(tbar2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_READONLY);
    comboInit->Append(wxT("Static sinks"));
    comboInit->Append(wxT("Fixed move (vertical)"));
    comboInit->Append(wxT("Fixed move (horizontal)"));
    comboInit->Append(wxT("Relevance based (Forgy)"));
    comboInit->Append(wxT("Relevance based (Random)"));
    comboInit->Append(wxT("Relevance based (Just)"));

    tbar1->AddTool(ID_TBAR_SENSOR,   wxT("Sensor"),   wxBitmap(wxT("png/tb_sensor.png"), wxBITMAP_TYPE_PNG),   wxT("Sensor adding tool"),   wxITEM_RADIO);
    tbar1->AddTool(ID_TBAR_PATH,     wxT("Path"),     wxBitmap(wxT("png/tb_path.png"), wxBITMAP_TYPE_PNG),     wxT("Path adding tool"),     wxITEM_RADIO);
    tbar1->AddTool(ID_TBAR_OBSTACLE, wxT("Obstacle"), wxBitmap(wxT("png/tb_obstacle.png"), wxBITMAP_TYPE_PNG), wxT("Obstacle adding tool"), wxITEM_RADIO);
    tbar1->AddSeparator();
    tbar1->AddControl(lbSensors);
    tbar1->AddControl(txtNodes);
    tbar1->AddTool(ID_TBAR_GRID,     wxT("Grid insert"),   wxBitmap(wxT("png/tb_grid.png"),        wxBITMAP_TYPE_PNG), wxT("Insert sensors in a grid manner"));
    tbar1->AddTool(ID_TBAR_RANDOM,   wxT("Random insert"), wxBitmap(wxT("png/tb_sensor_rand.png"), wxBITMAP_TYPE_PNG), wxT("Insert sensors randomly"));
    tbar1->AddControl(lbRL);
    tbar1->AddControl(txtRL);
    tbar1->AddTool(ID_TBAR_RANDRL,   wxT("Random RL"), wxBitmap(wxT("png/tb_random.png"), wxBITMAP_TYPE_PNG), wxT("Insert sensors with random RLs"), wxITEM_CHECK);
    tbar1->AddSeparator();
    tbar1->AddTool(ID_TBAR_RL,       wxT("Show RL"),          wxBitmap(wxT("png/tb_rl.png"),      wxBITMAP_TYPE_PNG), wxT("Display sensors RL"),     wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_COORD,    wxT("Show coordinates"), wxBitmap(wxT("png/tb_coord.png"),   wxBITMAP_TYPE_PNG), wxT("Display coordinates"),    wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_LINES,    wxT("Show links"),       wxBitmap(wxT("png/tb_links.png"),   wxBITMAP_TYPE_PNG), wxT("Display links"),          wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_ENERGY,   wxT("Show energy"),      wxBitmap(wxT("png/tb_batt.png"),    wxBITMAP_TYPE_PNG), wxT("Display energy bars"),    wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_ANIMATE,  wxT("Enable animation"), wxBitmap(wxT("png/tb_animate.png"), wxBITMAP_TYPE_PNG), wxT("Enable sinks animation"), wxITEM_CHECK);
    tbar1->AddSeparator();
    tbar1->AddControl(lbTime);
    tbar1->AddControl(txtTime);
    tbar1->AddTool(ID_TBAR_SIM,      wxT("Start"), wxBitmap(wxT("png/tb_run.png"), wxBITMAP_TYPE_PNG), wxT("Start simulation"));
    tbar1->AddSeparator();
    tbar1->AddTool(ID_TBAR_HELP,     wxT("Help"),  wxBitmap(wxT("png/tb_help.png"),  wxBITMAP_TYPE_PNG), wxT("Go to MobSink website"));
    tbar1->AddTool(ID_TBAR_ABOUT,    wxT("About"), wxBitmap(wxT("png/tb_about.png"), wxBITMAP_TYPE_PNG), wxT("About this software"));
    tbar1->Realize();

    tbar2->AddTool(ID_TBAR_CLEAR,    wxT("Clear"), wxBitmap(wxT("png/tb_new.png"), wxBITMAP_TYPE_PNG), wxT("Clear WSN"));
    tbar2->AddTool(ID_TBAR_LOADXML, wxT("Load WSN"), wxBitmap(wxT("png/tb_open_map.png"), wxBITMAP_TYPE_PNG), wxT("Load a XML file of a saved WSN"));
    tbar2->AddTool(ID_TBAR_SAVEXML, wxT("Save WSN"), wxBitmap(wxT("png/tb_save_map.png"), wxBITMAP_TYPE_PNG), wxT("Save a XML file containing the current WSN"));
    tbar2->AddSeparator();
    tbar2->AddTool(ID_TBAR_SAVEPNG, wxT("Save map"),    wxBitmap(wxT("png/tb_save_png.png"), wxBITMAP_TYPE_PNG), wxT("Save the current map in PNG format"));
    tbar2->AddTool(ID_TBAR_SAVECSV, wxT("Save report"), wxBitmap(wxT("png/tb_save_csv.png"), wxBITMAP_TYPE_PNG), wxT("Save a report for the last simulation in CSV format"));
    tbar2->AddSeparator();
    tbar2->AddControl(lbRange);
    tbar2->AddControl(txtRange);
    tbar2->AddSeparator();
    tbar2->AddControl(lbSinks);
    tbar2->AddControl(txtSinks);
    tbar2->AddSeparator();
    tbar2->AddControl(lbInit);
    tbar2->AddControl(comboInit);
    tbar2->AddSeparator();
    tbar2->AddTool(ID_TBAR_RUN, wxT("Insert sink(s)"), wxBitmap(wxT("png/tb_sink.png"), wxBITMAP_TYPE_PNG), wxT("Insert sink(s) in WSN"));
    tbar2->Realize();

    // Create other widgets
    txtOutput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 0), wxTE_MULTILINE | wxTE_READONLY);

    pnNet = new PanelNetwork(this);

    // Create sizers
    wxBoxSizer *sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer_sim = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizer_output = new wxBoxSizer(wxVERTICAL);

    // Output panel
    sizer_output->Add(txtOutput, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

    // Simulation area
    sizer_sim->Add(pnNet, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    sizer_sim->Add(sizer_output, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

    // Main sizer
    sizer_main->Add(tbar2, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
    sizer_main->Add(tbar1, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
    sizer_main->Add(sizer_sim, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

    SetSizerAndFit(sizer_main);
    SetAutoLayout(true);
    SetSize(800, 600);

#ifndef __WXMSW__
    SetIcon(wxIcon(app_icon_xpm));
#else
    SetIcon(wxIcon("aaaaaa"));
#endif
}

// Return the RL in the textbox
int FrameGUI::GetRL(void)
{
    int RL = atoi(txtRL->GetValue().char_str());

    if (tbar1->GetToolState(ID_TBAR_RANDRL))
        txtRL->SetValue(wxString::Format(wxT("%d"), rand() % 15));

    return RL;
}

// Return the Tx Range in the textbox
float FrameGUI::GetRange(void)
{
    return atoi(txtRange->GetValue().char_str());
}

// Change Tx Range in the textbox
void FrameGUI::SetRange(float range)
{
    txtRange->SetValue(wxString::Format(wxT("%.0f"), range));
}

// Change simulation time in the textbox
void FrameGUI::SetTime(int time)
{
    txtTime->SetValue(wxString::Format(wxT("%d"), time));
}

// Print text to the output control
void FrameGUI::PrintOutput(wxString text)
{
    txtOutput->AppendText(text);
    txtOutput->Refresh();
}

// Clear the map
void FrameGUI::Clear(wxCommandEvent &event)
{
	// Get current map size
	int width, height;
	pnNet->GetVirtualSize(&width, &height);

	// Ask for a new map size
	wxString map_size = wxString::Format(wxT("%dx%d"), width, height);
	map_size = wxGetTextFromUser(wxT("Enter new map size:"), APP_NAME, map_size);

	// Check new map size
	sscanf(map_size.c_str(), "%dx%d", &width, &height);

	if ((width <= 0) || (height <= 0))
	{
		wxMessageBox(wxT("Invalid map size!"), APP_NAME, wxICON_ERROR);
		return;
	}

	pnNet->SetVirtualSize(width, height);
    pnNet->Clear();
}

// Run button
void FrameGUI::Run(wxCommandEvent &event)
{
    if (comboInit->GetSelection() == wxNOT_FOUND)
    {
        wxMessageBox(wxT("You need to specify an initialization method."), APP_NAME, wxICON_ERROR);
        return;
    }

    // Run simulation
    pnNet->RunMMS(atoi(txtSinks->GetValue().char_str()), comboInit->GetSelection(), false);
}

// Fill the map with random sensors
void FrameGUI::RandomSensors(wxCommandEvent &event)
{
    // Number of sensors
    int n_nodes = atoi(txtNodes->GetValue().char_str());
    if (n_nodes < 1)
    {
        wxMessageBox(wxT("You need to specify how many sensors will be inserted."), APP_NAME, wxICON_ERROR);
        return;
    }

    // Randomly insert nodes
    for (int i = 0; i < n_nodes; i++)
    {
        Point p = pnNet->GetRandomPosition();
        Node *n = new Node(p.GetX(), p.GetY(), GetRL());
        pnNet->InsertNode(n);
    }

    pnNet->PaintNow();
}

// Fill the map with sensors forming a grid
void FrameGUI::GridSensors(wxCommandEvent &event)
{
    // Number of sensors
    unsigned int n_nodes = atoi(txtNodes->GetValue().char_str());
    if (n_nodes < 1)
    {
        wxMessageBox(wxT("You need to specify how many sensors will be inserted."), APP_NAME, wxICON_ERROR);
        return;
    }

    // Height and width
    int max_x;
    int max_y;
    pnNet->GetVirtualSize(&max_x, &max_y);

    // Get the list of sensors
    vector<Node *> sensors = Network::GridNodes(n_nodes, GetRL(), max_x, max_y);

    // Insert nodes
    for (unsigned int i = 0; i < sensors.size(); i++)
        pnNet->InsertNode(sensors.at(i));

    pnNet->PaintNow();
}

// Run simulation
void FrameGUI::Simulate(wxCommandEvent &event)
{
    // Ask if traffic control will be used
    bool use_traffic = (wxMessageBox(wxT("Do you want to use traffic control for sinks movement?"), APP_NAME, wxYES_NO | wxICON_QUESTION) == wxYES);

    pnNet->RunMMS(atoi(txtSinks->GetValue().char_str()), comboInit->GetSelection(), false);
    pnNet->RunSim(comboInit->GetSelection(), atoi(txtTime->GetValue().char_str()), use_traffic);
}

// Save output data to a CSV file
void FrameGUI::SaveCSV(wxCommandEvent &event)
{
    wxString filename = wxFileSelector(wxT("Save output"), pathCSV, wxEmptyString, wxT("csv"),
                                       wxT("CSV file (*.csv)|*.csv|All files|*.*"), wxFD_SAVE, this);

    if (!filename.empty())
    {
        if (wxFileExists(filename) &&
                (wxMessageBox(wxT("File \"") + filename + wxT("\" already exists. Overwrite it?"), APP_NAME, wxYES_NO | wxICON_QUESTION) == wxNO))
            return;

        pnNet->SaveCSV(filename);
    }
}

// Load a network setup from a XML file
void FrameGUI::LoadXML(wxCommandEvent &event)
{
    wxString filename = wxFileSelector(wxT("Load network"), pathXML, wxEmptyString, wxT("xml"),
                                       wxT("XML file (*.xml)|*.xml|All files|*.*"), wxFD_OPEN, this);

    if (!filename.empty())
    {
        if (pnNet->LoadXML(filename) == false)
            wxMessageBox(wxT("There was an error trying to load \"") + filename + wxT("\"."), APP_NAME, wxICON_ERROR);
    }
}

// Save network setup to a XML file
void FrameGUI::SaveXML(wxCommandEvent &event)
{
    wxString filename = wxFileSelector(wxT("Save network"), pathXML, wxEmptyString, wxT("xml"),
                                       wxT("XML file (*.xml)|*.xml|All files|*.*"), wxFD_SAVE, this);

    if (!filename.empty())
    {
        if (wxFileExists(filename) &&
                (wxMessageBox(wxT("File \"") + filename + wxT("\" already exists. Overwrite it?"), APP_NAME, wxYES_NO | wxICON_QUESTION) == wxNO))
            return;

        if (pnNet->SaveXML(filename) == false)
            wxMessageBox(wxT("There was an error trying to save \"") + filename + wxT("\"."), APP_NAME, wxICON_ERROR);
    }
}

// Save network map to a PNG file
void FrameGUI::SavePNG(wxCommandEvent &event)
{
    wxString filename = wxFileSelector(wxT("Save network map"), pathPNG, wxEmptyString, wxT("png"),
                                       wxT("PNG file (*.png)|*.png|All files|*.*"), wxFD_SAVE, this);

    if (!filename.empty())
    {
        if (wxFileExists(filename) &&
                (wxMessageBox(wxT("File \"") + filename + wxT("\" already exists. Overwrite it?"), APP_NAME, wxYES_NO | wxICON_QUESTION) == wxNO))
            return;

        if (pnNet->SavePNG(filename) == false)
            wxMessageBox(wxT("There was an error trying to save \"") + filename + wxT("\"."), APP_NAME, wxICON_ERROR);
    }
}

// Toolbar event handler
void FrameGUI::OnToolBarClick(wxCommandEvent &event)
{
    switch(event.GetId())
    {
    case ID_TBAR_SENSOR:
        pnNet->SetTool(PANEL_TOOL_SENSOR);
        break;

    case ID_TBAR_PATH:
        pnNet->SetTool(PANEL_TOOL_PATH);
        break;

    case ID_TBAR_OBSTACLE:
        pnNet->SetTool(PANEL_TOOL_OBSTACLE);
        break;

    case ID_TBAR_GRID:
        GridSensors(event);
        break;

    case ID_TBAR_RANDOM:
        RandomSensors(event);
        break;

    case ID_TBAR_RL:
    case ID_TBAR_COORD:
    case ID_TBAR_LINES:
    case ID_TBAR_ENERGY:
    case ID_TBAR_ANIMATE:
        pnNet->SetOpts(tbar1->GetToolState(ID_TBAR_RL),
                       tbar1->GetToolState(ID_TBAR_COORD),
                       tbar1->GetToolState(ID_TBAR_LINES),
                       tbar1->GetToolState(ID_TBAR_ENERGY),
                       tbar1->GetToolState(ID_TBAR_ANIMATE));
        pnNet->PaintNow();
        break;

    case ID_TBAR_CLEAR:
        Clear(event);
        break;

    case ID_TBAR_SIM:
        Simulate(event);
        break;

    case ID_TBAR_ABOUT:
        wxMessageBox(APP_FULL_NAME + wxT("\n\n") + \
                     wxT("Developed by:\n\n") + \
                     wxT("\tJoão Paulo Just Peixoto <just1982@gmail.com>\n") + \
                     wxT("\tDaniel G. Costa <danielgcosta@uefs.br>\n\n") + \
                     wxT("LARA, State University of Feira de Santana, BA, Brazil\n\n") + \
                     wxT("http://lara.uefs.br/\n\n") + \
                     wxT("http://just.pro.br/blog/mobsink/\n\n") + \
                     wxT("Toolbar icons made by Freepik from www.flaticon.com\n\n") + \
					 wxT("This program is Free Software licensed under GPLv3."),
                     APP_NAME,
                     wxICON_INFORMATION,
                     this);
        break;

    case ID_TBAR_HELP:
        if (!wxLaunchDefaultBrowser(wxT("http://just.pro.br/blog/mobsink/")))
            wxMessageBox(wxString(wxT("Couldn't run your default web browser. To get help, visit:\n\n")) + \
                         wxT("http://just.pro.br/blog/mobsink/"),
                         APP_NAME,
                         wxICON_INFORMATION,
                         this);
        break;

    case ID_TBAR_RUN:
    	Run(event);
    	break;

    case ID_TBAR_SAVECSV:
    	SaveCSV(event);
    	break;

    case ID_TBAR_LOADXML:
    	LoadXML(event);
    	break;

    case ID_TBAR_SAVEXML:
    	SaveXML(event);
    	break;

    case ID_TBAR_SAVEPNG:
    	SavePNG(event);
    	break;

    default:
        break;
    }
}

// Start a simulation via command line arguments
void FrameGUI::StartCmdlineSim(wxString file_net, wxString file_out, int n_sinks, int init, bool use_traffic)
{
if (pnNet == NULL)
    wxPrintf(wxT("NULLLLLLLLLLLLLLLLLLLLL\n\n\n\n"));
    // Load network file
    if (pnNet->LoadXML(file_net) == false)
        wxPrintf(wxT("There was an error trying to load \"%s\". Exiting...\n"), file_net.c_str());

    // Run MMS
    wxPrintf(wxT("Running initial positioning algorithm... "));
    pnNet->RunMMS(n_sinks, init, false);
    wxPrintf(wxT("Done!\n"));

    // Run simulation
    wxPrintf(wxT("\n----- Running simulation... -----\n"));
    pnNet->RunSim(init, atoi(txtTime->GetValue().char_str()), use_traffic);
    wxPrintf(wxT("\n----- Finished simulating!  -----\n\n"));

    // Save results
    wxPrintf(wxT("Saving results to \"%s\"... "), file_out.c_str());
    if (wxFileExists(file_out))
        wxPrintf(wxT("\nWARNING: File already exists. Overwritting... "));
    pnNet->SaveCSV(file_out);
    wxPrintf(wxT("Done!\n"));

    Close();
}
