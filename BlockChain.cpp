#include "Transaction.h"
#include "Utilities.h"
#include "BlockChain.h"
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::endl;



//        const Transaction& transaction,

Transaction* getTransaction(const Transaction& transaction)
{
    return getTransaction( transaction.value, transaction.sender, transaction.receiver);
}


Transaction* getTransaction(unsigned int value,const string sender,const string receiver)
{
 Transaction* temp = new Transaction();
    temp->value = value;
    temp->sender = sender;
    temp->receiver = receiver;
}



BlockChain* getBlockChain(
Transaction* transaction,
const string& timestamp)
{
    BlockChain* temp = new BlockChain();
    temp->data = transaction;
    temp->next = nullptr;
    temp->timestamp = timestamp;
}

int BlockChainGetSize(const BlockChain& blockChain)
{
    int counter = 0;
    for(const BlockChain* ptr = &blockChain; ptr != nullptr; ptr = ptr -> next)
    {
        counter++;
    }
    return counter;
}


int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name)
{
    int counter_sender = 0, counter_receiver = 0;
    for(const BlockChain* ptr = &blockChain; ptr != nullptr; ptr = ptr -> next)
    {
        if (ptr -> data-> sender == name)
        {
            counter_sender += (ptr -> data->value);

        }
        if (ptr -> data-> receiver == name)
        {
            counter_receiver += (ptr -> data->value);
        }
    }
    return counter_receiver - counter_sender;
}


#if 0
BlockChain* createBlock (
    unsigned int value,
    const string& sender,
    const string& receiver,
    const string& timestamp)
{
    BlockChain* new_block = new BlockChain;
    new_block -> data->value = value;
    new_block -> data.sender = sender;
    new_block -> data.receiver = receiver;
    new_block -> timestamp = timestamp;
    new_block -> next = nullptr;
    return new_block;
}
#endif

void BlockChainAppendTransaction(
    BlockChain& blockChain,
    unsigned int value,
    const string& sender,
    const string& receiver,
    const string& timestamp
)
{
    Transaction* trans = getTransaction(value,sender,receiver);
    BlockChainAppendTransaction(blockChain,*trans,timestamp);
}


void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
)
{
    Transaction* tran = getTransaction(transaction);
    BlockChain* new_block = getBlockChain(tran,timestamp);
    BlockChainAppendTransaction(blockChain,*new_block);
}

void BlockChainAppendTransaction(
        BlockChain& head,
        BlockChain& new_data
)
{
    Transaction* temp = head.data;
    head.data = new_data.data;
    new_data.data = temp;

    string timestamp_temp = head.timestamp;
    head.timestamp = new_data.timestamp;
    new_data.timestamp = timestamp_temp;

    new_data.next = head.next;
    head.next = &new_data;

}

BlockChain BlockChainLoad(ifstream& file)
{

}

void HelperBlockChainDump(const BlockChain& blockChain, ofstream& file, int num)
{
    if(blockChain.next != nullptr)
    {
        HelperBlockChainDump(*blockChain.next, file, num);
    }
    num++;
    file << num << "." << endl;
    file << "Sender Name: " << blockChain.data->sender << endl;
    file << "Receiver Name: " << blockChain.data->receiver << endl;
    file << "Transaction Value: " << blockChain.data->value << endl;
    file << "Transaction timestamp: " << blockChain.timestamp << endl;
    file << endl;
}


void BlockChainDump(const BlockChain& blockChain, ofstream& file)
{

    file << "BlockChain Info:" << endl;
    int num = 0;
    HelperBlockChainDump(blockChain,file,num);
}


void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file)
{
    const BlockChain* ptr = &blockChain;
    while (ptr != nullptr)
    {
        string curr_hash = hash (ptr->data->value, ptr->data->sender,ptr->data->receiver);
        if (ptr -> next != nullptr)
        {
            file << curr_hash << endl;
        }
        else
        {
            file << curr_hash;
        }
        ptr = ptr -> next;
    }

}


bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file)
{
    string curr_line;
    const BlockChain* ptr = &blockChain;
    while (ptr != nullptr && !file.eof())
    {
        getline(file, curr_line);
        if (!TransactionVerifyHashedMessage(*blockChain.data,curr_line))
        {
            return false;
        }
        ptr = ptr -> next;
    }
    return true;
}


void BlockChainCompress(BlockChain& blockChain)
{

}

void BlockChainTransform(BlockChain& blockChain, updateFunction function)
{
    //now it's not const because I want to change the value.
    BlockChain* ptr = &blockChain;
    while (ptr != nullptr)
    {
        ptr -> data -> value = function(ptr -> data -> value);
        ptr = ptr -> next;
    }
}





