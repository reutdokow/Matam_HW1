#include "BlockChain.h"
#include "Utilities.h"
#include "Transaction.h"
#include <iostream>


using std::cerr;
using std::string;
using std::cout;


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
    std::ofstream target_file(argv[TARGET]);
    std::ifstream new_target_file(argv[TARGET]);

    BlockChain block_chain = BlockChainLoad(source_file);
    if (operation == "format")
    {
        BlockChainDump(block_chain,target_file);
    }
    else if (operation == "hash")
    {
        BlockChainDumpHashed(block_chain,target_file);
    }
    else if (operation == "compress")
    {
        BlockChainCompress(block_chain);
        BlockChainDump(block_chain,target_file);
    }
    else if (operation == "verify")
    {
        if (BlockChainVerifyFile(block_chain,new_target_file))
        {
            cout << "Verification passed";
        }
        else
        {
            cout << "Verification failed";
        }
    }
    BlockChainDestroy(block_chain);
    return 0;
}