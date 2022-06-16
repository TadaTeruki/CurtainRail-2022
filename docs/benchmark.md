# Documentation: Benchmark test

This document describes the difference of time performance for some practical queries between **CurtainRail and Rtree[*]**.

### Tools

 - **RTree** by nushoin - https://github.com/nushoin/RTree
 - **benchmark** by google - https://github.com/google/benchmark

For performance comparison, we used [**RTree**](https://github.com/nushoin/RTree): the open-source implementation originally published in https://superliminal.com/sources/ and revisioned by [nushoin](https://github.com/nushoin).

And also we used [*benchmark*](https://github.com/google/benchmark) by google for getting and recording results.

### Environment

 - CPU: 11th Gen Intel i7-11390H (8)
 - Memory: 15746MiB

 - OS : Ubuntu 20.04.4 LTS x86_64<br>
    (Kernel: 5.13.0-48-generic)

# Results

For below tasks, **N** 2-dimentional-spatial objects are handled.


### Task 1: Search range movement

This task is for comparing the performance of continuous range search query for **R** times.<br>
The space is 1.0x1.0, and the edges of search range can move up to **M** in one loop.

(In the situation 'no search', range search is never conducted)

**N** = 10,000
**R** = 1,000

|**M**|CurtainRail|R-tree|Linear search| - no search - |
|---|---|---|---|---|
|$$ 10^-2 $$|48459643 ns|1368302 ns|48517314 ns|48115 ns|
|$$ 10^-3 $$|5938341 ns|995358 ns|52124821 ns|48669 ns|
|$$ 10^-4 $$|1358965 ns|1418208 ns|49744170 ns|48139 ns|
|$$ 10^-5 $$|637200 ns|892100 ns|47775982 ns|48366 ns|
|$$ 10^-6 $$|512370 ns|1660001 ns|48358962 ns|48560 ns|
|$$ 10^-7 $$|470312 ns|1012837 ns|48513083 ns|48411 ns|
|$$ 10^-8 $$|500723 ns|1024334 ns|48654319 ns|49004 ns|


### Task 2: Object movement (One object)

This task is for comparing the performance of continuous object movement query for **R** times.<br>
The space is 1.0x1.0, and only one object can move up to **M** in one loop.

**N** = 10,000
**R** = 1,000

|**M**|CurtainRail|R-tree|
|---|---|---|
|$$ 10^-2 $$|1863242 ns|1776582 ns|
|$$ 10^-3 $$|790722 ns|1786134 ns|
|$$ 10^-4 $$|558892 ns|1871878 ns|
|$$ 10^-5 $$|485990 ns|1685690 ns|
|$$ 10^-6 $$|501127 ns|1888049 ns|
|$$ 10^-7 $$|482074 ns|1604196 ns|
|$$ 10^-8 $$|496898 ns|1642682 ns|


### Task 3: Object movement (All objects)

This task is for comparing the performance of continuous object movement query for **R** times.<br>
The space is 1.0x1.0, and the all objects can move up to **M** in one loop.

**N** = 10,000
**R** = 1,000

|**M**|CurtainRail|R-tree|
|---|---|---|
|$$ 10^-2 $$|17218336112 ns|33402585971 ns|
|$$ 10^-3 $$|9484792960 ns|25955768737 ns|
|$$ 10^-4 $$|7914615079 ns|23821428118 ns|
|$$ 10^-5 $$|4909281450 ns|23083391371 ns|
|$$ 10^-6 $$|4682090166 ns|22993216210 ns|
|$$ 10^-7 $$|4789999071 ns|22880343752 ns|
|$$ 10^-8 $$|4667847843 ns|22721168536 ns|


### Task 4: Object movement (One object) +  Search range movement

This task is for comparing the performance of continuous object movement query for **R** times.<br>
The space is 1.0x1.0, and the all objects can move up to **M** in one loop.

**N** = 10,000
**R** = 1,000

(In the situation 'no search', range search is never conducted)

|**M**|CurtainRail|R-tree|Linear search| - no search - |
|---|---|---|---|---|
|$$ 10^-2 $$|52784905 ns|3984470 ns|50704342 ns|95891 ns|
|$$ 10^-3 $$|6701097 ns|2979662 ns|51915613 ns|95778 ns|
|$$ 10^-4 $$|2288734 ns|2635757 ns|49096434 ns|97798 ns|
|$$ 10^-5 $$|1366598 ns|2986545 ns|48709500 ns|96336 ns|
|$$ 10^-6 $$|1327353 ns|3654788 ns|48086565 ns|98665 ns|
|$$ 10^-7 $$|1322267 ns|3586314 ns|47657576 ns|95884 ns|
|$$ 10^-8 $$|1276557 ns|2559748 ns|48639974 ns|96797 ns|


### Task 4: Clustering (DBSCAN)

This task is for comparing the performance of DBSCAN clustering.<br>
The space is 1.0x1.0, and any two objects will be attributed to same class if the distance is less than **DIST**.

**N** = 10,000
**DIST** = 0.005

|CurtainRail|R-tree|Linear search| 
|---|---|---|
|1062963440 ns|5350731 ns|1452299046 ns|


### Assessment

(preparing)