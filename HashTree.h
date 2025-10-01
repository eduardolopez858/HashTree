#ifndef HASHTREE_H
#define HASHTREE_H

#include <iostream>
using namespace std;

// data structure class
class HashTree{
    private:
        // Bucket class
        class Bucket{
            public:

                // height
                int height;
        
                // balance factor
                int balanceFactor();

                // height updating
                void updateHeight();

                // successor pointer
                Bucket* successor();

                // left child pointer
                Bucket* left;

                // right child pointer
                Bucket* right;

                // parent pointer
                Bucket* parent;

                // key
                int key;

                // value
                string value;

                // constructor
                Bucket(int key, string value) : height(1), key(key), value(value), left(nullptr), right(nullptr), parent(nullptr) {}

        };

        // pointer to buckets
        Bucket** currbuckets;

        // number of elements
        int numElements;

        // number of buckets
        int numBuckets;

        // hashTree loadfactor
        float loadFactor;

        // rehash height updating
        void reupdateHeights(Bucket* curr);

        // AVL rotate right
        Bucket* rotateRight(Bucket* curr);

        // AVL rotate left
        Bucket* rotateLeft(Bucket* curr);

        // AVL rotation method
        Bucket* rotate(Bucket* curr);

         // helper dfs search method
        void dfs(Bucket* curr, Bucket** newBuckets, int newNumBuckets);

        // helper migration method
        void migration(Bucket*& currIndex, Bucket* migrant);

        // rehashing method
        bool rehash();

    public:
    
        // constructor
        HashTree();

        // destructor
        ~HashTree();

        // insert hash method
        bool map(int key, string value);

        // find method
        string find(int key);

        // delete method
        bool remove(int key);

        // size method
        int size();
};
#endif