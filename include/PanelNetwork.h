#ifndef PANELNETWORK_H
#define PANELNETWORK_H

#include <wx/wx.h>
#include "Network.h"
#include "Cluster.h"
#include "Point.h"
#include "Node.h"
#include "Sink.h"

// Drawing parameters

#ifdef ARTICLE
// To create article images, use:
# define NODE_RADIUS     10
# define SINK_SIZE       20
#else
// In normal operation, use:
# define NODE_RADIUS     5
# define SINK_SIZE       10
#endif // ARTICLE

#define RL_X_OFFSET     5
#define RL_Y_OFFSET     -15
#define ENERGY_X_OFFSET 10
#define ENERGY_Y_OFFSET -2
#define ENERGY_SIZE     30
#define COORD_X_OFFSET  5
#define COORD_Y_OFFSET  3
#define FONT_SIZE 8

// Sink type enum
enum
{
    SINK_STATIC,
    SINK_MOVE_VERTICAL,
    SINK_MOVE_HORIZONTAL,
    SINK_RL_FORGY,
    SINK_RL_RANDOM,
    SINK_RL_JUST,
};

// Panel tools
enum panel_tools
{
    PANEL_TOOL_SENSOR,
    PANEL_TOOL_PATH,
    PANEL_TOOL_OBSTACLE,
};

class PanelNetwork: public wxPanel
{
public:
    PanelNetwork(wxFrame *parent);

    // To avoid flickering
    void OnEraseBackGround(wxEraseEvent& event);

    float GetWidth(void);
    float GetHeight(void);
    void SetOpts(bool showRL, bool showCoord, bool drawLines, bool showEnergy, bool animate);
    void Clear(void);
    Point GetRandomPosition(void);
    void InsertNode(Node *newnode);
    void InsertObstacle(Obstacle newobs);
    void InsertPath(Path newpath);
    void InsertSink(Sink *newsink);
    int GetSinkPos(int init);
    void RunMMS(int sinks, int init, bool use_runtime_RL);
    void PaintNow(void);
    bool SavePNG(wxString filename);
    void RunSim(int init, int s_time);
    bool SaveCSV(wxString filename);
    bool LoadXML(wxString filename);
    bool SaveXML(wxString filename);
    void SetTool(panel_tools tool);

    DECLARE_EVENT_TABLE()

private:
    Network wsn;
    wxString file_output;
    wxString moves_output;
    panel_tools tool;
    Point *path_pa;
    Point mouse_point;
    float width;
    float height;

    bool showRL;
    bool showCoord;
    bool drawLines;
    bool showEnergy;
    bool animate;
    bool fixed_size;

    wxToolBar *tbar;

    void PaintEvent(wxPaintEvent & evt);
    void Render(wxDC& dc);
    void MouseClick(wxMouseEvent& event);
    void MouseMove(wxMouseEvent& event);
    void InitializeMeans(int init);
};

#endif // PANELNETWORK_H
