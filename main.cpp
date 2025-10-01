# include "HashTree.cpp"


// simple implementation
int main(){

    // intialzing data structure
    HashTree ht;

    // mapping elements (meteorite mass (kg) as key and meteorite name (location) as value)
    ht.map(720, "Aarhus");
    ht.map(48, "San Diego");
    ht.map(1914, "Acapulco");

    // retrieving element values on average O(1) and worst case O(logn) lookup 
    cout << "This should be Aarhus: " << ht.find(720) << endl; // O(logn) lookup since it collided with the element below
    cout << "This should be San Diego: " << ht.find(48) << endl; // O(logn) lookup since it collided with the element above
    cout << "This should be Acapulco: " << ht.find(1914) << endl; // O(1) lookup since there's no collisions

    // removing elements
    cout << "This should be true: " << ht.remove(720) << endl; // does exist
    cout << "This should be true: " << ht.remove(1914) << endl; // does exist
    cout << "This should be false: " << ht.remove(99) << endl; // does not exist

    // data structure size
    cout << "This should be size 1: " << ht.size() << endl; // should be 1 element

    return 0;
}