# CurtainRail

This git repository is the original documentation of **CurtainRail**, includes experimental implementation for testing.


## About CurtainRail

**CurtainRail** is a spatial KDS(kinetic data structure), based on multiple kinetic sorted lists. 

Tree-structure-based KDSs(like R-tree[Antonin Guttman 1984], kd-tree[Jon Louis Bentley 1975] etc.) give an efficient algorithm for directly searching spatial objects. However, They does not work well when searching neighboring objects continuously. Moreover, when objects move, such structures become more unbalanced by updating structure which results in poor performance.

Today, a majority of traditional KDSs which handle spatial objects are based on tree structures.

CurtainRail is proposed as a linear-structure-based(**"not tree"**) KDS for dealing with above issues of tree-structure-based ones.
This gives an effective and stable method to update structures for handling continuously moving spatial objects, although it cannot  work effectively for direct spatial access like other KDSs. For range searching, this requires **O(ND)** time initially and performs faster when the range moves continuously. If the movement of the search range is small enough, only approximately **O(D)** time is required. <br>(**N**:the number of objects, **D**:the number of dimensions)

<br>

**Figure: Representation of CurtainRail**
<br><img src="https://user-images.githubusercontent.com/69315285/173576003-20d8f60b-8230-4252-8b70-c779448454f6.png" width="70%"/>



## How CurtainRail works

### Components

CurtainRail consists of two types of linear data structures.

 - Sorted doubly-linked-lists on each dimensions (In this documentation, these will be simply called "**list(s)**".) for collecting data<br>
   When one data inserted, the value of the coordinates of each dimension is inserted into each list along with the address of the data.
 - Hash tables for range searching<br>
   It maps the address of each data to integer flags (The flag is initially 0, and the usage is described later).


<br><img src="https://user-images.githubusercontent.com/69315285/185815728-92f98e92-f5d9-4a7c-9676-b0bed6c64184.png" width="50%"/>
<br><img src="https://user-images.githubusercontent.com/69315285/185816140-67ca72b6-7c92-4edd-b3cb-982e4322c177.png" width="50%"/>


### Range search(initial)

CurtainRail initially does range search through below steps.

 1. Insert the two endpoints of each dimension of the search ranges into the lists.
 2. Do sequential search in each dimension and record the results in a hash table. Specifically, find all data between the two endpoints of search range, and increment 1 into the flag corresponding to each data using the hash table. The hash table eventually counts how many dimensions each data has been sequential searched.
 3. Refering the values in the hash table, extract all data that has been sequential searched in all dimensions.

This sequence of steps will satisfies the query. The time complexity will be **O(ND)**.

<br><img src="https://user-images.githubusercontent.com/69315285/185815767-fa29f293-af39-4d7b-8c4e-f8c7b6db759a.png" width="50%"/>
<br><img src="https://user-images.githubusercontent.com/69315285/185816269-eeab38cc-a2e9-4353-a623-e89598ad652a.png" width="50%"/>
<br><img src="https://user-images.githubusercontent.com/69315285/185816215-7f1fdfbd-4ff5-4b4a-92e7-e18d685471df.png" width="50%"/>

### Range search(continuous)

After range searching through above steps, the result can be refered whenever from the values in the hash table. However, if the search range or/and data are moved or transformed, the result can need to update. CurtainRail has method for updating hash tables, which is more efficient than range searching again.

 1. Update the lists based on the new data location. Specifically, in each dimension, update the value of the coordinates of the moved data and re-order them using bubble sort. Then, do sequential search on them and update the flags. By applying it to all the data, the structure .
 2. Insert the two endpoints of each dimension of the new search range into each list.
 3. Focus on one side of the two endpoints of the search range. Count all data between the old endpoint and the new endpoint and extract the flag of each data from the hash table. Then check whether the data is within the sequence of the new search range, and if so, increment 1 to the flag, if not, decrement 1. Do this on the other side. Finally, apply it to all dimensions.
　(4) Remove the old search range endpoints from all lists.

This sequence of steps satisfies the query.

<br><img src="https://user-images.githubusercontent.com/69315285/185815773-b61386e2-0dd0-4968-a8a4-ad603507f590.png" width="50%"/>
<br><img src="https://user-images.githubusercontent.com/69315285/185816229-a4db4d99-7938-445d-90f2-3f521ba57b2e.png" width="50%"/>
<br><img src="https://user-images.githubusercontent.com/69315285/185816231-dd5878ec-2049-4de6-810f-7e8979b38459.png" width="50%"/>

## Time complexity

**N** : the number of objects<br>**D** : the number of dimensions

||Object insertion & deletion|Range search(initial)|Range search(continuous)|Spatial object replacement|
|---|---|---|---|---|
|**best case**|$$O(D)$$|$$O(D)$$|$$O(D)$$|$$O(D)$$|$$O(D)$$|
|**expected case(for appropriate uses)**|$$O(ND)$$|$$O(ND)$$|$$O(D)$$|$$O(D)$$|$$O(D)$$|
|**worst case**|$$O(ND)$$|$$O(ND)$$|$$O(ND)$$|$$O(ND)$$|$$O(ND)$$|

## Performance comparison

We compared the time performance with **Rtree**. <br>
See: **https://github.com/TadaTeruki/CurtainRail-2022/blob/documentation/docs/benchmark.md**


## References

Antonin Guttman. 1984. R-trees: a dynamic index structure for spatial searching. SIGMOD Rec. 14, 2 (June 1984), 47–57. https://doi.org/10.1145/971697.602266

Jon Louis Bentley. 1975. Multidimensional binary search trees used for associative searching. Commun. ACM 18, 9 (Sept. 1975), 509–517. https://doi.org/10.1145/361002.361007

## Licence

The experimental implementation is available under the MIT License.<br>
Copyright (c) 2022 Tada Teruki. All rights reserved.
