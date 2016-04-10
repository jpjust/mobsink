#ifndef NODE_H
#define NODE_H

// TODO: include some parameters in XML file (battery, tx power, rx power)
#ifdef PARAM_REAL_WORLD
// Parameters for energy consumption (real world)
// Battery
#define VOLT     3.0f                   // Tension (Volts)
#define BATT     1000.0f                // Capacity (mAh)
#define POWER    (3.6 * BATT * VOLT)    // Initial power (Joules)

// Power consumption
#define TX_POWER 60.0f                  // Radio tx power (mW)
#define RX_POWER 60.0f                  // Radio rx power (mW)
#define PWR_BIT  (RX_POWER / 1000)      // Power consumed per second of reception (Joules)
#define PWT_BIT  (TX_POWER / 1000)      // Power consumed per second of transmission (Joules)

// Data transmission
//#define PDU_SIZE 80000.0f               // Size of a PDU (bits)
#define PDU_SIZE 1016.0f                // Size of a PDU (bits)
#define TX_BIT   0.000004f              // How much time to transmit 1 bit (seconds)
#define TX_TIME  (TX_BIT * PDU_SIZE)    // How much time to transmit 1 PDU (seconds)
#define GEN_RATE 0.2f                   // Packet generation rate (will be multiplied by sensor relevance)

#define PWR      (TX_TIME * PWR_BIT)    // Energy consumed to receive 1 PDU (Joules)
#define PWT      (TX_TIME * PWT_BIT)    // Energy consumed to transmit 1 PDU (Joules)

#else

// Parameters for energy consumption (for simulation)

#define POWER    100.0f // Initial power
#define TX_TIME  1.0f   // How many time to transmit 1 PDU
#define PWR      0.5f   // Energy consumed to receive 1 PDU
#define PWT      0.5f   // Energy consumed to transmit 1 PDU
#define GEN_RATE 0.1f   // Packet generation rate (will be multiplied by sensor relevance)

#endif // PARAM_REAL_WORLD

#include "Point.h"
#include <stdlib.h>
#include <map>

using namespace std;

// Struct to define control parameters
struct control_params
{
    int RL;
    bool active;
};

// Just to define the existence of Cluster class
class Cluster;

// Node class
class Node: public Point
{
public:
    Node(float x = 0, float y = 0, int RL = 0, Cluster *cluster = NULL, Node *nexthop = NULL);
    virtual ~Node(void);

    int GetRL(void);
    int GetRunTimeRL(void);
    Cluster *GetCluster(void);
    Node *GetNextHop(void);
    int GetHopsToSink(void);
    double GetPDUs(void);
    double GetPower(void);
    bool IsActive(void);
    double GetHopsAvg(void);

    void SetRL(int RL);
    void SetCluster(Cluster *cluster);
    void SetNextHop(Node *nexthop);
    void SetHopsToSink(int hops);

    void Reset(void);
    bool CompareLinks(Node *dest);
    void GenerateData(void);
    bool HasData(void);
    bool HasChanged(void);
    virtual double GetData(void);
    double SendData(void);
    void Work(void);
    void InsertControl(int time, int RL, bool active);
    //int SendAllData(void);

private:
    float x;
    float y;
    int RL;
    Cluster *cluster;
    Node *nexthop;
    int hops_to_sink;
    double power;
    int time_elapsed;
    double pdu;
    double hops;
    double tx;
    bool active;
    bool changed;
    struct control_params params_init;
    map<int, struct control_params> node_control;    // Key: time in seconds

    // TODO: include this in XML file
    int pkt_rate[16] = {0, 10, 10, 10, 10, 20, 20, 20, 20, 20, 20, 30, 30, 30, 30, 40};

};

#endif // NODE_H
