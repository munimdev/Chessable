//https://stackoverflow.com/questions/8317508/hash-function-for-a-string
#include "List.h"
#include <cmath>
#include <iostream>
#include <array>

using namespace std;

template <typename nodeType>
class HashNode
{
  public:
    string key; //stores the key of the hashnode. this helps to identify a hash index of a key in cases of collision
    LinkedList<nodeType> list; //value of the hashnode

    HashNode() //default constructor
    {
      this->key = "";
    }
    
    HashNode(string key) //parametrized constructor
    {
      this->key = key;
    }
};

template <typename nodeType>
class HashTable
{
  public:
    int totalBuckets;
    int currentSize;

    //an array of size totalBuckets that will store the hashed values
    HashNode<nodeType> *hashTable;

    HashTable(int buckets) //default constructor
    {
      this->totalBuckets = buckets;
      currentSize = 0;
      hashTable = new HashNode<nodeType>[totalBuckets];
      //will need to change .fill to some other value as fill(-1) works for int keyType only
      //hashTable.fill(-1); //fills the entire array with -1 values
    }

    // hash function to map values to key
    int hashFunction(int x) {
      return (x % totalBuckets);
    }

    int hashFunction(string x)
    {
      int hash = 37;
      for(int i=0; i<x.size(); i++)
      {
        hash = (hash*54059) ^ (int(x[i]) * 76793);
      }
      return abs(hash % totalBuckets);
    }

    // void linearProbingInsert(keyType key) //inserts an item into the hash table at a particular index generated by the hash function
    // {
    //   int hashIndex = hashFunction(key);
      
    //   while (hashTable[hashIndex] != key && hashTable[hashIndex] != -1) 
    //   { //linearly probe through the list
    //     hashIndex++;
    //     hashIndex %= totalBuckets;
    //   }

    //   if(hashTable[hashIndex] == -1)
    //     currentSize++;
    //   hashTable[hashIndex] = key;
    // }

    // void quadraticProbingInsert(keyType key) //inserts an item into the hash table at a particular index generated by the hash function
    // {
    //   int hashIndex = hashFunction(key);
      
    //   if(hashTable[hashIndex] == -1)
    //     hashTable[hashIndex] = key;
    //   else
    //   {
    //     for(int i=1; i<totalBuckets; i++)
    //     {
    //       //calaculate new hash index by quadratic probing
    //       int newHashIndex = (hashIndex + i*i) % totalBuckets;

    //       if(hashTable[newHashIndex] == -1)
    //       {
    //         hashTable[newHashIndex] = key;
    //         break;
    //       }
    //     }
    //   }
    //   currentSize++;
    // }

    void doubleHashInsert(string key, nodeType value) //inserts an item into the hash table at a particular index generated by the hash function using double hash functionality
    {
      int hashIndex = hashFunction(key);

      if( hashTable[hashIndex].key == key ) //if the key has an existing index
      {
        hashTable[hashIndex].list.insert(value); 
      }
      //else if index is unassigned, insert the data and save the key in the index 
      else if( hashTable[hashIndex].key == "" )
      {
        hashTable[hashIndex].list.insert(value);
        hashTable[hashIndex].key = key;
        currentSize++;
      }
      else if(hashTable[hashIndex].key != key && hashTable[hashIndex].key != "") //else if a collision has occurred, we must now either find its index if the key has been assigned one, or find the next empty index and assign it that
      {
        int newHashIndex;
        for(int i=1; ; i++) //infinite loop until an index is located
        {
          newHashIndex = (hashIndex + i*hashIndex) % totalBuckets; //double hash probing 
          if( hashTable[newHashIndex].key == key || hashTable[newHashIndex].key == "") 
          {
            hashTable[newHashIndex].list.insert(value);
            if(hashTable[newHashIndex].key == "")
            {
              hashTable[newHashIndex].key = key;
              currentSize++;
            }
            break;
          }
        }
      }
    };

    int getIndex(string key)
    {
      int hashedIndex = hashFunction(key);

      if(hashTable[hashedIndex].key == key) //return the index if the key matches
        return hashedIndex;
      else if(hashTable[hashedIndex].key != "") //else if a collision has occurred, we must find the actual index (if it exists) 
      {
        int newHashIndex;
        for(int i=1; ; i++) //loop to find the new index 
        {
          newHashIndex = (hashedIndex + i*hashedIndex) % totalBuckets;
          if( hashTable[newHashIndex].key == "" ) //however, if at any time an empty index is found, that means the key wasn't assigned any index to begin with, therefore -1 is returned
          {
            return -1;
          }
          else if( hashTable[newHashIndex].key == key ) {
            break;
          }
        }
        return newHashIndex;
      }
      return -1;
    }

    // void deleteItem(string key)
    // {
    //   // get the hash index of key
    //   int index = getIndex(key);

    //   // if key is found in hash hashTable, remove it
    //   if(hashTable[index] == key )
    //     hashTable[index] = -1;
      
    //   currentSize--;
    // }

    // function to display hashTable with index and its entries
    void displayHash() 
    {
      for (int i = 0; i < totalBuckets; i++) 
      {
        cout << "Index " << i << ": " << hashTable[i] << endl;
      }
      cout << endl;
    }
};