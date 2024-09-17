A few issues and inconsistencies were discovered throughout completing this project. 
Due to this, certain design aspects had to be improvised off the assumptions we made.


Below are some important ones note:


1.
// Sample output file implies that the DepositAccount::Bonus() function was applied once
// before or during the first display call for loan type accounts. And bonus() is applied everytime print()
// is called for deposit type account. This is a rather strange behavior, but our code will try to match this behavior.


2.
// Overloaded operator behaves abnormally
// - It is flipping the direction of the comparison.
// - With both comparison for ID and Type, many bankaccounts will be determined to be equal.
//   exmaple: both (id=5,type=2) < (id=3,type=6) and (id=5,type=2) < (id=3,type=6) will return false.


3.
// We decided to sort based off ID first in ascending order, and for accounts
// with the same id, they will be sorted based on account type also in ascending order.
// The overloaded comparison operator behaves abnormally for bankaccounts objects,
// we will just use the getter methods to make direct member comparison.