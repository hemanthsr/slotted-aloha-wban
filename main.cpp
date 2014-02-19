#include "defs.h"
#include "inits.cpp"


// main function
int main()
{
    cout << "Started..." << endl;

/* output file will generate, the file name is:simout.txt */

    outfile.open("simout.txt",ios::out);
    outfile<<"#nodes(Lower UP)-#nodes(Higher UP)-PerClassThroughput(Lower UP)-PerClassThroughput(Higher UP)"<<endl;

    srand((unsigned)time(NULL));//  seeds the random number generator

//input CPmax and CPmin for two user priorities
cout<<"Input CPmax for lower UP (e.g. 0.125 for UP0)"<<endl;
cin>>CPmax_UP0;
cout<<"Input CPmin for lower UP (e.g. 0.0625 for UP0)"<<endl;
cin>>CPmin_UP0;

cout<<"Input CPmax for higher UP (e.g. 0.375 for UP5)"<<endl;
cin>>CPmax_UP5;
cout<<"Input CPmin for higher UP (e.g. 0.1875 for UP5)"<<endl;
cin>>CPmin_UP5;

// input number of nodes for lower and higher priority users
cout<<"Input highest number of nodes for lower UP (e.g. 15 for UP0)"<<endl;
int nodes_in_UP0;
cin>>nodes_in_UP0;
cout<<"Input highest number of nodes for higher UP (e.g. 15 for UP5)"<<endl;
int nodes_in_UP5;
cin>>nodes_in_UP5;
// number of nodes for lower UP, e.g.,UP0
    for(number_of_nodes_in_UP0=1; number_of_nodes_in_UP0<=nodes_in_UP0; number_of_nodes_in_UP0++)

// number of nodes for higher UO, e.g, UP5
      for(number_of_nodes_in_UP5=1; number_of_nodes_in_UP5<=nodes_in_UP5; number_of_nodes_in_UP5++)

    {
        initialize();
        simulate();
        writestat();
    }


    if (outfile.is_open())
    {
        outfile.close();
    }

    cout<<"It's done"<<endl;
    return 0;
} // end main function


/* slot allocation by the nodes and
 find how many nodes have got ALLOCATION in the current slot */

unsigned num_of_nodes_won_the_current_slot()
{
    unsigned count = 0;
// for UP0

    for(unsigned i = 1; i<= number_of_nodes_in_UP0; i++)
    {
/* Step(a):
 A node generates a random number from the interval [0, 1] uniformly */

        nodes_up0[i].z=(double)rand()/RAND_MAX;

/* Step (c):
 A node obtains a contended allocation by the current Aloha slot if z<=CP */

        if(nodes_up0[i].z <= nodes_up0[i].current_cp)
        {
            nodes_up0[i].status = SLOT_ALLOCATED;//slot is ALLOCATED
            count++;//count how many nodes got the slot ALLOCATION
        }
            else {nodes_up0[i].status = SLOT_NOT_ALLOCATED;}//slot NOT ALLOCATED to UP0
    }// end for

// for UP5, same as UP0 as above

    for(unsigned j = 1; j<= number_of_nodes_in_UP5; j++)
    {
        nodes_up5[j].z=(double)rand()/RAND_MAX;

        if(nodes_up5[j].z <= nodes_up5[j].current_cp)
        {
            nodes_up5[j].status = SLOT_ALLOCATED;
            count++;
        }//
            else {nodes_up5[j].status = SLOT_NOT_ALLOCATED;}
    }// end for
  return count; // how many nodes got the slot ALLOCATION in total
}// end function


// Update the nodes parameters

void process_current_slot()
{
// updates the parameters if a node transmitted successfully

  if(slot_status == SUCCESSFUL_TX) //
  {
// for UP0
    for(unsigned n0 = 1; n0<= number_of_nodes_in_UP0; n0++){

// which node of UP0 transmitted successfully?
      if(nodes_up0[n0].status == SLOT_ALLOCATED)
      {
// Update the variable to get total successful number of packets for UP0
          nodes_up0[n0].total_successful_packets++;

/* if success for current packet, this variable is set to zero for new packet */
          nodes_up0[n0].collision_number_for_current_packet=0;

/* step (b):
A successful packet transmission means the node shall update CP to CPmax for new packet */
          nodes_up0[n0].current_cp = nodes_up0[n0].cpmax;

      }
    }//end for UP0

// for UP5, do same as UP0 as above

    for(unsigned n5 = 1; n5<= number_of_nodes_in_UP5; n5++){
      if(nodes_up5[n5].status == SLOT_ALLOCATED)
        {
          nodes_up5[n5].total_successful_packets++;
          nodes_up5[n5].collision_number_for_current_packet=0;
          nodes_up5[n5].current_cp = nodes_up5[n5].cpmax;
        }
    }// end for UP5
      return;
  }// end if (slot_status == SUCCESSFUL_TX)


// updates the parameters if nodes are collided
  if(slot_status == COLLIDING_TX)
  {
// for UP0
    for(unsigned n0 = 1; n0<= number_of_nodes_in_UP0; n0++){

// which of node of UP0 got ALLOCATION but resulted in a collision?
      if(nodes_up0[n0].status == SLOT_ALLOCATED)

       {
           //how many times this packet is collided for this node
          nodes_up0[n0].collision_number_for_current_packet++;//update

/* Step (d):
If the node failed in the last contended allocation it had obtained, it shall halve the CP if
this was  the n-th time the node had collided consecutively, where n is an even number;
otherwise the CP is unchanged. If halving the CP would make the new CP smaller than CPmin ,
the node shall set CP to CPmin */

          if((nodes_up0[n0].collision_number_for_current_packet)%2 == 0){

                if(((nodes_up0[n0].current_cp)/2) < nodes_up0[n0].cpmin)
                    nodes_up0[n0].current_cp = nodes_up0[n0].cpmin;
                        else nodes_up0[n0].current_cp = (nodes_up0[n0].current_cp)/2;//CP halved
                }
       }
    }// end for UP0

// for UP5 same procedure as above colliding case for UP0
    for(unsigned n5 = 1; n5<= number_of_nodes_in_UP5; n5++){
       if(nodes_up5[n5].status == SLOT_ALLOCATED)
         {
            nodes_up5[n5].collision_number_for_current_packet++;// update

            if((nodes_up5[n5].collision_number_for_current_packet)%2 == 0){
                if(((nodes_up5[n5].current_cp)/2) < nodes_up5[n5].cpmin)
                    nodes_up5[n5].current_cp = nodes_up5[n5].cpmin;
                        else nodes_up5[n5].current_cp = (nodes_up5[n5].current_cp)/2;//CP halved
                }
         }
    }// end for UP5

     return;
  }// end if (slot_status == COLLIDING_TX)

}//end process_current_slot function


// emulate the Slotted Aloha of IEEE 802.15.6

void simulate()
{
    long n_tx;

// Slot-wise calculation, total slot is from 1 to MAXSLOT
    for(total_slots = 1;total_slots <= MAXSLOTS; total_slots++)
    {
// to find how many nodes are being ALLOCATED in the current slot
            n_tx = num_of_nodes_won_the_current_slot();
            if(n_tx >1)//more than 1 node got slot ALLOCATION?
            {
                slot_status = COLLIDING_TX;
            }

            else if(n_tx == 1)//only 1 node got slot ALLOCATION

            {
                slot_status = SUCCESSFUL_TX;
            }

            else if(n_tx == 0)//No node got slot ALLOCATION(IDLE)
            {
                // do nothing
            }
//update node status and other parameters
        {
            process_current_slot();
        }

    }// end for total_slots
}// end simulate function


/* calculate the normalized saturation throughput and print output in a file */
void writestat()
{
    double throughput_up0, throughput_up5;
    unsigned long total_successful_packets_up0=0,total_successful_packets_up5=0;

    for(unsigned n0=1; n0<=number_of_nodes_in_UP0; n0++){
/* for UP0
per class throughput for UP0 = total_successful_packets_up0/total slots */

      total_successful_packets_up0 = total_successful_packets_up0+ nodes_up0[n0].total_successful_packets;
      throughput_up0 = (double)(total_successful_packets_up0)/(MAXSLOTS);
    }

    for(unsigned n5=1; n5<=number_of_nodes_in_UP5; n5++){
/* for UP5
 per class throughput for UP5 = total_successful_packets_up5/total slots */
      total_successful_packets_up5 = total_successful_packets_up5+ nodes_up5[n5].total_successful_packets;
      throughput_up5 = (double)(total_successful_packets_up5)/(MAXSLOTS);
    }
// file operation
    outfile<<number_of_nodes_in_UP0<<"   "<<number_of_nodes_in_UP5<<"  "<<throughput_up0<<"  "<<throughput_up5<<"  "<<endl;

    cout<<"nodes_in_UP0: "<<number_of_nodes_in_UP0<<"nodes_in_UP5: "<<number_of_nodes_in_UP5<<"throughput_up0: "<<throughput_up0<<"throughput_up5: "<<throughput_up5<<endl;
}// end writestat function



