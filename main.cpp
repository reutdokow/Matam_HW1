#include "BlockChain.h"
#include "Utilities.h"
#include "Transaction.h"
#include <iostream>


using std::cerr;
using std::string;
using std::cout;
using std::endl;


#define Expected_args 4
#define OPERATION 1
#define SOURCE 2
#define TARGET 3

int main (int argc, char* argv[])
{
    if (argc != Expected_args)
    {
        cout << getErrorMessage();
        return -1;
    }

    string operation = argv[OPERATION];
    std::ifstream source_file(argv[SOURCE]);


    BlockChain block_chain = BlockChainLoad(source_file);
    if (operation == "format")
    {
        ofstream target_file(argv[TARGET]);
        BlockChainDump(block_chain,target_file);
    }
    else if (operation == "hash")
    {
        ofstream target_file(argv[TARGET]);
        BlockChainDumpHashed(block_chain,target_file);
    }
    else if (operation == "compress")
    {
        BlockChainCompress(block_chain);
        ofstream target_file(argv[TARGET]);
        BlockChainDumpHashed(block_chain,target_file);
    }
    else if (operation == "verify")
    {
        BlockChainCompress(block_chain);
        std::ifstream new_target_file(argv[TARGET]);
        if (BlockChainVerifyFile(block_chain,new_target_file))
        {
            cout << "Verification passed" << std::endl;
        }
        else
        {
            cout << "Verification failed" << std::endl;
        }
    }
    BlockChainDestroy(block_chain);
    return 0;
}