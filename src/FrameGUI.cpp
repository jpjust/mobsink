#include "FrameGUI.h"
#include "Node.h"
#include "appdefs.h"
#include <stdlib.h>
#include <time.h>
#include <wx/utils.h>

#ifndef __WXMSW__
# include "../res/app_icon.xpm"
#endif // __WXMSW__
#include "../res/tb_sensor.xpm"
#include "../res/tb_path.xpm"
#include "../res/tb_obstacle.xpm"
#include "../res/tb_grid.xpm"
#include "../res/tb_sensor_rand.xpm"
#include "../res/tb_random.xpm"
#include "../res/tb_rl.xpm"
#include "../res/tb_coord.xpm"
#include "../res/tb_links.xpm"
#include "../res/tb_batt.xpm"
#include "../res/tb_animate.xpm"
#include "../res/tb_new.xpm"
#include "../res/tb_run.xpm"
#include "../res/tb_about.xpm"
#include "../res/tb_help.xpm"

// Events table
BEGIN_EVENT_TABLE(FrameGUI, wxFrame)
    EVT_BUTTON(ID_BTN_RUN, FrameGUI::Run)
    EVT_BUTTON(ID_BTN_SAVECSV, FrameGUI::SaveCSV)
    EVT_BUTTON(ID_BTN_LOADXML, FrameGUI::LoadXML)
    EVT_BUTTON(ID_BTN_SAVEXML, FrameGUI::SaveXML)
    EVT_BUTTON(ID_BTN_SAVEPNG, FrameGUI::SavePNG)
    EVT_TOOL_RANGE(ID_TBAR_SENSOR, ID_TBAR_HELP, FrameGUI::OnToolBarClick)
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

    btnRun = new wxButton(tbar2, ID_BTN_RUN, wxT("Insert sinks"));

    tbar1->AddTool(ID_TBAR_SENSOR,   wxT("Sensor"),   wxBitmap(tb_sensor_xpm), wxT("Sensor adding tool"),   wxITEM_RADIO);
    tbar1->AddTool(ID_TBAR_PATH,     wxT("Path"),     wxBitmap(tb_path_xpm), wxT("Path adding tool"),     wxITEM_RADIO);
    tbar1->AddTool(ID_TBAR_OBSTACLE, wxT("Obstacle"), wxBitmap(tb_obstacle_xpm), wxT("Obstacle adding tool"), wxITEM_RADIO);
    tbar1->AddSeparator();
    tbar1->AddControl(lbSensors);
    tbar1->AddControl(txtNodes);
    tbar1->AddTool(ID_TBAR_GRID,     wxT("Grid insert"),   wxBitmap(tb_grid_xpm), wxT("Insert sensors in a grid manner"));
    tbar1->AddTool(ID_TBAR_RANDOM,   wxT("Random insert"), wxBitmap(tb_sensor_rand_xpm), wxT("Insert sensors randomly"));
    tbar1->AddControl(lbRL);
    tbar1->AddControl(txtRL);
    tbar1->AddTool(ID_TBAR_RANDRL,   wxT("Random RL"),     wxBitmap(tb_random_xpm), wxT("Insert sensors with random RLs"), wxITEM_CHECK);
    tbar1->AddSeparator();
    tbar1->AddTool(ID_TBAR_RL,       wxT("Show RL"),          wxBitmap(tb_rl_xpm), wxT("Display sensors RL"),     wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_COORD,    wxT("Show coordinates"), wxBitmap(tb_coord_xpm), wxT("Display coordinates"),    wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_LINES,    wxT("Show links"),       wxBitmap(tb_links_xpm), wxT("Display links"),          wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_ENERGY,   wxT("Show energy"),      wxBitmap(tb_batt_xpm), wxT("Display energy bars"),    wxITEM_CHECK);
    tbar1->AddTool(ID_TBAR_ANIMATE,  wxT("Enable animation"), wxBitmap(tb_animate_xpm), wxT("Enable sinks animation"), wxITEM_CHECK);
    tbar1->AddSeparator();
    tbar1->AddTool(ID_TBAR_CLEAR,    wxT("Clear"), wxBitmap(tb_new_xpm), wxT("Clear WSN"));
    tbar1->AddControl(lbTime);
    tbar1->AddControl(txtTime);
    tbar1->AddTool(ID_TBAR_SIM,      wxT("Start"), wxBitmap(tb_run_xpm), wxT("Start simulation"));
    tbar1->AddSeparator();
    tbar1->AddTool(ID_TBAR_HELP,     wxT("Help"), wxBitmap(tb_help_xpm), wxT("Go to Multi Mobile Sink Simulator website"));
    tbar1->AddTool(ID_TBAR_ABOUT,    wxT("About"), wxBitmap(tb_about_xpm), wxT("About this software"));
    tbar1->Realize();

    tbar2->AddControl(lbRange);
    tbar2->AddControl(txtRange);
    tbar2->AddSeparator();
    tbar2->AddControl(lbSinks);
    tbar2->AddControl(txtSinks);
    tbar2->AddSeparator();
    tbar2->AddControl(lbInit);
    tbar2->AddControl(comboInit);
    tbar2->AddSeparator();
    tbar2->AddControl(btnRun);
    tbar2->Realize();

    // Create other widgets
    txtOutput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 0), wxTE_MULTILINE | wxTE_READONLY);

    btnLoadXML = new wxButton(this, ID_BTN_LOADXML, wxT("Load network"));
    btnSaveXML = new wxButton(this, ID_BTN_SAVEXML, wxT("Save network"));
    btnSaveCSV = new wxButton(this, ID_BTN_SAVECSV, wxT("Save results"));
    btnSavePNG = new wxButton(this, ID_BTN_SAVEPNG, wxT("Save PNG"));

    pnNet = new PanelNetwork(this);

    // Create sizers
    wxBoxSizer *sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer_sim = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizer_output = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizer_output_tbar1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizer_output_tbar2 = new wxBoxSizer(wxHORIZONTAL);

     // Output toolbar 1
    sizer_output_tbar1->Add(btnLoadXML, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    sizer_output_tbar1->Add(btnSaveXML, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

    // Output toolbar 2
    sizer_output_tbar2->Add(btnSavePNG, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    sizer_output_tbar2->Add(btnSaveCSV, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

    // Output panel
    sizer_output->Add(txtOutput, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    sizer_output->Add(sizer_output_tbar1, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
    sizer_output->Add(sizer_output_tbar2, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

    // Simulation area
    sizer_sim->Add(pnNet, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    sizer_sim->Add(sizer_output, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);

    // Main sizer
    sizer_main->Add(tbar1, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
    sizer_main->Add(tbar2, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
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
    int max_x = pnNet->GetWidth();
    int max_y = pnNet->GetHeight();

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
    pnNet->RunMMS(atoi(txtSinks->GetValue().char_str()), comboInit->GetSelection(), false);
    pnNet->RunSim(comboInit->GetSelection(), atoi(txtTime->GetValue().char_str()));
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
                     wxT("NetMedia, State University of Feira de Santana, BA, Brazil\n\n") + \
                     wxT("http://netmedia.uefs.br/\n\n") + \
                     wxT("http://just.pro.br/blog/mobsink/\n\n") + \
                     wxT("Toolbar icons made by Freepik from www.flaticon.com"),
                     APP_NAME,
                     wxICON_INFORMATION,
                     this);
        break;

    case ID_TBAR_HELP:
        if (!wxLaunchDefaultBrowser(wxT("http://just.pro.br/blog/multimobilesink/")))
            wxMessageBox(wxString(wxT("Couldn't run your default web browser. To get help, visit:\n\n")) + \
                         wxT("http://just.pro.br/blog/multimobilesink/"),
                         APP_NAME,
                         wxICON_INFORMATION,
                         this);
        break;

    default:
        break;
    }
}
