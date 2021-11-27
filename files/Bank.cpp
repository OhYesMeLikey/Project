
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>

#include "Bank.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
     accountId = type = updateDate = 0;
     clientName = new char[1];
     balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                char *name, long newDate, double newBalance) : accountId(id), type(newType),
                                                                               updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete[] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char *name)
{
     if (clientName)
     {
          delete[] clientName;
     }
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print()
{
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char *name,
                                      long newDate, double balanceArg, int nbyear) : BankAccount(id, newType, name,
                                                                                                 newDate, balanceArg),
                                                                                     nbyears(nbyear)
{
}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print()
{
     Bonus();
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
     setBalance(getBalance() + (getBalance() * getRate() * (getNbYears()) / 36000.00));
}

//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char *name,
                                long newDate, double newBalance, int nbyear,
                                double newRate) : BankAccount(id, newType,
                                                              name, newDate, newBalance),
                                                  nbyears(nbyear), rate(newRate)
{
}

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print()
{
     BankAccount::print();
     cout.setf(ios::fixed);
     cout.precision(2);
     cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                int codeTr = 01, double amountTr) : accountId(idTr), type(typeTr),
                                                                    date(dateTr), code(codeTr),
                                                                    amount(amountTr)
{
}

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}

//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount **list)
{
     //We decided to sort based off ID first in ascending order, and for ACCOUNTS
     //with the same id, they will be sorted based on account type also in ascending order.
     //The overloaded comparison operator behaves abnormally for bankaccounts objects,
     //will just use the getter methods to make direct member comparison.

     //Implementing with bubble sort
     for (int i = 0; i < K_SizeMax; i++)
     {
          if (list[i + 1]->getAccountId() == 0)
          {
               //cout << "[Done Sort]" << endl;
               break;
          }

          for (int j = 0; j < K_SizeMax; j++)
          {
               if (list[j + 1]->getAccountId() == 0)
               {
                    break;
               }

               int currentID = list[j]->getAccountId();
               int nextID = list[j + 1]->getAccountId();

               if (nextID < currentID)
               {
                    BankAccount *temp = list[j + 1];
                    list[j + 1] = list[j];
                    list[j] = temp;
               }

               //For when there are multiple account types for one ID.
               else if (nextID == currentID)
               {
                    if (nextID < currentID)
                    {
                         BankAccount *temp = list[j + 1];
                         list[j + 1] = list[j];
                         list[j] = temp;
                    }
               }
          }
     }

     //For testing purposes only
     /*
     cout << "Showing results" << endl;
     for (int i = 0; i < K_SizeMax; i++)
     {
          cout << list[i]->getAccountId() << endl;
          if (list[i + 1]->getAccountId() == 0)
          {
               //cout << "Done Sort" << endl;
               break;
          }
     }
     */
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount **readAccounts()
{
     ifstream inputFile("clients.txt"); // Opening the input file
     if (!inputFile)                    // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }

     BankAccount **listAccounts = new BankAccount *[K_SizeMax];
     if (!listAccounts)
     {
          cout << "Full memory. !!!" << endl;
          return listAccounts;
     }

     BankAccount **pAccount = listAccounts;

     long accountRead, dateRead;
     int TypeRead, nbyearRead, counter = 0;
     double balanceRead, RateRead;
     char nameRead[60];

     inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
     inputFile.getline(nameRead, 60);

     while (inputFile && (counter < K_SizeMax - 1))
     {
          // YOU HAVE TO DO SOMETHING FROM HERE !!!
          if (TypeRead < 3)
          {
               *pAccount = new BankAccount{accountRead, TypeRead, nameRead, dateRead, balanceRead};
          }
          else if (TypeRead == 03)
          {
               *pAccount = new DepositAccount{accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead};
          }
          else
          {
               *pAccount = new LoanAccount{accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead};
          }
          // UNTIL THIS POINT.
          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
     }
     *pAccount = new BankAccount();

     //For test printing
     /*
     cout << "----------------------Print Test----------------------" << endl;
     for (int i = 0; i < K_SizeMax; i++)
     {
          if (listAccounts[i]->getAccountId() == 0)
          {
               cout << "DONE!" << endl;
               break;
          }
          cout << "Id:" << listAccounts[i]->getAccountId() << endl;
          cout << "Type:" << listAccounts[i]->getType() << endl;
          cout << "Name:" << listAccounts[i]->getClientName() << endl;
          cout << "Update:" << listAccounts[i]->getUpdatedate() << endl;
          cout << "Balance:" << listAccounts[i]->getBalance() << endl;

          DepositAccount* subClassPtr1 = dynamic_cast<DepositAccount*> (listAccounts[i]);
          if (subClassPtr1 != nullptr){
            cout << "nbYear:" << subClassPtr1->getNbYears() << endl;
            cout << "Rate:" << subClassPtr1->getRate() << endl;
          }
          LoanAccount* subClassPtr2 = dynamic_cast<LoanAccount*> (listAccounts[i]);
          if (subClassPtr2 != nullptr){
            cout << "nbYear:" << subClassPtr2->getNbYears() << endl;
            cout << "Rate:" << subClassPtr2->getRate() << endl;
          }
          cout << endl;
     }
     */

     //cout << "[Done Read]" << endl;
     return listAccounts;
}

//*****************************************************************************************
// Purpose: This function validates whether the transaction code
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
     if (((trans.getCode() == 02) && (isDepositAccount() || isLoanAccount())) ||
         ((trans.getCode() == 03) && (isDepositAccount() || isLoanAccount() || isSavingsAccount())))
     {
          return FALSE;
     }
     else
     {
          return TRUE;
     }
}

//******************************************************************************
// Purpose: This function is used to execute the transaction already performed
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() + trans.getAmount());
          }
          else
          {
               if (trans.getCode() == 02) // Withdrawal
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - (trans.getAmount() + 0.50));
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
               else // Check
               {
                    if (getBalance() >= trans.getAmount())
                    {
                         setBalance(getBalance() - trans.getAmount());
                    }
                    else
                    {
                         cout << " insufficient balance!! " << endl;
                    }
               }
          }
     }
}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
          if (trans.getCode() == 01) // Deposit
          {
               setBalance(getBalance() - trans.getAmount());
          }
     }
}

//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt'
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount **listAccounts)
{
     ifstream inputFile("transact.txt"); // Opening the input file
     if (!inputFile)                     // If the file is not found...
     {
          cout << "File not found !!!" << endl;
          exit(0);
     }

     int accountId, accountType, date, transType;
     double amount;

     while (inputFile)
     {
          inputFile >> accountId >> accountType >> date >> transType >> amount;
          Transaction newTransaction = Transaction(accountId, accountType, date, transType, amount);

          // For testing
          /*
       cout << "Transaction Details" << endl;
       cout << "AccId: " << accountId << endl;
       cout << "AccType: " << accountType << endl;
       cout << "Date: " << date << endl;
       cout << "TransType: " << transType << endl;
       cout << "Amount: " << amount << endl << endl;
       */

          int index = 0;
          while ((listAccounts[index]->getAccountId()) != 0)
          {
               if (((listAccounts[index]->getAccountId()) == accountId) && ((listAccounts[index]->getType()) == accountType))
               {
                    listAccounts[index]->executeTransaction(newTransaction);
               }
               index++;
          }
     }
     //cout << "[Done Update]" << endl;
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount **listAccounts)
{
     cout << endl
          << endl
          << endl;

     Bool find[K_SizeMax];
     for (int k = 0; k < K_SizeMax; k++)
     {
          find[k] = FALSE;
     }

     cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
     cout << "                       ------------------------------------------" << endl
          << endl;

     // Manually sortting the list of bank accounts without the implementation of sort function
     /*
     for (int i = 0; i < count; i++)
     {
          for (int j = 0; j < count; j++)
          {
          }
     }

     int i = 0;

     int diffPeople = 0;
     double *totalSum = NULL;
     for (int index = 0; index < K_SizeMax; index++)
     {
          int currentID = listAccounts[index]->getAccountId();
          if (listAccounts[index]->getAccountId() == 0)
          {
               break;
          }
          else
          {
               int nextID == listAccounts[index + 1]->getAccountId();
               if (currentId != nextID)
               {
                    diffPeople++;
                    double *temp = new double[diffPeople];
               }
          }
     }
     */
     int i = 0;

     double totalSum = 0;

     // Assumming the list is sorted in ascending order of account id, print out the list of bank accounts
     while (i < K_SizeMax - 1)
     {
          // Checking if the current account is the "null" account
          long currentID = listAccounts[i]->getAccountId();
          if (currentID == 0)
          {
               break;
          }

          // Assign the ID of the previous account if it exists
          long prevID = listAccounts[i]->getAccountId();
          if (i != 0)
          {
               prevID = listAccounts[i - 1]->getAccountId();
          }

          // Assign the ID of the next account if it exists
          long nextID = 0;
          if (listAccounts[i + 1]->getAccountId() != 0)
          {
               nextID = listAccounts[i + 1]->getAccountId();
          }

          // Print the titles of each section for every different account
          if ((i == 0) || (prevID != currentID))
          {
               totalSum = 0;
               cout << "Client Name: " << listAccounts[i]->getClientName() << endl;
               cout << "\n"
                    << endl;
               cout << "Bank Account"
                    << "\t\t"
                    << "Type"
                    << "\t"
                    << "Update Date"
                    << "\t"
                    << "Balance"
                    << "\t\t"
                    << "Nb. Years"
                    << "\t"
                    << "Rate"
                    << endl;
               cout << string(12, '-')
                    << "\t\t"
                    << string(4, '-')
                    << "\t"
                    << string(11, '-')
                    << "\t"
                    << string(7, '-')
                    << "\t\t"
                    << string(9, '-')
                    << "\t"
                    << string(4, '-')
                    << endl;
          }
          cout << endl;
          cout << "totalSum now: " << totalSum << endl;
          cout << "balance: " << listAccounts[i]->getBalance() << endl;

          totalSum += listAccounts[i]->getBalance();

          cout << "totalSum after: " << totalSum << endl;

          // Print the details of the last account of the user
          //and the combined balance of all of the person's accounts,
          //or print the details of the person's current account
          if (currentID != nextID)
          {

               cout << endl;
               listAccounts[i++]->print();
               cout << "\n"
                    << endl;

               cout << endl;
               cout << "totalSum now: " << totalSum << endl;
               cout << "balance: " << listAccounts[i]->getBalance() << endl;
               cout << "totalSum after: " << totalSum << endl;

               cout.setf(ios::fixed);
               cout.precision(2);
               cout << "Total Amounts: " << setprecision(10) << totalSum << endl;
               cout << "\n"
                    << endl;
          }
          else
          {
               cout << endl;
               listAccounts[i++]->print();
          }
     }

     /*
     while (i < K_SizeMax - 1)
     {
          // Checking if the current account is the "null" account
          long currentID = listAccounts[i]->getAccountId();
          if (currentID == 0)
          {
               break;
          }

          cout << "Client Name: " << listAccounts[i]->getClientName() << endl;
          cout << "\n"
               << endl;
          cout << "Bank Account"
               << "\t\t"
               << "Type"
               << "\t"
               << "Update Date"
               << "\t"
               << "Balance"
               << "\t\t"
               << "Nb. Years"
               << "\t"
               << "Rate"
               << endl;
          cout << string(12, '-')
               << "\t\t"
               << string(4, '-')
               << "\t"
               << string(11, '-')
               << "\t"
               << string(7, '-')
               << "\t\t"
               << string(9, '-')
               << "\t"
               << string(4, '-')
               << endl;
          listAccounts[i++]->print();
          cout << "\n"
               << endl;
     }
     */
}

int main()
{
     BankAccount **list = readAccounts();

     sortAccounts(list);
     displayAccounts(list);
     updateAccounts(list);
     cout << endl
          << endl;
     cout << "               ************************************************" << endl;
     cout << "               * RE-DISPLAY OF DATA AFTER THE UPDATE *" << endl;
     cout << "               ************************************************" << endl;
     displayAccounts(list);
     cout << endl;

     //system("PAUSE");
     return 0;
}
