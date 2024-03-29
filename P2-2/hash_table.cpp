/*
 Student Name: Samay Chandna
 Date: 11/01/2021

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"

/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry **buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void *value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry *next;
};

/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
  HashTableEntry *newHTE = (HashTableEntry *)malloc(sizeof(HashTableEntry)); // Allocates space for a new hashtable entry
  newHTE->key = key; // Sets key to key paramater 
  newHTE->value = value; // Sets key to value paramater 
  newHTE->next = NULL; // Sets next to NULL
  return newHTE; // Returns the new hashtable entry
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
  unsigned int bucketNum = hashTable->hash(key); // Finds bucket of hashed key
  HashTableEntry *temp = hashTable->buckets[bucketNum]; // Retrieves head of linked list
  while (temp != NULL) // While we are still looking for an entry
  {
    if (temp->key == key) // If the key of the temp is equal to the paramater key
    {
      return temp; // return the hashtable entry
    }
    temp = temp->next; // Sets temp to next value to check if key matches
  }
  return temp; // Returns null
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets == 0)
  {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i = 0; i < numBuckets; ++i)
  {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
  for (unsigned int i = 0; i < hashTable->num_buckets; ++i) // Looks through every bucket of the hashtable
  {
    HashTableEntry *current = hashTable->buckets[i]; // Looks at first bucket
    HashTableEntry *next; // Variable created to look at the next hashtable entry
    while (current != NULL) // While current is not null
    {
      next = current->next; // Sets next to next
      free(current); // Frees the current hashtable entry
      current = next; // Sets current to next
    }
  }
  free(hashTable->buckets); // Frees buckets after each entry is removed from each lined list
  free(hashTable); // Frees hashtable
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
  HashTableEntry *newHTE = createHashTableEntry(key, value); // Creates new hashtable entry
  unsigned int bucketNum = hashTable->hash(key); // Finds bucket of hashed key
  if (hashTable->buckets[bucketNum] == NULL) // If linked list has no head
  {
    hashTable->buckets[bucketNum] = newHTE; // The head becomes the hashtable entry to be inserted
    return NULL;
  }
  HashTableEntry *curr = hashTable->buckets[bucketNum]; // Retrieves head of linked list
  while (curr != NULL) // While current hashtable entry is not null
  {
    if (curr->key == key) // In this case, it replaces the value of the entry, and returns the old value
    {
      void *oldValue = curr->value; // Sets aside old value
      curr->value = value; // Sets value to new paramater value
      return oldValue; // Returns the old value
    }
    if (curr->next == NULL) // In this case, the entry's key does not exist and gets added to the hashtable
    {
      curr->next = newHTE; // Sets the tail of the linked list to the hashtable entry to be added
      break;
    }
    curr = curr->next; // If no empty spot found, or if no entry shares the same key, move onto the next spot
  }
  return NULL; // Returns null afer inserting new value
}

void *getItem(HashTable *hashTable, unsigned int key)
{
  HashTableEntry *entry = findItem(hashTable, key); // Gets entry of item if it is in the table
  if (entry == NULL) // If the item does not exist within the hashtable
  {
    return NULL; // Return null
  }
  return entry->value; // Returns entry's value if its key exists within the hashtable
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
  unsigned int bucketNum = hashTable->hash(key); // Finds bucket of hashed key
  HashTableEntry *curr = hashTable->buckets[bucketNum]; // Retrieves head of linked list
  if (curr == NULL) // If bucket is empty
  {
    return NULL; // Return null
  }
  if (hashTable->buckets[bucketNum]->key == key) // Replaces head, frees memory, and replaces with next entry
  {
    HashTableEntry *head = hashTable->buckets[bucketNum]; // Saves head of the linked list
    void *value = head->value; // Saves the value of the head which is about to be removed
    hashTable->buckets[bucketNum] = head->next; // Sets the head of the linked list to the next entry
    free(head); // Frees the head from the heap
    return value; // Returns the value of the head
  }
  HashTableEntry *next; // Creates a variable for the next entry
  while (curr->next != NULL) // While the next entry is not null
  {
    next = curr->next; // The next variable is set to the next entry
    if (next->key == key) // If the key of the next entry matches the paramater key
    {
      curr->next = next->next; // Sets the current entry's next link the next entry's next table entry
      void *value = next->value; // Saves the value of the entry to be removed
      free(next); // Frees the entry from the table
      return value; // Returns the value of the removed entry
    }
    curr = curr->next; // If entry not found, check next entry
  }
  return NULL; // Returns null if entry is not found
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
  free(removeItem(hashTable, key)); // removeItem returns a value, so this frees that value once the item is deleted.
}