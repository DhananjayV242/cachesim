/*
Header file for parsing the trace file to analyse performance. 
Contains the class definition and prototypes for functions. 

Author: Dhananjay Vijayaraghavan
Date Created: 30 September 2024
*/

#include <stdio.h>
#include <inttypes.h>

struct command{
    bool rw;
    uint32_t address;
};

class TraceParser{

    private:
        FILE* filePtr;
        char *filename;

    public:
        TraceParser(char *filename);
        ~TraceParser();
        int getcommand(command &cmd);

};

void testParser();