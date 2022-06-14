# CurtainRail

This git repository is documentation of **CurtainRail** for research, includes test implementations.



## About CurtainRail

**CurtainRail** is a spatial KDS(kinetic data structure), based on multiple kinetic sorted lists. 

Tree-structure-based KDSs(like kd-tree, R-tree, Quad tree etc.) give an efficient algorithm for directly searching spatial objects. However, They does not work well when searching neighboring objects continuously. Moreover, when objects move, such structures become more unbalanced by updating structure which results in poor performance. 

Today, a majority of traditional KDSs which handle spatial objects are based on tree structures. 

CurtainRail is proposed as a linear-structure-based(**"non-tree"**) KDS for dealing with issues of tree-structure-based ones.
This gives an effective and stable method to update structures for handling continuously moving spatial objects, although it cannot  work effectively for direct spatial access like other KDSs. For range searching, this requires O(N) time initially and performs faster when the range moves continuously. If the movement of the search range is small enough, only approximately O(1) time is required.
![Representation](https://user-images.githubusercontent.com/69315285/173576003-20d8f60b-8230-4252-8b70-c779448454f6.png)


### How CurtainRail works

(preparing)

### Time complexity


||Object insertion & deletion|Range search(initial)|Range search(continuous)|Spatial object replacement|
|---|---|---|---|---|
|**best**|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|
|**expected (for appropriate cases)**|$$ O(ND) $$|$$ O(ND) $$|$$ O(D) $$|$$ O(D) $$|$$ O(D) $$|
|**worst**|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|$$ O(ND) $$|


### Issues

(preparing)
