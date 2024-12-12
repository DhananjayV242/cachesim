/*
Implementation of the TraceParser class for the simulator

The parsing of the tracefile is handled by a separate object in order to 
easily modify code in case tracefile formats change.

Author: Dhananjay Vijayaraghavan
Date Created: 30 September 2024. 

*/

#include "TraceParser.h"

TraceParser::TraceParser(char* filename): filename(filename){

    filePtr = fopen(filename, "r");
    if(filePtr == NULL){
        printf("[TraceParser]:: File %s could not be opened!!! Please try again.\n", filename);
    }
    else{
        printf("[TraceParser]:: Trace file %s opened for reading\n", filename);
    }
}

TraceParser::~TraceParser(){
    printf("[TraceParser]:: Destructor called. Closing file\n");
    fclose(filePtr);
}

/*
Function: getcommand
Parses the tracefile and stores the command in cmd argument. 
Returns 0 if EOF or filePtr errors occur
    cmd: struct/object to store command in 
*/
int TraceParser::getcommand(command &cmd){

    int retVal;
    char rw;
    uint32_t address;

    if(filePtr != NULL){
        
        if(fscanf(filePtr, "%s %x", &rw, &address)==2){
            if(rw == 'r'){
                cmd.rw = 0;
            }
            else if(rw == 'w'){
                cmd.rw = 1;
            }
            cmd.address = address;
            retVal = 1;

            #ifdef DBG
                printf("[TraceParser]:: Command parsed\n");
                printf("[TraceParser]:: Command: %c %x\n", rw, address);
            #endif
        }
        else{
            printf("[TraceParser]:: EOF reached\n");
            retVal = 0;
        }

    }
    else{
        printf("[TraceParser]:: Trace file %s not opened correctly!!\n", filename);
        retVal = 0;
    }

    return retVal;
}

/* Function to unit test the parser. */
// void testParser(){

//     char *filename = "sampletrace.txt";
//     TraceParser parser(filename);

//     command cmd;
//     printf("Starting to read file\n");
//     while(parser.getcommand(cmd)){
        
//         printf("Received command\n");
//         printf("RW: %d Address: %x\n", cmd.rw, cmd.address);
//     }

//     printf("File reading done\n");

// }

// EOF