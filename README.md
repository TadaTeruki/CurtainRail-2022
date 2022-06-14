# CurtainRail

This git repository is the original documentation of **CurtainRail**, includes implementation for testing.



## About CurtainRail

**CurtainRail** is a spatial KDS(kinetic data structure), based on multiple kinetic sorted lists. 

Tree-structure-based KDSs(like R-tree[Antonin Guttman 1984], kd-tree[Jon Louis Bentley 1975] etc.) give an efficient algorithm for directly searching spatial objects. However, They does not work well when searching neighboring objects continuously. Moreover, when objects move, such structures become more unbalanced by updating structure which results in poor performance.

Today, a majority of traditional KDSs which handle spatial objects are based on tree structures.

CurtainRail is proposed as a linear-structure-based(**"no-tree"**) KDS for dealing with above issues of tree-structure-based ones.
This gives an effective and stable method to update structures for handling continuously moving spatial objects, although it cannot  work effectively for direct spatial access like other KDSs. For range searching, this requires **O(ND)** time initially and performs faster when the range moves continuously. If the movement of the search range is small enough, only approximately **O(D)** time is required. <br>(**N**:the number of objects, **D**:the number of dimensions)
![Representation](https://user-images.githubusercontent.com/69315285/173576003-20d8f60b-8230-4252-8b70-c779448454f6.png)


### How CurtainRail works

(preparing)

### Time complexity

**N** : the number of objects<br>**D** : the number of dimensions

||Object insertion & deletion|Range search(initial)|Range search(continuous)|Spatial object replacement|
|---|---|---|---|---|
|**best case**|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|
|**expected case(for appropriate uses)**|$$ O(ND) $$|$$ O(ND) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|
|**worst case**|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|


### Issues

(preparing)


# References

Antonin Guttman. 1984. R-trees: a dynamic index structure for spatial searching. SIGMOD Rec. 14, 2 (June 1984), 47–57. https://doi.org/10.1145/971697.602266

Jon Louis Bentley. 1975. Multidimensional binary search trees used for associative searching. Commun. ACM 18, 9 (Sept. 1975), 509–517. https://doi.org/10.1145/361002.361007


