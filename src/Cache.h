/*
Header file for the Cache class. 
Contains class definitions and function prototypes. 

Author: Dhananjay Vijayaraghavan
Date Created: 18 September

*/

#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Memory.h"
#include "PrintTools.h"
#include "Metrics.h"

#define ADDR_SIZE 32

struct taginfo
{
    bool valid;
    bool dirty;
    uint32_t tag;
};

struct decoded_addr{
    uint32_t tag;
    uint32_t index;
    uint32_t block_offset;
};

typedef std::vector<std::vector<taginfo>> Tag_Array;
typedef std::vector<std::list<uint32_t>> Lru_Array;
typedef std::vector<taginfo> Set_Array;
typedef std::list<uint32_t> Lru_unit;

class Cache: public Memory{

    private:
        
        // Memory variables
        uint32_t blocksize;
        uint32_t size;
        uint32_t assoc;
        
        // Prefetch variables
        bool en_prefetch;
        uint32_t pref_n;
        uint32_t pref_m;

        // Tag variables
        /* 2-D vector to hold tag, valid and dirty info. 
        Could use map, but wanted to stick to constructs that can be modified to include timing in case SystemC is used. */ 
        Tag_Array tag_array;
        Lru_Array cache_lru;

        // Address fields
        uint32_t tag_bits;
        uint32_t block_bits;
        uint32_t index_bits;
        uint32_t num_sets;
        

    public:
        Cache(uint32_t blocksize, uint32_t size, uint32_t assoc, bool en_prefetch, uint32_t pref_n, uint32_t pref_m);
        void request(bool rw, uint32_t address, bool pf);
        uint32_t bitcount(uint32_t value);
        decoded_addr decode(uint32_t addr);
        uint32_t allocate(uint32_t index, bool evict, bool &writeback);
        uint32_t evictline(uint32_t index, bool &writeback);
        void updatelru(Lru_unit &lru, uint32_t way);

        void updateMetrics(bool rw, bool pf, bool cache_hit, bool prefetch_hit, bool writeback);

        // Debug functions

        void printConfig();
        void printState();

};

void test();
