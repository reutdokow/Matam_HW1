#include "Transaction.h"
#include "Utilities.h"
#include <fstream>
#include <string>

using std::string;
using std::ofstream;


void TransactionDumpInfo(const Transaction& transaction, ofstream& file)
{
    file << "Sender Name: " << transaction.sender << "\n";
    file << "Receiver Name: " << transaction.receiver << "\n";
    file << "Transaction Value: " << transaction.value << "\n";
}

string TransactionHashedMessage(const Transaction& transaction)
{
  return hash(transaction.value, transaction.sender, transaction.receiver);
}


bool TransactionVerifyHashedMessage(const Transaction& transaction,string hashedMessage)
{
  return TransactionHashedMessage(transaction) == hashedMessage;
}