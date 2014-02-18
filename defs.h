#ifndef _DEFS_
#define _DEFS_

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define SUCCESSFUL_TX 1
#define COLLIDING_TX 2
#define IDLE 3
#define SLOT_ALLOCATED 4
#define SLOT_NOT_ALLOCATED 5

#define MAXSLOTS 1000000 // for how many slots per simulation run


struct Node
{
    unsigned status;
    double current_cp; //CP
    double cpmax;//CPmax
    double cpmin;//CPmin
    double z;//Random number
    unsigned long total_successful_packets;
    unsigned long collision_number_for_current_packet;

};

// each of them may have 100 nodes each at highest.
struct Node nodes_up0[100], nodes_up5[100]; // any two users, e.g., UP0 and UP5
unsigned number_of_nodes_in_UP0;
unsigned number_of_nodes_in_UP5;
unsigned long total_slots;
unsigned slot_status; //1) SUCCESSFUL_TX (2) COLLIDING_TX (3) IDLE

double CPmax_UP0,CPmin_UP0,CPmax_UP5,CPmin_UP5;
void initialize();// Function to initialize the nodes and its parameters
void simulate(); // Function to emulate the slotted Aloha in 802.15.6 MAC
void writestat();// Function for output

ofstream outfile;

#endif


