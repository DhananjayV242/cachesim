/*
Implementation of the Cache class for the simulator

Objectives are to create modular code matching the microarchitecture of
a cache. Considering future improvements towards cycle-accurate or SystemC
models, I have tried to use structures that model the operation 
of the cache. 

Author: Dhananjay Vijayaraghavan
Date Created: 23 September 2024
*/

#include "Cache.h"


// Function implementations

/*
Function: bitcount 
Returns the number of bits needed to count upto a number
    value: Number to count upto
*/
uint32_t Cache::bitcount(uint32_t value){
    uint32_t bits = 0;
    if(value){
        while(value){
            bits++;
            value >>= 1;
        }
        bits--;
    }
    return bits;
}


Cache::Cache(uint32_t blocksize, uint32_t size, uint32_t assoc,bool en_prefetch, uint32_t pref_n, uint32_t pref_m) 
: blocksize(blocksize), size(size), assoc(assoc), en_prefetch(en_prefetch), pref_n(pref_n), pref_m(pref_m){

    block_bits = bitcount(blocksize);
    num_sets = size/(blocksize * assoc);
    index_bits = bitcount(num_sets);
    tag_bits = ADDR_SIZE - (index_bits + block_bits);

    // Size all vectors and lists accordingly
    
    // Initialise tag array
    tag_array.resize(num_sets);
    for(uint32_t i = 0; i<num_sets; i++){
        tag_array[i].resize(assoc);
    }

    // Initialize LRU array
    cache_lru.resize(num_sets);

    // Stream Buffer initialization

    // Debug info print
    #ifdef DBG
        printConfig();
    #endif

}

/*
Function: decode
Decode the address into tag, index and block offset
    address: 32 bit address
    tag: variable to store extracted tag 
    index: variable to store extracted index
    block_offset: variable to store extracted block offset
*/
decoded_addr Cache::decode(uint32_t addr){
    decoded_addr dec_addr;
    uint32_t block_mask = (1<<block_bits) - 1;
    uint32_t ind_mask = ((1<<index_bits) - 1) << block_bits;
    uint32_t tag_mask = ((1<<tag_bits) - 1) << (block_bits + index_bits);

    dec_addr.tag = (addr & tag_mask) >> (block_bits + index_bits);
    dec_addr.index = (addr & ind_mask) >> block_bits;
    dec_addr.block_offset = addr & block_mask;
    return dec_addr;
}


/*
Function: request
Request the cache to read or write at a particular address
    rw: Pin for read/write. 0 for read and 1 for write
    address: 32 bit address
    pf: Pin to determine if request is from prefetch unit
*/
void Cache::request(bool rw, uint32_t address, bool pf){

    // Decode the address;
    decoded_addr dec_addr;
    dec_addr = decode(address);

    uint32_t tag = dec_addr.tag;
    uint32_t index = dec_addr.index;
    uint32_t block_offset = dec_addr.block_offset;
    uint32_t block_address = address >> block_bits;

    Set_Array::iterator it;
    bool cache_hit = false;
    uint32_t way;
    bool evict = true;
    bool prefetch_hit = false;
    bool writeback = false;

    // Calculate if hit
    // Parallel operation possible in SystemC/RTL
    for(uint32_t i = 0; i<assoc; i++){
        taginfo element = tag_array[index][i];

        // Check if cache hit
        if((element.tag == tag) && (element.valid)){
            cache_hit = true;
            way = i;
        }

        // Check if eviction required.
        // Checks if all ways in a set are valid, then evict required
        evict &= tag_array[index][i].valid;
    }

    #ifdef DBG
        printf("[Request]:: Tag: %x \n", tag);
        printf("[Request]:: Index: %x \n", index);
        printf("[Request]:: Hit: %d\n", cache_hit);
        if(cache_hit) printf("[Request]:: Way: %d \n", way);
        printf("[Request]:: Evict required: %d \n", evict);
    #endif

    if(!cache_hit){
        // Allocate a block for the miss
        way = allocate(index, evict, writeback);

        // Check prefetch unit if enabled
        // prefetch_hit = searchstrmbuff(block_address)


        // If miss, read from next level of memory
        if(!prefetch_hit){

            // Assumes next level of memory set during init
            // This is a cache, there is definitely a next level

            #ifdef DBG
                printf("[Request]:: Requested data from next level. Address - 0x%x", address);
            #endif
           
            next_level->request(0, address, 0);

        }
        else{
            // Bring data from prefetch to cache
            // stmbuff_read(block_address)

        }

        // Add tag information and mark valid
        tag_array[index][way].tag = tag;
        tag_array[index][way].valid = 1;
        tag_array[index][way].dirty = 0;
    }

    // Service request
    // Ignoring since data integrity is not checked.
    if(rw){
        // Write into sram
        // sram[way].write(index, data)

        // Mark dirty bit
        tag_array[index][way].dirty = 1;

        #ifdef DBG
            printf("[Request]::Write serviced\n");
            printf("[Request]::Address: %x\n", address);
        #endif

    }
    else{
        //Read from sram
        //sram[way].read(index, data);

        #ifdef DBG
            printf("[Request]::Read serviced\n");
            printf("[Request]::Address: %x\n", address);
        #endif


    }

    // Update states
    if(en_prefetch && !(cache_hit && !prefetch_hit)){
        // Update LRU for prefetch unit
        //updatelru()
        //strmbuff_fetch()
    }
    // Update LRU associated with set
    updatelru(cache_lru[index], way);

    // Update metrics
    if(metrics) updateMetrics(rw, pf, cache_hit, prefetch_hit, writeback);

    #ifdef DBG
        printf("\n");
    #endif

}

/*
Function: allocate
Allocates one way for storing missed block
    index: index of the set to allocate
*/
uint32_t Cache::allocate(uint32_t index, bool evict, bool &writeback){

    uint32_t way;
    if(!evict){
        for(way = 0; way<assoc; ++way){
            if(!tag_array[index][way].valid){
                break; 
            }
        }
    }
    else{
        // Evict LRU block from set
        way = evictline(index, writeback);
    }

    #ifdef DBG
        printf("[Allocate]:: Allocated way: %d\n", way);
    #endif

    return way;
}

/*
Function: evictline
Evicts the least recently used line in the set
    index: index of the set to evict from
*/
uint32_t Cache::evictline(uint32_t index, bool &writeback){
    uint32_t way = cache_lru[index].back();

    uint32_t tag = tag_array[index][way].tag;
    uint32_t address = (tag << (block_bits + index_bits)) | (index << block_bits); 

    if(tag_array[index][way].dirty){
        // Writeback while evicting

        // Again, assume next level exists, cause this is a cache
        next_level->request(1, address, 0);
        writeback = true;
    }
    // There is no need to mark invalid, since evict is always 
    // followed by setting new tag info. 
    return way;
}

/*
Function: updatelru
Updates the LRU queue/list at the most recently used position
Removes the element if found in the queue and moves to MRU
    way: way to be updated as MRU
*/
void Cache::updatelru(Lru_unit &lru, uint32_t way){
    // No need to check if exists or not, remove handles it
    lru.remove(way);

    // Move to front of list
    lru.push_front(way);
}


/* Print functions for debug */

/*
Function: printConfig
Prints the configuration of the cache after constructor is 
called. 
*/
void Cache::printConfig(){

    printf("####### Cache configuration #######\n");
    printf("Block size: %d \n", blocksize);
    printf("Cache size: %d \n", size);
    printf("Associativity: %d \n", assoc);
    printf("Prefetch enabled: %d", en_prefetch);
    if(en_prefetch){
        printf("Stream Buffer count: %d", pref_n);
        printf("Stream Buffer size: %d", pref_m);
    }
    printf("\n\n");
    printf("Block bits: %d\n", block_bits);
    printf("Index bits: %d\n", index_bits);
    printf("Number of sets: %d\n", num_sets);
    printf("Tag bits: %d\n", tag_bits);

    printf("\n\n");
    printf("Tag Array size: %lu x %lu\n", tag_array.size(), tag_array[0].size());
    printf("Cache LRU size: %lu\n", cache_lru.size());
    printf("###################################\n");

}

/*
Function: printState
Prints the complete state of the cache
Structure of printing:

####### Cache State #####
           Way 0  ..  Way i
       +----------+--......--+
 Idx 0 | Tag val  |   .....  |
       +----------+--......--+

####### LRU State #####
       +----------+--......--+
 Idx 0 | Tag val  |   .....  | LRU
       +----------+--......--+

*/
void Cache::printState(){

    int idx_width = 11;
    int tag_width = 13;
    int col_width = tag_width + 2; // Keeping space for delimiters
    
    uint32_t idx;
    uint32_t way;
    uint32_t tagval;

    std::string waystring;
    std::string indexstring;
    std::string tagstring;

    printf("\n############### Cache State ###############\n");

    // Print Header
    std::cout << std::string(idx_width, ' ');
    for(way = 0; way<assoc; ++way){
        waystring = std::string("Way ") + std::to_string(way) + std::string(" ");
        std::cout << "  " <<  std::setw(col_width - 3) <<  waystring;
    }
    std::cout << std::endl;

    printSeperator(assoc, col_width, '-', idx_width);

    for(idx = 0; idx<num_sets; ++idx){
        // Print Index
        indexstring = std::string("Idx ") + std::to_string(idx) + std::string(" ");
        std::cout << std::setw(idx_width) << indexstring;
        for(way = 0; way<assoc; ++way){
            
            // Print only if valid
            if(tag_array[idx][way].valid){
                tagval = tag_array[idx][way].tag;
                tagstring = int2hex(tagval);
                if(tag_array[idx][way].dirty) tagstring += std::string("(D)");
            }
            else{
                tagstring = std::string("Invalid");
            }
            std::cout << "|" << std::setw(tag_width - 1) << tagstring << " ";
        }
        std::cout << "|" << std::endl;  // Close row
        printSeperator(assoc, col_width, '-', idx_width); // Print seperator line
    }

    printf("\n################ LRU State ################\n");

    printSeperator(assoc, col_width, '-', idx_width);
    for(idx = 0; idx<num_sets; ++idx){
        // Only print if elements present
        if(cache_lru[idx].size()){
            indexstring = std::string("Idx ") + std::to_string(idx) + std::string(" ");
            std::cout << std::setw(idx_width) << indexstring;
            Lru_unit::iterator it;
            for(it = cache_lru[idx].begin(); it!=cache_lru[idx].end(); ++it){
                std::cout << "|" << std::setw(tag_width - 1) << int2hex(*it) << " ";
            }
            std::cout << "| LRU " << std::endl;
            printSeperator(assoc, col_width, '-', idx_width);
        }
    }

}

/*
Function: updateMetrics
Updates the relevant metrics based on the current request serviced
    rw: read or write request
    pf: Is the request from prefetcher
    cache_hit: hit/miss
    prefetch_hit: prefetcher hit/miss
    writeback: writeback occured
*/
void Cache::updateMetrics(bool rw, bool pf, bool cache_hit, bool prefetch_hit, bool writeback){

    // Update n_access
    metrics->inc_n_access();

    // Update all metrics
    if(!rw){
        if(pf){
            metrics->inc_n_pf_read();
        } else {
            metrics->inc_n_reads();
        }

        if(!cache_hit && !prefetch_hit){
            if(pf){
                metrics->inc_n_pf_read_miss();
            } else {
                metrics->inc_n_read_miss();
            }
        }

    }
    else{
        metrics->inc_n_writes();
        if(!cache_hit && !prefetch_hit){
            metrics->inc_n_write_miss();
        }
    }

    if(writeback) metrics->inc_n_writebacks();
}

/* Function to unit test the cache class */
void test(){
    printf("Cache Unit Test starting \n");
}
