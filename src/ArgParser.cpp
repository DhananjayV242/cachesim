/*
Implementation of the argument parser for the simulator. 
Seperating this functionality from main function for
easy future modification. 

Author: Dhananjay Vijayaraghavan
Date Created: 18 September 2024

*/

#include "ArgParser.h"
#include <stdio.h>
#include <stdlib.h>

/* Global variables */
const char *helptext = "USAGE: ./sim <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <PREF_N> <PREF_M> <trace_file> \n"
                       "\t BLOCKSIZE: Blocksize of cache \n";

/* Class function implementations */

ArgParser::ArgParser(int argc, char *argv[]){
    parse(argc, argv);
}

/* 
Function to parse the commandline arguments
Written seperately to ensure easy modification if CLI 
structure changes.

CLI CONFIG: 
*/

int ArgParser::parse(int argc, char *argv[]){

    if(argc != 9){
        printf("[ERROR] Need 8 commandline arguments to parse !! \n");
        printf("%s", helptext);
        return 0;
    }

    BLOCKSIZE = (uint32_t) atoi(argv[1]);
    L1_SIZE   = (uint32_t) atoi(argv[2]);
    L1_ASSOC  = (uint32_t) atoi(argv[3]);
    L2_SIZE   = (uint32_t) atoi(argv[4]);
    L2_ASSOC  = (uint32_t) atoi(argv[5]);
    PREF_N    = (uint32_t) atoi(argv[6]);
    PREF_M    = (uint32_t) atoi(argv[7]);
    tracefile =  argv[8];

    return 1;
}

void ArgParser::print_params(){
    
    printf("\t BLOCKSIZE: %d\n", BLOCKSIZE);
    printf("\t L1_SIZE:   %d\n", L1_SIZE);
    printf("\t L1_ASSOC:  %d\n", L1_ASSOC);
    printf("\t L2_SIZE:   %d\n", L2_SIZE);
    printf("\t L2_ASSOC:  %d\n", L2_ASSOC);
    printf("\t PREF_N:    %d\n", PREF_N);
    printf("\t PREF_M:  %d\n", PREF_M);
    printf("\t Tracefile: %s\n", tracefile);
    printf("\n");

}