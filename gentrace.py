# Script to generate some directed testing traces for the cache

block_bits = 5
index_bits = 6
tag_bits = (32 - (block_bits + index_bits))
assoc = 4

def readAllIndex(fPtr):

    # Fill up all sets. 
    for idx in range(2**index_bits):

        tagval = 0xA0A0
        address = (tagval << (index_bits + block_bits)) | (idx << block_bits)
        outStr = "Address:: " + hex(address)
        print(outStr)

        # Read command to the address
        cmdStr = 'r ' + hex(address)[2:] + '\n'
        fPtr.write(cmdStr)

def writeAllIndex(fPtr):

    # Write to all sets
    for idx in range(2**index_bits):

        tagval = 0xA0A0
        address = (tagval << (index_bits + block_bits)) | (idx << block_bits)
        outStr = "Address:: " + hex(address)
        print(outStr)

        # Read command to the address
        cmdStr = 'w ' + hex(address)[2:] + '\n'
        fPtr.write(cmdStr)

def readCompleteSet(fPtr, idx):
    tagval = 0xA0A0
    for way in range(assoc):
        address = (tagval << (index_bits + block_bits)) | (idx << block_bits)
        outStr = "Address:: " + hex(address)
        print(outStr)

        cmdStr = 'r ' + hex(address)[2:] + '\n'
        fPtr.write(cmdStr)
        tagval += 1

def writeCompleteSet(fPtr, idx):
    tagval = 0xA0A0
    for way in range(assoc):
        address = (tagval << (index_bits + block_bits)) | (idx << block_bits)
        outStr = "Address:: " + hex(address)
        print(outStr)
        
        cmdStr = 'w ' + hex(address)[2:] + '\n'
        fPtr.write(cmdStr)
        tagval += 1

def rwAllIndex(fPtr):
    readAllIndex(fPtr)
    writeAllIndex(fPtr)

def readNSetsComplete(fPtr, N):

    for idx in range(N):
        readCompleteSet(fPtr, idx)

def testReplacement(fPtr, N):
    idx = 0
    for idx in range(N):
        readCompleteSet(fPtr, idx)
        tagval = 0xBBBB
        address = (tagval << (index_bits + block_bits)) | (idx << block_bits) 
        cmdStr = 'r ' + hex(address)[2:] + '\n'
        fPtr.write(cmdStr)

if __name__ == '__main__':
    fileName = 'strace.txt'
    fPtr = open(fileName, 'w')
    rwAllIndex(fPtr)
    # Closing the file
    fPtr.close()

