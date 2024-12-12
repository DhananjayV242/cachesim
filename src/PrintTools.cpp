/* Implementations for miscellaneous functions to print stuff

Author: Dhananjay Vijayaraghavan
Date Created: 08 October 2024
*/

#include "PrintTools.h"

/*
Function: int2hex
Converts unsigned integer to hex value formatted for size
    val: unsigned integer 
*/
std::string int2hex(uint32_t val){
    std::string hexval;
    std::stringstream stream;

    stream << std::setfill('0')
           << std::setw(sizeof(uint32_t)*2) 
           << std::hex << val;
    hexval = stream.str();

    return hexval;
}


/*
Function: printSeperator
Prints the seperator line for each row of a table.
    ncols: Number of cols in table
    width: Width of each column
    delim: deliminator character to draw line with
    offset: Optional amount of offset to the line
*/
void printSeperator(uint32_t ncols, uint32_t width, char delim, uint32_t offset = 0){
    if(offset) std::cout << std::string(offset, ' ');
    for(uint32_t i = 0; i<ncols; ++i){
        std::cout << '+' << std::string(width-2, delim);
    }
    std::cout << "+" << std::endl;
}