#include "Transaction.h"
#include "Utilities.h"
#include "BlockChain.h"
#include <fstream>
#include <string>
#include <iostream>


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
    return temp;
}



BlockChain* getBlockChain(
Transaction* transaction,
const string& timestamp)
{
    BlockChain* temp = new BlockChain();
    temp->data = transaction;
    temp->next = nullptr;
    temp->timestamp = timestamp;
    return temp;
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
    delete trans;
}


void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
)
{
    Transaction* tran = getTransaction(transaction);
    BlockChain* new_block = getBlockChain(tran, timestamp);
    BlockChain* ptr = &blockChain;
    while (ptr->next != nullptr) {
        ptr = ptr->next;
    }
    ptr->next = new_block;
}




BlockChain BlockChainLoad(ifstream& file)
{
    BlockChain head = BlockChain();
    string sender, receiver, timestamp;
    unsigned int value;
    while (file >> sender >> receiver >> value >> timestamp)
    {
        BlockChainAppendTransaction(head,value,sender,receiver,timestamp);
    }
    return head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file)
{

    file << "BlockChain Info:" << endl;
    int num = 0;
    const BlockChain* ptr = &blockChain;
    while (ptr != nullptr)
    {
        if (ptr->data == nullptr)
        {
            ptr = ptr->next;
            continue;
        }
        num++;
        file << num << "." << endl;
        file << "Sender Name: " << ptr->data->sender << endl;
        file << "Receiver Name: " << ptr->data->receiver << endl;
        file << "Transaction Value: " << ptr->data->value << endl;
        file << "Transaction timestamp: " << ptr->timestamp << endl;
        file << endl;
        ptr = ptr -> next;
    }
}


void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file)
{
    const BlockChain* ptr = blockChain.next;
    while (ptr != nullptr)
    {
        if (ptr->data == nullptr)
        {
            ptr = ptr->next;
            continue;
        }
        string curr_hash = hash (ptr->data->value, ptr->data->sender,ptr->data->receiver);
        file << curr_hash << endl;
        ptr = ptr -> next;
    }
}


bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file)
{
    string curr_line;
    const BlockChain* ptr = blockChain.next;
    while (ptr != nullptr)
    {
        if (ptr->data == nullptr)
        {
            ptr = ptr->next;
            continue;
        }
        if (!getline(file, curr_line)) {
            return false;
        }
        std::cout << TransactionHashedMessage(*ptr->data) << " VS " << curr_line << std::endl;
        if (!TransactionVerifyHashedMessage(*ptr->data,curr_line))
        {
            return false;
        }
        ptr = ptr -> next;
    }
    if (getline(file, curr_line)) {
        return false;
    }
    return true;
}


void BlockChainCompress(BlockChain& blockChain)
{
    BlockChain* current = &blockChain;

    while (current != nullptr && current->next != nullptr)
    {
        if (current-> data!= nullptr && current->next->data != nullptr &&
            current->data->sender == current->next->data->sender &&
            current->data->receiver == current->next->data->receiver)
        {
            current->data->value += current->next->data->value;
            current->timestamp = current->next->timestamp;
            BlockChain* BlocktoDelete = current -> next;
            current->next = BlocktoDelete->next;

            delete BlocktoDelete->data;
            delete BlocktoDelete;
            continue;
        }
        else
        {
            current = current -> next;
        }

    }
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



void BlockChainDestroy(BlockChain& block_chain)
{
    BlockChain* current = &block_chain;

    while (current != nullptr)
    {
        BlockChain* next = current->next;

        if (current->data != nullptr) {
            delete current->data;
            current->data = nullptr;
        }

        if (current != &block_chain) {
            delete current;
        }

        current = next;
    }

    block_chain.next = nullptr;
    block_chain.timestamp = "";
}

