#include "HashTree.h"
#include <functional>
#include <algorithm>

// constructor method ** finished **
HashTree :: HashTree(){
    numElements = 0;
    loadFactor = 0;
    numBuckets = 32;
    currbuckets = new Bucket*[numBuckets];
    for (int i = 0; i < numBuckets; i++){
        currbuckets[i] = nullptr;
    }
}

// destructor method ** finished **
HashTree :: ~HashTree(){
    for(int i = 0; i < numBuckets; i++){
        Bucket* curr = currbuckets[i];
        while(curr != nullptr){
            Bucket* next = curr->successor();
            delete curr;
            curr = next;
        }
    }
    delete[] currbuckets;
}

// height update method ** finished **
void HashTree::Bucket::updateHeight(){
    int leftHeight = 0;
    int rightHeight = 0;

    if(left != nullptr){
        leftHeight = left->height;
    }

    if(right != nullptr){
        rightHeight = right->height;
    }

    height = max(leftHeight, rightHeight) + 1;
}

// rehash updating heights recursive method ** finished **
void HashTree :: reupdateHeights(Bucket* curr){
    if(curr == nullptr) return;

    reupdateHeights(curr->left);

    reupdateHeights(curr->right);

    curr->updateHeight();
}

// balance factor method ** finished **
int HashTree::Bucket::balanceFactor(){
    int leftHeight = 0;
    int rightHeight = 0;

    if(left != nullptr){
        leftHeight = left->height;
    }

    if(right != nullptr){
        rightHeight = right->height;
    }
    
    return leftHeight - rightHeight;
}

// right rotation method ** finished **
HashTree :: Bucket* HashTree :: rotateRight(Bucket* curr){

    Bucket* newRoot = curr->left;
    Bucket* temp = newRoot->right;

    // rotating
    newRoot->right = curr;
    curr->left = temp;

    // updating parents
    newRoot->parent = curr->parent;
    curr->parent = newRoot;
    if(temp != nullptr){
        temp->parent = curr;
    }

    // reconnecting grandparent
    if(newRoot->parent != nullptr){
        if(newRoot->parent->left == curr){
            newRoot->parent->left = newRoot;
        }else{
            newRoot->parent->right = newRoot;
        }
    }

    // update heights
    curr->updateHeight();
    newRoot->updateHeight();

    return newRoot;
}

// left rotation method ** finished **
HashTree :: Bucket* HashTree :: rotateLeft(Bucket* curr){

    Bucket* newRoot = curr->right;   
    Bucket* temp = newRoot->left;    

    // rotating
    newRoot->left = curr;
    curr->right = temp;

    // updating parents
    newRoot->parent = curr->parent;
    curr->parent = newRoot;
    if(temp != nullptr){
        temp->parent = curr;
    }

    // reconnecting grandparent
    if(newRoot->parent != nullptr){
        if(newRoot->parent->left == curr){
            newRoot->parent->left = newRoot;
        }else{
            newRoot->parent->right = newRoot;
        }
    }

    // update heights
    curr->updateHeight();
    newRoot->updateHeight();

    return newRoot;
}

// rotation method ** finished **
HashTree :: Bucket* HashTree :: rotate(Bucket* curr){

    int bf = curr->balanceFactor();

    // case where it's left -> left heavy (rotate right once)
    if(bf < -1 and curr->left->balanceFactor() <= 0){
        return rotateRight(curr);
    }

    // case where it's right -> right heavy (rotate left once)
    if(bf > 1 and curr->right->balanceFactor() >= 0){
        return rotateLeft(curr);
    }

    // case where it's left -> right heavy (rotate left on left child and right on current)
    if(bf < -1 and curr->left->balanceFactor() > 0){
        curr->left = rotateLeft(curr->left);
        curr->left->parent = curr;
        return rotateRight(curr);
    }

    // case where it's right -> left heavy (rotate right on right child and left on current)
    if(bf > 1 and curr->right->balanceFactor() < 0){
        curr->right = rotateRight(curr->right);
        curr->right->parent = curr;
        return rotateLeft(curr);
    }

    // update height if no rotation
    curr->updateHeight();
    return curr;
}

// find method ** finished **
string HashTree :: find(int key){

    // index mapping
    int idx = key % numBuckets;

    // case of no value
    if(currbuckets[idx] == nullptr){return "None";}
   
    // case of correct index
    if(currbuckets[idx]->key == key){
        return currbuckets[idx]->value;
    }else{
        // case of collision
        Bucket* curr = currbuckets[idx];
        while(curr != nullptr and curr->key != key){
            if(curr->key < key){
                curr = curr->right;
            }else{
                curr = curr->left;
            }
        }
        if(curr != nullptr){return curr->value;}
    }
    return "None";
}

// insert method ** finished **
bool HashTree :: map(int key, string value){
    // mapping index function
    int idx = key % numBuckets;

    // new bucket object
    Bucket* newBucket = new Bucket(key, value);

    // load factor multiplier
    if(loadFactor > 0.75){
        rehash();
    }

    // no collision case
    if(currbuckets[idx] == nullptr){
        currbuckets[idx] = newBucket;
    }else{

        // pointer to current
        Bucket* curr = currbuckets[idx];

        // pointer to previous
        Bucket* prev = nullptr;

        // handling collision case
        while(curr != nullptr){
            prev = curr;
            // duplicates case (update original)
            if(curr-> key == key){
                curr->value = value;
                return true;
            // non duplicates case
            }else if(curr->key < newBucket->key){
                curr = curr->right;
            }else{
                curr = curr->left;
            }
        }
        if(prev->key < newBucket->key){
            prev->right = newBucket;
            newBucket->parent = prev;
        }else{
            prev->left = newBucket;
            newBucket->parent = prev;
        }
    }

    // incrementing number of elements
    numElements ++;

    // new load factor
    loadFactor = (float)numElements / (float)numBuckets;

    // rebalancing 
    Bucket* curr = newBucket;
    while(curr != nullptr){
        curr->updateHeight();
        Bucket* newRoot = rotate(curr);
        if(newRoot != nullptr and newRoot->parent == nullptr){
            currbuckets[idx] = newRoot;
        }
        curr = curr->parent;
    }
    return true;
}

// helper migration ** finished **
void HashTree :: migration(Bucket*& currIndex, Bucket* migrant){

    // no collision case
    if(currIndex == nullptr){
        currIndex = migrant;
        return;
    }else{

        // pointer to current
        Bucket* curr = currIndex;

        // pointer to previous
        Bucket* prev = nullptr;

        // handling collsion
        while(curr != nullptr){
            if(curr->key < migrant->key){
                prev = curr;
                curr = curr->right;
            }else{
                prev = curr;
                curr = curr->left;
            }
        }
        if(prev->key < migrant->key){
            prev->right = migrant;
            migrant->parent = prev;
        }else{
            prev->left = migrant;
            migrant->parent = prev;
        }
    }
}

// helper dfs algorithm ** finished **
void HashTree :: dfs(Bucket* curr, Bucket** newBuckets, int newNumBuckets){
    if(!curr) return;
    
    // recursion dfs
    dfs(curr->left, newBuckets, newNumBuckets);

    // new mapping
    int idx = curr->key % newNumBuckets;
    migration(newBuckets[idx], curr);

    // recursion dfs
    dfs(curr->right, newBuckets, newNumBuckets);

}

// rehashing method ** finished **
bool HashTree :: rehash(){

    // creating new buckets array
    int newNumBuckets = numBuckets * 2;
    Bucket** newbuckets = new Bucket*[newNumBuckets];
    for(int i = 0; i < newNumBuckets; i++){
        newbuckets[i] = nullptr;
    }
    
    // keeping old buckets array
    Bucket** oldbuckets = currbuckets;

    // initializing algorithm
    for(int i = 0; i < numBuckets; i++){
        Bucket* head = oldbuckets[i];
        dfs(head, newbuckets, newNumBuckets);
    }

    // deleting old array
    delete[] oldbuckets;

    // updating current
    numBuckets = newNumBuckets;
    currbuckets = newbuckets;

    // updating new heights on new array
    for(int i = 0; i < newNumBuckets; i++){
        reupdateHeights(newbuckets[i]);
    }
    return true;
}

// successor method ** finished **
HashTree :: Bucket* HashTree :: Bucket :: successor(){
    Bucket* curr = this;

    // case of no successors
    if(curr->right == nullptr and curr->parent == nullptr){return nullptr;}

    // case of right child
    if(curr->right != nullptr){
        curr = curr->right;
        while(curr->left != nullptr){
            curr = curr->left;
        }
        return curr;
    }

    // case where no right child
    Bucket* par = curr->parent;
    while(par != nullptr and par->left != curr){
        curr = par;
        par = par->parent;
    }
    return par;
}

// remove method ** finished **
bool HashTree :: remove(int key){
    // getting key index
    int idx = key % numBuckets;

    // searching for element
    Bucket* curr = currbuckets[idx];
    while(curr != nullptr and curr->key != key){
        if(curr->key < key){
            curr = curr->right;
        }else{
            curr = curr->left;
        }
    }

    // doesn't exist case
    if(curr == nullptr){return false;}

    // start of balancing
    Bucket* rebalanceStart = nullptr;
   
    // case where it's leaf
    if(curr->left == nullptr and curr->right == nullptr){
        rebalanceStart = curr->parent;
        if(curr == currbuckets[idx]){
            currbuckets[idx] = nullptr;
            delete curr;
        }else{
             if(curr->parent->left == curr){
                curr->parent->left = nullptr;
            }else{
                curr->parent->right = nullptr;
            }
            delete curr;
        }

    // case where one child exists
    }else if(curr->left == nullptr or curr->right == nullptr){
        rebalanceStart = curr->parent;
        if(curr->left == nullptr){
            curr->right->parent = curr->parent;
            if(curr == currbuckets[idx]){currbuckets[idx] = curr->right;}
            delete curr;
        }else if(curr->right == nullptr){
            curr->left->parent = curr->parent;
            if(curr == currbuckets[idx]){currbuckets[idx] = curr->left;}
            delete curr;
        }
    
    // case where both children exists
    }else{
        
        // temporary pointer to successor
        Bucket* temp = curr->successor();
        curr->key = temp->key;
        curr->value = temp->value;

        rebalanceStart = temp->parent;

        // handling successor in case 1 or 2
        if(temp->left == nullptr and temp->right == nullptr){
            // case where successor is a leaf
            if(temp->parent->left == temp){
                temp->parent->left = nullptr;
            }else{
                temp->parent->right = nullptr;
            }
            delete temp;
        }else{
            // case where successor has one child
            Bucket* child = (temp->left != nullptr) ? temp->left : temp->right;
            child->parent = temp->parent;
            if(temp->parent->left == temp){
                temp->parent->left = child;
            }else{
                temp->parent->right = child;
            }
            delete temp;
        }
    }

    // decrementing number of elements
    numElements --;

    // rebalancing 
    Bucket* parent = rebalanceStart;
    while(parent != nullptr){
        parent->updateHeight();
        Bucket* newRoot = rotate(parent);
        if(newRoot != nullptr and newRoot->parent == nullptr){
            currbuckets[idx] = newRoot;
        }
        parent = parent->parent;
    }
    return true;
}

// size method ** finished **
int HashTree :: size(){
    return numElements;
}
