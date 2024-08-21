### Practice Segment Tree & Fenwick Tree

##### Segment Tree
- start from index 1 : info range 0 to N-1
- left = index*2 : info range (left index of parent's) to (mid index of parent's)
- right = index*2+1 : info range (mid index of parent's range)+1 to
  (right index of parent's)

##### Fenwick Tree
- start from index 1 : info index 0 of original input
- while N >= (index + Least Significant Digit) append info to (index + LSD)

---
##### Problems
- [x] [1395. Count Number of Teams](https://leetcode.com/problems/count-number-of-teams)
