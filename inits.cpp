#ifndef _INITIALIZE_
#define _INITIALIZE_

#include "defs.h"

// function to initialize the nodes with parameters

void initialize()
{
 slot_status=IDLE;// initial value

//UP0 nodes
for(unsigned n0=1; n0<=number_of_nodes_in_UP0; n0++)
    {
        nodes_up0[n0].cpmax=CPmax_UP0;// for lower UP, e,g.,UP0
                                  //CPmax value

//The CP value is set to CPmax initially
        nodes_up0[n0].current_cp=nodes_up0[n0].cpmax;//CP

        nodes_up0[n0].cpmin=CPmin_UP0;//for lower UP, e,g.,UP0
                                   //CPmin value
        nodes_up0[n0].status = IDLE; // initial status
        nodes_up0[n0].total_successful_packets=0;
        nodes_up0[n0].collision_number_for_current_packet = 0;
    }

//UP5 nodes
 for(unsigned n5=1; n5<=number_of_nodes_in_UP5; n5++)
  {
        nodes_up5[n5].cpmax=CPmax_UP5;//for higher UP, e,g.,UP5
                                 //CPmax value

 //The CP value is set to CPmax initially
        nodes_up5[n5].current_cp=nodes_up5[n5].cpmax;

        nodes_up5[n5].cpmin=CPmin_UP5;//for higher UP, e,g.,UP5
                                   // CPmin value
        nodes_up5[n5].status = IDLE; // initial value
        nodes_up5[n5].total_successful_packets=0;
        nodes_up5[n5].collision_number_for_current_packet = 0;
   }
}// end function initialize

#endif

