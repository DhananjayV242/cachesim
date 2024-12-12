/*
Header file for command line argument parser. 
Contains class definition and function prototypes only. 

Author: Dhananjay Vijayaraghavan
Date Created: 18 September 2024. 

*/
#include <inttypes.h>

class ArgParser{

    private:
        int parse(int argc, char *argv[]);

    public: 

        uint32_t BLOCKSIZE;
        uint32_t L1_SIZE;
        uint32_t L1_ASSOC;
        uint32_t L2_SIZE;
        uint32_t L2_ASSOC;
        uint32_t PREF_N;
        uint32_t PREF_M;
        char*    tracefile;

        ArgParser(int argc, char *argv[]);        
        void print_params();

};

// EOF