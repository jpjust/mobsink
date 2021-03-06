/*
 * Nodes modeling for MobSink.
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
//#define PDU_SIZE 80000.0f                  // Size of a PDU (bits)
#define PDU_SIZE    1016.0f                  // Size of a PDU (bits)
#define TX_BIT      0.000004f                // How much time to transmit 1 bit (seconds)
#define TX_TIME     (TX_BIT * PDU_SIZE)      // How much time to transmit 1 PDU (seconds)
#define GEN_RATE    10                     // Packet generation rate (will be multiplied by sensor relevance)
#define DATA_BUFFER 1048576.0f               // Data buffer size in bits (1,048,576 bits = 128 KB)
#define PDU_BUFFER  (DATA_BUFFER / PDU_SIZE) // Data buffer size in PDUs

#define PWR      (TX_TIME * PWR_BIT)    // Energy consumed to receive 1 PDU (Joules)
#define PWT      (TX_TIME * PWT_BIT)    // Energy consumed to transmit 1 PDU (Joules)

#else

// Parameters for energy consumption (for simulation)

#define POWER    100.0f   // Initial power
#define TX_TIME  1.0f     // How many time to transmit 1 PDU
#define PWR      0.5f     // Energy consumed to receive 1 PDU
#define PWT      0.5f     // Energy consumed to transmit 1 PDU
#define GEN_RATE 0.1f     // Packet generation rate (will be multiplied by sensor relevance)
#define PDU_BUFFER 128.0f // Data buffer size in KB

#endif // PARAM_REAL_WORLD

#include "Point.h"
#include <stdlib.h>
#include <map>

using namespace std;

// Struct to define control parameters
struct node_control_params
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
    double GetDropped(void);
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
    double dropped;
    double hops;
    double tx;
    bool active;
    bool changed;
    struct node_control_params params_init;
    map<int, struct node_control_params> node_control;    // Key: time in seconds

    // TODO: include this in XML file
    int pkt_rate[16] = {0, 10, 10, 10, 10, 20, 20, 20, 20, 20, 20, 30, 30, 30, 30, 40};

};

#endif // NODE_H
