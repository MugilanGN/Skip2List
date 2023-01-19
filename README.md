# Skip2List

An augmented skiplist data structure that adapts to the probability distribution of the queried data. It uses guard entries to "skip the skiplist", hence the name.

**Paper Abstract:**

There has recently been an increased interest in developing distribution adaptive data structures, which use the frequency distribution of the queried data as a measure by which to optimize their operations.

In this paper, we present a design for an adaptive data structure called the skip2list. The skip2list uses a novel algorithm to determine optimally positioned guard entries, which are used to improve the worst-case time complexity of a skip list search from O(n) to O(√n):

Existing skip lists can be augmented without altering their underlying implementations. We first describe the structure of the skip2list, before ex- plaining our guard entry selection algorithm. We then provide experimental results testing the skip2list and the guard entry selection algorithm on various data distributions.

https://cfrce.com/images/Skip2List-July2021.pdf
