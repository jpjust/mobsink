/*
 * MobSink network panel GUI.
 * Copyright (C) 2015-2017 João Paulo Just Peixoto <just1982@gmail.com>.
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

#include "PanelNetwork.h"
#include "FrameGUI.h"
#include "appdefs.h"
#include <deque>
#include <time.h>
#include <wx/dcbuffer.h>
#include <wx/xml/xml.h>

// Events table
BEGIN_EVENT_TABLE(PanelNetwork, wxPanel)
    EVT_ERASE_BACKGROUND(PanelNetwork::OnEraseBackGround)
    EVT_PAINT(PanelNetwork::PaintEvent)
    EVT_LEFT_DOWN(PanelNetwork::MouseClick)
    EVT_MOTION(PanelNetwork::MouseMove)
END_EVENT_TABLE()

// Constructor
PanelNetwork::PanelNetwork(wxFrame *parent)
    :wxScrolled<wxPanel>(parent)
{
	SetScrollRate(SCROLL_X, SCROLL_Y);
	SetVirtualSize(WIDTH_DEFAULT, HEIGHT_DEFAULT);
	Clear();
    showRL = false;
    showCoord = false;
    drawLines = false;
    showEnergy = false;
    tool = PANEL_TOOL_SENSOR;
    path_pa = NULL;
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetCursor(wxCursor(wxCURSOR_CROSS));
}

// Getters
float PanelNetwork::GetWidth(void)
{
    // If there is a fixed size defined by XML file, use it.
    if (fixed_size)
        return this->width;
    else
    {
        int max_x, max_y;
        GetSize(&max_x, &max_y);
        return max_x;
    }
}

float PanelNetwork::GetHeight(void)
{
    // If there is a fixed size defined by XML file, use it.
    if (fixed_size)
        return this->width;
    else
    {
        int max_x, max_y;
        GetSize(&max_x, &max_y);
        return max_y;
    }
}

// To avoid flickering
void PanelNetwork::OnEraseBackGround(wxEraseEvent& event) {};

// Set options
void PanelNetwork::SetOpts(bool showRL, bool showCoord, bool drawLines, bool showEnergy, bool animate)
{
    this->showRL = showRL;
    this->showCoord = showCoord;
    this->drawLines = drawLines;
    this->showEnergy = showEnergy;
    this->animate = animate;
}

// Clear the map
void PanelNetwork::Clear(void)
{
    fixed_size = false;
    wsn.Clear();
    PaintNow();
}

// Return a random position in the network area
Point PanelNetwork::GetRandomPosition(void)
{
    // Height and width
    int max_x, max_y;

    if (fixed_size)
    {
        max_x = width;
        max_y = height;
    }
    else
        GetSize(&max_x, &max_y);

    int x = rand() % max_x;
    int y = rand() % max_y;

    Point p(x, y);
    return p;
}

// Insert a new node
void PanelNetwork::InsertNode(Node *newnode)
{
    wsn.InsertNode(newnode);
}

// Insert a new obstacle
void PanelNetwork::InsertObstacle(Obstacle newobs)
{
    wsn.InsertObstacle(newobs);
}

// Insert a new path
void PanelNetwork::InsertPath(Path newpath)
{
    wsn.InsertPath(newpath);
}

// Insert a new sink
void PanelNetwork::InsertSink(Sink *newsink)
{
    wsn.InsertSink(newsink);
}

// Paint event
void PanelNetwork::PaintEvent(wxPaintEvent & evt)
{
    wxBufferedPaintDC dc(this);
    PrepareDC(dc);
    dc.Clear();
    Render(dc);
}

// Paint the map
void PanelNetwork::PaintNow(void)
{
    Refresh();  // Mark this panel as invalidated (needs repainting)
    FrameGUI *parent = (FrameGUI *)GetParent();
    parent->Update();   // Make everything invalidated to be repainted
}

// Get positioning method
int PanelNetwork::GetSinkPos(int init)
{
    switch (init)
    {
    case 0:
        return SINKPOS_STATIC;

    case 1:
        return SINKPOS_VERTICAL;

    case 2:
        return SINKPOS_HORIZONTAL;

    case 3:
        return SINKPOS_RLFORGY;

    case 4:
        return SINKPOS_RLRANDOM;

    case 5:
    default:
        return SINKPOS_RLJUST;
    }
}

// Run simulation
void PanelNetwork::RunSim(int init, int s_time)
{
    if (wsn.GetClusters().size() == 0)
    {
        wxMessageBox(wxT("You need to insert sinks before starting a simulation."), APP_NAME, wxICON_ERROR);
        return;
    }

    // Variables
    vector<Node *> nodes = wsn.GetNodes();
    deque<Node *> node_to_transmit;
    vector<Cluster *> clusters = wsn.GetClusters();
    int t = 1, last_t = 0, active = 0, active_before;
    unsigned int tx = 0;
    double total_pdus = 0;
    time_t time_begin, time_end;
    int time_h, time_m, time_s;
    double moved_pixels = 0;
    double energy = 0;
    bool network_changed = false;
    wxString details = wxEmptyString, moves = wxEmptyString;

    // Set a dynamic paiting rate
    int paint_rate = s_time / 10000;
    if (paint_rate == 0)
        paint_rate = 1;

    // Set a dynamic output rate
    // As a reference, for 1 day simulation, we will output at every 5 minutes.
    // This gives 288 output lines. So, let's calculate it proportionally.
    int output_rate = s_time / 288;
    if (output_rate < 1)
        output_rate = 1;

    // Get positioning method
    int sinkpos = GetSinkPos(init);

    // Before simulation, reset energy supply of all nodes
    for (unsigned int i = 0; i < nodes.size(); i++)
        nodes.at(i)->Reset();

    // Also, reset the PDUs counting of each cluster
    for (unsigned int k = 0; k < clusters.size(); k++)
        clusters.at(k)->ResetPDUs();

    // Get Tx Range
    FrameGUI *parent = (FrameGUI *)GetParent();
    float range = parent->GetRange();
    parent->Enable(false);
    wsn.SetRange(range);

    // Start output
    parent->PrintOutput(wxString::Format(wxT("Starting simulations with %lu sensors and %lu sinks until %d seconds...\n"),
                                         nodes.size(), wsn.GetClusters().size(), s_time));

    if (animate)
        parent->PrintOutput(wxString::Format(wxT("t = 0, J = 0, Active: %lu\n"), nodes.size()));

    details.Append(wxString::Format(wxT("%d;%.2f;%d\n"), t, energy, active));

    // Get time
    time_begin = time(NULL);

    // Build the network graph
    wsn.BuildGraph();

    do
    {
        // First, make every node generate data and insert it in the nodes to transmit vector
        active_before = active;
        active = 0;
        network_changed = false;
        node_to_transmit.clear();
        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            // Make the node work for 1 second
            nodes.at(i)->Work();

            // Check if this node has changed
            if (nodes.at(i)->HasChanged())
                network_changed = true;

            // If it is an active node
            if (nodes.at(i)->IsActive())
            {
                active++; // To count active nodes in each turn

                if (nodes.at(i)->HasData())
                    node_to_transmit.push_back(nodes.at(i));
            }
        }

        // After data generation, position sinks
        // Only if topology has changed (any sensor has died, or RL changed) or if using
        // fixed movement.
        if ((network_changed) || (active != active_before) || (init == SINKPOS_HORIZONTAL) || (init == SINKPOS_VERTICAL))
        {
            // Initialize means
            InitializeMeans(init);

            // Reposition sinks
            wsn.PositionSinks(true, sinkpos);
            wsn.CreateLinks();

            // This last part should be executed only if number of active nodes has changed
            if ((network_changed) || (active != active_before))
            {
                wxPrintf(wxT("\nTopology change detected at t = %d! Sink(s) movement(s):\n"), t);

                for (unsigned int k = 0; k < clusters.size(); k++)
                {
                    vector<Point> route = clusters.at(k)->GetLastPath();
                    wxString coordinates = wxEmptyString;

                    wxPrintf(wxT("Sink #%d path: "), k);
                    for (int i = route.size() - 1; i >= 0; i--)
                        coordinates.Append(wxString::Format(wxT("(%.0f, %.0f) "), route.at(i).GetX(), route.at(i).GetY()));

                    wxPrintf(coordinates + wxT("\n"));
                    moves.Append(wxString::Format(wxT("%d;%d;"), t, k) + coordinates + wxT("\n"));
                }
            }
        }

        // Then, make every node make a transmission
        for (unsigned int i = 0; i < node_to_transmit.size(); i++)
        {
            double energy_cur = node_to_transmit.at(i)->SendData();

            if (energy_cur > 0)
            {
                last_t = t;
                energy += energy_cur;
                tx++;
            }
        }

        // Output data
        if (t % output_rate == 0)
            details.Append(wxString::Format(wxT("%d;%.2f;%d\n"), t, energy, active));

        // Update window at a dynamic rate
        if ((animate) && (t % paint_rate == 0))
        {
            parent->PrintOutput(wxString::Format(wxT("t = %d, Energy = %.2f, Active: %d\n"), t, energy, active));
            PaintNow();
            wxMilliSleep(1);
        }

        // Increase time
        t++;
    }
    while ((t <= s_time));

    // Clear the network graph
    wsn.ClearGraph();

    // Get time
    time_end = time(NULL);
    timesplit((unsigned int)(time_end - time_begin), time_h, time_m, time_s);

    // Compute how many packets arrived to the sinks and how much
    // the sinks has moved
    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        total_pdus += clusters.at(k)->GetPDUs();
        moved_pixels += clusters.at(k)->GetMovedPixels();
    }

    // Summary
    parent->PrintOutput(wxT("\n\n----- Simulation finished -----\n"));
    parent->PrintOutput(wxString::Format(wxT("Time elapsed (real): %02d:%02d:%02d\n"), time_h, time_m, time_s));
    parent->PrintOutput(wxString::Format(wxT("Transmission range: %.0f m\n"), range));
    parent->PrintOutput(wxString::Format(wxT("Sinks movement: %.2f m\n"), moved_pixels));
    parent->PrintOutput(wxString::Format(wxT("Remaining sensors: %d of %lu\n"), active, nodes.size()));
    parent->PrintOutput(wxString::Format(wxT("Total energy spent: %.2f J\n"), energy));
    parent->PrintOutput(wxString::Format(wxT("Last transmission: t = %d s\n"), last_t));
    parent->PrintOutput(wxString::Format(wxT("Transmissions occurred: %u\n"), tx));
    parent->PrintOutput(wxString::Format(wxT("Packets arrived to the sinks: %.0f\n\n\n"), total_pdus));

    // CSV output
    file_output = wxT("SIMULATION RESULTS\n\n");
    file_output.Append(wxString::Format(wxT("Transmission range;%.0f\n"), range));
    file_output.Append(wxString::Format(wxT("Sinks movement;%.2f\n"), moved_pixels));
    file_output.Append(wxString::Format(wxT("Remaining sensors;%d/%lu\n"), active, nodes.size()));
    file_output.Append(wxString::Format(wxT("Total energy spent;%.2f\n"), energy));
    file_output.Append(wxString::Format(wxT("Last transmission;%d\n"), last_t));
    file_output.Append(wxString::Format(wxT("Transmissions occurred;%u\n"), tx));
    file_output.Append(wxString::Format(wxT("Packets arrived to the sinks;%.0f\n"), total_pdus));

    file_output.Append(wxT("\n\nDETAILS\nt;Energy;Active nodes\n"));
    file_output.Append(details);

    // Average of hops used for each sensor
    file_output.Append(wxT("\n\nAVERAGE HOPS PER SENSOR\n"));
    for (unsigned int i = 0; i < nodes.size(); i++)
        file_output.Append(wxString::Format(wxT("S[%d];%.2f\n"), i, nodes.at(i)->GetHopsAvg()));

    // Moves output
    moves_output = wxT("SINK(S) MOVEMENT(S)\n\n");
    moves_output.Append(wxT("t;Sink;Path\n"));
    moves_output.Append(moves);

    // Paint the final round
    PaintNow();

    parent->Enable(true);
}

// Render the network map
void PanelNetwork::Render(wxDC &dc)
{
    // Default settings
    dc.SetFont(wxFont(FONT_SIZE, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Paint canvas
    dc.FloodFill(0, 0, wxColor(255, 255, 255), wxFLOOD_BORDER);

    vector<Node *> nodes = wsn.GetNodes();
    vector<Cluster *> clusters = wsn.GetClusters();
    vector<Obstacle> obstacles = wsn.GetObstacles();
    vector<Path> paths = wsn.GetPaths();

    // Draw the obstacles (they must be drawn first to stay under all other stuff)
    dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
    dc.SetBrush(wxBrush(wxColor(255, 255, 0)));
    for (unsigned int i = 0; i < obstacles.size(); i++)
        dc.DrawCircle(wxPoint(obstacles.at(i).GetX(), obstacles.at(i).GetY()), obstacles.at(i).GetRadius());

    // Draw the paths
    dc.SetPen(wxPen(wxColor(127, 0, 0), 3));
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        // Draw lines if the node is alive and has a next hop (lines must be drawn first, so they get under the sensors)
        wxPoint points[2];
        points[0].x = (int)paths.at(i).GetPointA().GetX();
        points[0].y = (int)paths.at(i).GetPointA().GetY();
        points[1].x = (int)paths.at(i).GetPointB().GetX();
        points[1].y = (int)paths.at(i).GetPointB().GetY();
        dc.DrawLine(points[0], points[1]);

        // Draw path points coordinates
        if (showCoord)
        {
            dc.DrawText(wxString::Format(wxT("(%d, %d)"), points[0].x, points[0].y), points[0].x + COORD_X_OFFSET, points[0].y + COORD_Y_OFFSET);
            dc.DrawText(wxString::Format(wxT("(%d, %d)"), points[1].x, points[1].y), points[1].x + COORD_X_OFFSET, points[1].y + COORD_Y_OFFSET);
        }
    }

    // Draw the Dijkstra's paths
    dc.SetPen(wxPen(wxColor(255, 0, 0), 3));
    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        vector<Point> last_path = clusters.at(k)->GetLastPath();
        if (last_path.size() < 2)
            continue;

        for (unsigned int i = 0; i < last_path.size() - 1; i++)
        {
            wxPoint points[2];
            points[0].x = (int)last_path.at(i).GetX();
            points[0].y = (int)last_path.at(i).GetY();
            points[1].x = (int)last_path.at(i + 1).GetX();
            points[1].y = (int)last_path.at(i + 1).GetY();
            dc.DrawLine(points[0], points[1]);
        }
    }

    // If using path tool, draw a path when needed
    if (path_pa != NULL)
    {
        wxPoint points[2];
        points[0].x = path_pa->GetX();
        points[0].y = path_pa->GetY();
        points[1].x = mouse_point.GetX();
        points[1].y = mouse_point.GetY();
        dc.DrawLine(points[0], points[1]);
    }

    // Draw the sensors
    dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        // Set style and color
        if (nodes.at(i)->IsActive())
            dc.SetBrush(*wxBLUE_BRUSH);
        else
            dc.SetBrush(*wxWHITE_BRUSH);

        // Draw lines if the node is alive and has a next hop (lines must be drawn first, so they get under the sensors)
        if (drawLines && clusters.size() > 0 && nodes.at(i)->GetPower() > 0 && nodes.at(i)->GetNextHop() != NULL)
        {
            wxPoint points[2];
            points[0].x = (int)nodes.at(i)->GetX();
            points[0].y = (int)nodes.at(i)->GetY();
            points[1].x = (int)nodes.at(i)->GetNextHop()->GetX();
            points[1].y = (int)nodes.at(i)->GetNextHop()->GetY();
            dc.DrawLine(points[0], points[1]);
        }

        // Draw sensor
        dc.DrawCircle(wxPoint(nodes.at(i)->GetX(), nodes.at(i)->GetY()), NODE_RADIUS);

        // Draw sensor data
        if (showRL)
            dc.DrawText(wxString::Format(wxT("RL: %d"), nodes.at(i)->GetRL()), nodes.at(i)->GetX() + RL_X_OFFSET, nodes.at(i)->GetY() + RL_Y_OFFSET);
        if (showCoord)
            dc.DrawText(wxString::Format(wxT("(%.0f, %.0f)"), nodes.at(i)->GetX(), nodes.at(i)->GetY()), nodes.at(i)->GetX() + COORD_X_OFFSET, nodes.at(i)->GetY() + COORD_Y_OFFSET);
        if (showEnergy)
        {
            int e_green, e_red;
            e_green = ENERGY_SIZE * nodes.at(i)->GetPower() / POWER;
            e_red = ENERGY_SIZE - e_green;

            dc.SetBrush(*wxGREEN_BRUSH);
            dc.DrawRectangle(wxPoint(nodes.at(i)->GetX() + ENERGY_X_OFFSET, nodes.at(i)->GetY() + ENERGY_Y_OFFSET), wxSize(e_green, 4));

            dc.SetBrush(*wxRED_BRUSH);
            dc.DrawRectangle(wxPoint(nodes.at(i)->GetX() + ENERGY_X_OFFSET + e_green, nodes.at(i)->GetY() + ENERGY_Y_OFFSET), wxSize(e_red, 4));
        }
    }

    // Draw the sinks
    dc.SetBrush(*wxRED_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
    for (unsigned int i = 0; i < clusters.size(); i++)
    {
        dc.DrawRectangle(wxPoint(clusters.at(i)->GetSink()->GetX() - SINK_SIZE / 2, clusters.at(i)->GetSink()->GetY() - SINK_SIZE / 2), wxSize(SINK_SIZE, SINK_SIZE));

        // Draw sink data
        if (showCoord)
            dc.DrawText(wxString::Format(wxT("(%.0f, %.0f)"), clusters.at(i)->GetSink()->GetX(), clusters.at(i)->GetSink()->GetY()), clusters.at(i)->GetSink()->GetX() + COORD_X_OFFSET, clusters.at(i)->GetSink()->GetY() + COORD_Y_OFFSET);
    }

    // Show cursor position only if drawing a path or if coordinates checkbox is checked
    if ((path_pa != NULL) || (showCoord))
        dc.DrawText(wxString::Format(wxT("(%.0f, %.0f)"), mouse_point.GetX(), mouse_point.GetY()), mouse_point.GetX() + COORD_X_OFFSET, mouse_point.GetY() + COORD_Y_OFFSET);
}

// Saves the network diagram to a file
bool PanelNetwork::SavePNG(wxString filename)
{
    // Height and width
    int width, height;
    GetSize(&width, &height);

    // Create a wxBitmap and a wxMemoryDC
    wxBitmap bmp(width, height, 32);
    wxMemoryDC dc;

    // Draw on bitmap
    dc.SelectObject(bmp);
    Render(dc);
    dc.SelectObject(wxNullBitmap);

    // Save file
    return bmp.SaveFile(filename, wxBITMAP_TYPE_PNG);
}

// Grabs a mouse click and add a new node to the vector
void PanelNetwork::MouseClick(wxMouseEvent &event)
{
    Node *newnode = new Node();
    wxPoint offset = GetViewStart();

    switch (tool)
    {
    case PANEL_TOOL_SENSOR:
        // Add a new node to the vector
        newnode->SetX(event.GetX() + offset.x * SCROLL_X);
        newnode->SetY(event.GetY() + offset.y * SCROLL_Y);
        newnode->SetRL(((FrameGUI *)GetParent())->GetRL());

        // Find the nearest cluster and add this node into it
        newnode->SetCluster(wsn.GetNearestClusterFrom(*newnode));

        InsertNode(newnode);
        break;

    case PANEL_TOOL_PATH:
        // Insert a new point for the path
        if (path_pa == NULL)
            path_pa = new Point(event.GetX() + offset.x * SCROLL_X,
            		event.GetY() + offset.y * SCROLL_Y);
        else
        {
            Path p(*path_pa, mouse_point);
            InsertPath(p);
            delete path_pa;
            path_pa = NULL;
        }

        break;

    case PANEL_TOOL_OBSTACLE:
        // Ask the radius of the new obstacle
        float radius = atoi(wxGetTextFromUser(wxT("Type the radius of the new obstacle:"),
                                              APP_NAME,
                                              wxT("20"),
                                              this).char_str());

        if (radius < 1)
            return;

        // Add a new obstacle to the vector
        Obstacle newobs;
        newobs.SetX(event.GetX() + offset.x * SCROLL_X);
        newobs.SetY(event.GetY() + offset.y * SCROLL_Y);
        newobs.SetRadius(radius);

        InsertObstacle(newobs);
        break;
    }

    PaintNow();
}

// Grabs a mouse movement
void PanelNetwork::MouseMove(wxMouseEvent &event)
{
	wxPoint offset = GetViewStart();
    mouse_point.SetX(event.GetX() + offset.x * SCROLL_X);
    mouse_point.SetY(event.GetY() + offset.y * SCROLL_Y);

    // Help making straight lines
    if (path_pa != NULL)
    {
        int x_delta = (event.GetX() + offset.x * SCROLL_X) - path_pa->GetX();
        int y_delta = (event.GetY() + offset.y * SCROLL_Y) - path_pa->GetY();

        if ((x_delta >= -3) && (x_delta <= 3))
            mouse_point.SetX(path_pa->GetX());

        if ((y_delta >= -3) && (y_delta <= 3))
            mouse_point.SetY(path_pa->GetY());
    }

    // Only redraw if drawing a path or if coordinates checkbox is checked
    if ((path_pa != NULL) || (showCoord))
        PaintNow();
}

// Initialize means
void PanelNetwork::InitializeMeans(int init)
{
    switch(init)
    {
    case 0:
    case 1:
    case 2:
        wsn.MeansInitializeStatic();
        break;

    case 3:
        wsn.MeansInitializeForgy();
        break;

    case 4:
        wsn.MeansInitializeRandom();
        break;

    default:
        wsn.MeansInitializeJust();
    }
}

// Run the MMS algorithm
void PanelNetwork::RunMMS(int n_sinks, int init, bool use_runtime_RL)
{
    // Initialization
    vector<Node *> nodes = wsn.GetNodes();

    if (n_sinks < 1)
    {
        wxMessageBox(wxT("You need to specify how many sinks will be used."), APP_NAME, wxICON_ERROR);
        return;
    }

    if (nodes.size() < 1)
    {
        wxMessageBox(wxT("You need to insert at least one sensor to continue."), APP_NAME, wxICON_ERROR);
        return;
    }

    if (n_sinks > (int)nodes.size())
    {
        wxMessageBox(wxT("You can't use more sinks than sensors."), APP_NAME, wxICON_ERROR);
        return;
    }

    // Get Tx Range
    FrameGUI *parent = (FrameGUI *)GetParent();
    float range = parent->GetRange();
    wsn.SetRange(range);

    // If this is not a simulation, reset nodes
    for (unsigned int i = 0; i < nodes.size() && use_runtime_RL == false; i++)
        nodes.at(i)->Reset();

    // Create the clusters
    wsn.CreateClusters(n_sinks);

    // Initialize means
    InitializeMeans(init);

    // Move sinks
    wsn.PositionSinks(use_runtime_RL, GetSinkPos(init));
    wsn.CreateLinks();

    PaintNow();
}

// Save file_output to a CSV file
bool PanelNetwork::SaveCSV(wxString filename)
{
    FILE *fd = fopen(filename.char_str(), "w");

    if (fd == NULL)
    {
        wxMessageBox(wxT("Couldn't open ") + filename + wxT(" for writing."), APP_NAME, wxICON_ERROR);
        return false;
    }

    fputs(file_output.char_str(), fd);
    fputs("\n\n", fd);
    fputs(moves_output.char_str(), fd);
    fclose(fd);

    return true;
}

// Load a network setup from a XML file
bool PanelNetwork::LoadXML(wxString filename)
{
    // Open the XML file
    wxXmlDocument doc(filename);

    if ((!doc.IsOk()) || (doc.GetRoot()->GetName() != wxT("network")))
        return false;

    // Clear the network and loads all nodes
    Clear();

    // Get network data
    wxXmlNode *node_net = doc.GetRoot();
    if (node_net->GetName() != wxT("network"))
        return false;

    int range = atoi(node_net->GetAttribute(wxT("txrange"), wxT("100")).char_str());
    int simtime = atoi(node_net->GetAttribute(wxT("time"), wxT("240")).char_str());
    FrameGUI *parent = (FrameGUI *)GetParent();
    parent->SetRange(range);
    parent->SetTime(simtime);

    this->width = atof(node_net->GetAttribute(wxT("width"), wxT("0")).char_str());
    this->height = atof(node_net->GetAttribute(wxT("height"), wxT("0")).char_str());
    this->fixed_size = ((this->width > 0) && (this->height > 0));

    // Adjust Panel size
    if (this->fixed_size)
    	SetVirtualSize(this->width, this->height);
    else
    	SetVirtualSize(WIDTH_DEFAULT, HEIGHT_DEFAULT);

    // Add sensors, obstacles and paths
    wxXmlNode *child = doc.GetRoot()->GetChildren();
    while (child)
    {
        // Sensor
        if (child->GetName() == wxT("sensor"))
        {
            // Sensor attributes
            float x = atof(child->GetAttribute(wxT("x"), wxT("0")).char_str());
            float y = atof(child->GetAttribute(wxT("y"), wxT("0")).char_str());
            int rl = atoi(child->GetAttribute(wxT("rl"), wxT("0")).char_str());

            bool random_pos = child->GetAttribute(wxT("random_pos"), wxT("false")) == wxT("true") ? true : false;
            bool random_rl  = child->GetAttribute(wxT("random_rl"), wxT("false"))  == wxT("true") ? true : false;

            // Create a new node
            if (random_pos)
            {
                Point p = GetRandomPosition();
                x = p.GetX();
                y = p.GetY();
            }

            if (random_rl)
                rl = rand() % 15;

            Node *newnode = new Node(x, y, rl);

            // Get node control changes
            wxXmlNode *nodechild = child->GetChildren();
            while (nodechild)
            {
                if (nodechild->GetName() == wxT("control"))
                {
                    int stime = atoi(nodechild->GetAttribute(wxT("time"), wxT("0")).char_str());
                    int rl = atoi(nodechild->GetAttribute(wxT("rl"), wxT("0")).char_str());

                    bool active = nodechild->GetAttribute(wxT("enabled"), wxT("false")) == wxT("true") ? true : false;

                    newnode->InsertControl(stime, rl, active);
                }

                // Get next control change
                nodechild = nodechild->GetNext();
            }

            // Insert the node
            InsertNode(newnode);
        }

        // Obstacle
        else if (child->GetName() == wxT("obstacle"))
        {
            float x = atof(child->GetAttribute(wxT("x"), wxT("0")).char_str());
            float y = atof(child->GetAttribute(wxT("y"), wxT("0")).char_str());
            float r = atoi(child->GetAttribute(wxT("radius"), wxT("1")).char_str());

            Obstacle newobs(x, y, r);
            InsertObstacle(newobs);
        }

        // Path
        else if (child->GetName() == wxT("path"))
        {
            float xa = atof(child->GetAttribute(wxT("xa"), wxT("0")).char_str());
            float ya = atof(child->GetAttribute(wxT("ya"), wxT("0")).char_str());
            float xb = atof(child->GetAttribute(wxT("xb"), wxT("0")).char_str());
            float yb = atof(child->GetAttribute(wxT("yb"), wxT("0")).char_str());

            Path newpath(xa, ya, xb, yb);
            InsertPath(newpath);
        }

        // Get next item
        child = child->GetNext();
    }

    PaintNow();
    return true;
}

// Save network setup to a XML file
bool PanelNetwork::SaveXML(wxString filename)
{
    wxXmlDocument doc;
    wxXmlNode *root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("network"));

    // Insert the sensor nodes in the root XML node
    vector<Node *> nodes = wsn.GetNodes();
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        wxXmlNode *newnode = new wxXmlNode(root, wxXML_ELEMENT_NODE, wxT("sensor"));

        newnode->AddAttribute(wxT("x"), wxString::Format(wxT("%f"), nodes.at(i)->GetX()));
        newnode->AddAttribute(wxT("y"), wxString::Format(wxT("%f"), nodes.at(i)->GetY()));
        newnode->AddAttribute(wxT("rl"), wxString::Format(wxT("%d"), nodes.at(i)->GetRL()));
    }

    // Insert the obstacles in the root XML node
    vector<Obstacle> obstacles = wsn.GetObstacles();
    for (unsigned int i = 0; i < obstacles.size(); i++)
    {
        wxXmlNode *newnode = new wxXmlNode(root, wxXML_ELEMENT_NODE, wxT("obstacle"));

        newnode->AddAttribute(wxT("x"), wxString::Format(wxT("%f"), obstacles.at(i).GetX()));
        newnode->AddAttribute(wxT("y"), wxString::Format(wxT("%f"), obstacles.at(i).GetY()));
        newnode->AddAttribute(wxT("radius"), wxString::Format(wxT("%f"), obstacles.at(i).GetRadius()));
    }

    // Insert the paths in the root XML node
    vector<Path> paths = wsn.GetPaths();
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        wxXmlNode *newnode = new wxXmlNode(root, wxXML_ELEMENT_NODE, wxT("path"));

        newnode->AddAttribute(wxT("xa"), wxString::Format(wxT("%f"), paths.at(i).GetPointA().GetX()));
        newnode->AddAttribute(wxT("ya"), wxString::Format(wxT("%f"), paths.at(i).GetPointA().GetY()));
        newnode->AddAttribute(wxT("xb"), wxString::Format(wxT("%f"), paths.at(i).GetPointB().GetX()));
        newnode->AddAttribute(wxT("yb"), wxString::Format(wxT("%f"), paths.at(i).GetPointB().GetY()));
    }

    doc.SetRoot(root);
    bool saved = doc.Save(filename);

    return saved;
}

// Set the current tool
void PanelNetwork::SetTool(panel_tools tool)
{
    this->tool = tool;
}
