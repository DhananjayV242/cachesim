/* 
This file consists of the main function of the program. 

Author: Dhananjay Vijayaraghavan
Date Created: 18 September 2024

TODO: Edit with changes

Originally the sc_main function is written in no different manner from 
a conventional main function in C++. However, maybe future improvements 
or additions may require SystemC functionality, hence the program is built around using SystemC 
*/

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

#include "ArgParser.h"
#include "Cache.h"
#include "TraceParser.h"
#include "Metrics.h"

void printMetrics(Metrics *metrics);

int main(int argc, char *argv[]){

    printf("*****************************\n");
    printf("        CACHE SIMULATOR      \n");
    printf("*****************************\n");
    printf("\n\n");

    // CLI Parser initialization
    ArgParser parser(argc, argv);
    
    printf("CLI Configuration details \n");
    parser.print_params();

    // Tracefile parser initialization
    TraceParser traceparser(parser.tracefile);

    // Flags to determine config for each level of the cache
    bool en_l2 = (parser.L2_SIZE!=0);
    bool en_prefetch = (parser.PREF_N!=0);
    bool en_l1_pf = (!en_l2 && en_prefetch);
    bool en_l2_pf = (en_l2 && en_prefetch);

    // Setting up the cache
    printf("\nSetting up Caches.........\n");

    Memory *ddr = new Memory();
    Memory *l1_nxt = ddr;

    Cache *l1 = new Cache(parser.BLOCKSIZE, parser.L1_SIZE, parser.L1_ASSOC, en_l1_pf, parser.PREF_N, parser.PREF_M);
    if(en_l2){
        Cache *l2 = new Cache(parser.BLOCKSIZE, parser.L2_SIZE, parser.L2_ASSOC, en_l2_pf, parser.PREF_N, parser.PREF_M);
        l1_nxt = l2;
        l2->setNext(ddr);
    }

    l1->setNext(l1_nxt);


    // Set Metrics objects
    Metrics *metrics = new Metrics();
    l1->setMetrics(metrics);


    // Cache operation
    printf("Starting Cache test ..........\n\n\n");
    command cmd;
    while(traceparser.getcommand(cmd)){
        l1->request(cmd.rw, cmd.address, false);
    }

    l1->printState();
    printMetrics(metrics);

    printf("Ending simulation ........ \n");    
    return 0;

}

void printMetrics(Metrics *metrics){

    std::cout << metrics->get_n_access() << std::endl;
    std::cout << metrics->get_n_reads() << std::endl;    
    std::cout << metrics->get_n_writes() << std::endl;
    std::cout << metrics->get_n_writebacks() << std::endl;
    std::cout << metrics->get_n_prefetches() << std::endl;
    std::cout << metrics->get_n_read_miss() << std::endl;
    std::cout << metrics->get_n_write_miss() << std::endl;
    std::cout << metrics->get_n_pf_read() << std::endl;
    std::cout << metrics->get_n_pf_read_miss() << std::endl;
}

//EOF