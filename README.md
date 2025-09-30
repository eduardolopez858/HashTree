# HashTree
## Abstract
#### “The only thing that grows faster than computer performance is expectation.” – Bjarne Stroustrup.

#### In computer science, new data structures are often adopted rapidly because they promise performance improvements. One of the most widely used is the Hash Map, which excels in average-case efficiency. Its fundamental operations—find, insert, and remove—can all be performed in O(1) time on average, with space complexity O(n). However, its performance suffers in the worst case, degrading to O(n) due to hash collisions.

#### Traditional collision resolution strategies such as open addressing (probing) or separate chaining with linked lists do little to mitigate this issue. Linked lists, in particular, are inefficient for repeated lookups. This raises an important question: what if we could preserve the O(1) average-case behavior of hash maps, but guarantee a better worst-case bound?

#### The idea is to replace the linked list traditionally used in separate chaining with a balanced binary search tree, specifically an AVL tree. This ensures that in the rare event of a collision, the operations degrade only to O(log n) instead of O(n). In effect, this hybrid approach combines the strengths of hash maps and AVL trees, achieving O(1) average performance with O(log n) guaranteed in the worst case.
