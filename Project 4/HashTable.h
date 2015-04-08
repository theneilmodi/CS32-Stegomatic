//
//  HashTable.h
//  Project 4
//
//  Created by Nilay Modi on 3/8/15.
//  Copyright (c) 2015 Nilay Modi. All rights reserved.
//

#ifndef __Project_4__HashTable__
#define __Project_4__HashTable__

#include <iostream>

template <typename KeyType,	typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets,	unsigned int capacity);
    ~HashTable();
    bool isFull()	const;
    bool set(const KeyType&	key, const ValueType& value, bool permanent =	false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,	ValueType& value);

private:
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    
    struct Node{
        KeyType n_key;
        ValueType n_value;
        Node* n_next;
        Node* n_prev;
        bool n_permanent;
    };
    
    Node* *m_buckets;
    
    Node *m_mostRecentHead;
    Node *m_mostRecentTail;
    
    int m_capacity;
    int m_totalBuckets;
    int m_totalNodes;
    
};


template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
{
    m_buckets = new Node*[numBuckets];
    for(int i = 0; i<numBuckets; i++){
        m_buckets[i] = nullptr;
    }
        
    m_mostRecentHead = nullptr;
    m_mostRecentTail = nullptr;
    
    m_capacity = capacity;
    m_totalBuckets = numBuckets;
    m_totalNodes = 0;
}

template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::~HashTable()
{
    // deleting node in each bucket
    for(int i = 0; i<m_totalBuckets; i++){
        
        Node *headNode = m_buckets[i];
        while(headNode != nullptr){
            Node *nextNode = headNode->n_next;
            delete headNode;

            headNode = nextNode;
        }
        

    }
    
    //deleting buckets array
    delete[] m_buckets;
    
    // deleting most recent list
    while (m_mostRecentHead != nullptr) {
        Node *nextNode = m_mostRecentHead->n_next;
        delete m_mostRecentHead;
        m_mostRecentHead = nextNode;
    }
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{
    return m_totalNodes == m_capacity;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)
{
    unsigned int computeHash(KeyType);
    int HASH = computeHash(key) % m_totalBuckets;
    Node* nodeInBucket = m_buckets[HASH];
    
    // updating existing node
    while(nodeInBucket != nullptr){
        if(nodeInBucket->n_key == key){
            nodeInBucket->n_value = value;
            
            //adding to top of most recent table
            if(!nodeInBucket->n_permanent){
                touch(key);
            }
            
            return true;
        }
        nodeInBucket = nodeInBucket->n_next;
    }
    
    
    if(isFull()) return false;
    
    //creating a new node
    Node *myNode = new Node();
    myNode->n_key = key;
    myNode->n_value = value;
    myNode->n_permanent = permanent;
    myNode->n_next = m_buckets[HASH];
    myNode->n_prev = nullptr;
    
    /// adding the new node to the head appropriately
    if(m_buckets[HASH] != nullptr)
        m_buckets[HASH]->n_prev = myNode;
    
    m_buckets[HASH] = myNode;
    
    //adding to top of most recent table
    if(!permanent){
        Node* tempNode = new Node();
        tempNode->n_key = key;
        tempNode->n_value = value;
        tempNode->n_permanent = permanent;
        tempNode->n_next = m_mostRecentHead;
        if(m_mostRecentHead != nullptr)
            m_mostRecentHead->n_prev = tempNode;
        else m_mostRecentTail = tempNode;
        m_mostRecentHead = tempNode;
        
    }
    
    m_totalNodes++;
    
    return true;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value)	const
{
    unsigned int computeHash(KeyType);
    int HASH = computeHash(key) % m_totalBuckets;
    Node* nodeInBucket = m_buckets[HASH];
    
    // go through all nodes in bucket
    while(nodeInBucket != nullptr){
        if(nodeInBucket->n_key == key){ // if key's match return value
            value = nodeInBucket->n_value;
            return true;
        }
        nodeInBucket = nodeInBucket->n_next;
    }
    
    return false;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
    Node *tempHead = m_mostRecentHead;
    
    // go through every node is most recent listr
    while(tempHead != nullptr){
        if(tempHead->n_key == key){
            if(tempHead == m_mostRecentHead){
               return false; 
            }
            if(tempHead == m_mostRecentTail){
                m_mostRecentTail = tempHead->n_prev;
            }
            
            // connecting nodes appropriately
            Node* before = tempHead->n_prev;
            Node* after = tempHead->n_next;
            if(before != nullptr) before->n_next = after;
            if(after != nullptr) after->n_prev = before;
            
            // move node to the beginning of most recent list
            tempHead->n_prev = nullptr;
            tempHead->n_next = m_mostRecentHead;
            m_mostRecentHead->n_prev = tempHead;
            m_mostRecentHead = tempHead;
            return true;
        }
        tempHead = tempHead->n_next;
    }
    return false;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key,	ValueType& value)
{
    Node *leastRecent = m_mostRecentTail;
    
    if(leastRecent != nullptr){

        key = leastRecent->n_key; //get key from least recent node
        
        if(leastRecent == m_mostRecentHead){
            m_mostRecentHead = nullptr;
            m_mostRecentTail = nullptr;
        }else{
            //reset to tail to the one before
            m_mostRecentTail = leastRecent->n_prev;
            m_mostRecentTail->n_next = nullptr;
        }
        delete leastRecent;
        
        // setting key and value to ones of least recent
        for(int i = 0; i<m_totalBuckets; i++){
            Node *theNode = m_buckets[i];
            while(theNode != nullptr){
                if(theNode->n_key == key){
                    
                    value = theNode->n_value;  // set value
                    
                    //reconnect node's nefore and after nodes
                    Node* before = theNode->n_prev;
                    Node* after = theNode->n_next;
                    
                    if(before != nullptr) before->n_next = after;
                    if(after != nullptr) after->n_prev = before;
                    
                    //deleting head with nothing else
                    if(theNode == m_buckets[i] && theNode->n_next == nullptr)
                            m_buckets[i] = nullptr;
                    
                    //deleting head with other stuff
                    if(theNode == m_buckets[i] && theNode->n_next != nullptr)
                            m_buckets[i] = theNode->n_next;
                    
                    
                    delete theNode;
                    m_totalNodes--;
                    return true;
                }
                theNode = theNode->n_next;
            }
        }
    }
    return false;
}



#endif /* defined(__Project_4__HashTable__) */
