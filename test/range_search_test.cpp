/*
Copyright (c) 2022 Tada Teruki, All rights reserved.

[ range_search_test.cpp ]
Check that the CurtainRail implementation correctly searches objects.

---** Information **-----------------------------------------------------------------------------------------
This test may get failed even if CurtainRail correctly searchs objects
    because the CurtainRail implementation does not support correctly handling decimal errors of float value.
-------------------------------------------------------------------------------------------------------------
*/

#include "../curtain_rail.hpp"

#include <iostream>
#include <random>
#include <vector>

// Structure to note the test case to handle
// The meaning of each members are described at 'main()'
struct test_case {
    double n, max_value, max_range_movement, max_objects_movement;
    // **preparing**
    //bool objects_insertion;
    //bool objects_deletion;
};

// random value generators
std::random_device rd;
std::default_random_engine eng(rd());
std::uniform_real_distribution<double> distr(0.0, 1.0);

// get_random01: get random value between 0.0 and 1.0
double get_random01(){
    return distr(eng);
}

// range_search_test: main test function
//
//      repeat: How many times the test is conducted
//      search: How many times continuous range searchings are repeated
//      tcase : Test case to handle
template<class KEY, unsigned int DIM>
bool range_search_test(unsigned int repeat, int search, test_case tcase){
    
    // objects: Container of target objects
    std::vector<std::vector<double>> objects = std::vector<std::vector<double>>(tcase.n,std::vector<double>(DIM, 0));
    // range: Container of search range
    std::vector<std::vector<double>> range = std::vector<std::vector<double>>(2,std::vector<double>(DIM, 0));

    // cr_cont: The CurtainRail container
    curtain_rail::container<KEY, DIM> cr_cont;
    // cindex: Container of CurtainRail pointers of objects 
    std::vector<curtain_rail::index<KEY, DIM>> cindex(tcase.n+1);

    // Generate objects, and insert objects to cr_cont;
    for(int i=0;i<int(objects.size());i++){
        for(int d=0;d<DIM;d++){
            objects[i][d] = get_random01()*tcase.max_value;
        }
        cindex[i] = cr_cont.insert(objects[i].begin(), objects[i].end(), i);
    }

    // Genrate initial search ranges
    for(int i=0; i<2; i++){
        for(int d=0;d<DIM;d++){
            range[i][d] = ( i == 0 ? tcase.max_value*0.25:tcase.max_value*0.75 );
        }
    }

    // CurtainRail pointer of search ranges
    auto rpt = cr_cont.new_range_pointer();

    // Search
    for(int r=0;r<search;r++){

        // actual_size: Count how many objects were actually extracted by linear search
        int actual_size = 0;
        for(int i=0;i<int(objects.size());i++){
            bool res = true;
            for(int d=0;d<DIM;d++){
                if(objects[i][d] < range[0][d] || objects[i][d] > range[1][d]){
                    res = false;
                }
            }
            actual_size += res;
        }

        // Move search range for cr_cont
        cr_cont.move_range(rpt, range[0].begin(), range[0].end(), range[1].begin(), range[1].end());

        // cr_cont_size :Count how many objects were extracted by cr_cont
        int cr_cont_size = 0;
        for(auto it=rpt->begin(); it!=rpt->end(); it++){
            cr_cont_size++;
        }
        
        // Compare the value of cr_cont_size with actual_size
        if(cr_cont_size != actual_size){
            std::cout<<"CurtainRail : Invalid objects size"<<std::endl;
            std::cout<<"CurtainRail :"<<cr_cont_size<<", actual:"<<actual_size<<std::endl;
            std::cout<<"-- range --"<<std::endl;
            for(int d=0;d<DIM;d++){
                std::cout<<"["<<d<<"]"<<"0:"<<range[0][d]<<", 1:"<<range[1][d]<<std::endl;
            }
            return false;
        }

        // Move objects
        for(int i=0;i<int(objects.size());i++){
            for(int d=0;d<DIM;d++){
                objects[i][d] += get_random01()*(tcase.max_objects_movement*2-tcase.max_objects_movement);
            }
            cr_cont.move(cindex[i], objects[i].begin(), objects[i].end());
        }

        // Move the search range
        
        for(int d=0;d<DIM;d++){
            for(int i=0; i<2; i++){
                double noise = get_random01()*(tcase.max_range_movement*2-tcase.max_range_movement);
                range[i][d] += noise;
            }
            if(range[0][d] > range[1][d]) std::swap(range[0][d], range[1][d]);
        }

        // Apply new position of the search range to cr_cont
        cr_cont.move_range(rpt, range[0].begin(), range[0].end(), range[1].begin(), range[1].end());
    }

    // repeat tests
    if(repeat > 1) return range_search_test<KEY, DIM>(repeat-1, search, tcase);
    else return true;
}

int main(){

    int A = 100; // How many times the test is conducted
    int B = 300; // How many times continuous range searchings are repeated

    test_case tcase;

    // Number of objects
    tcase.n = 1000;

    // Size of the space
    tcase.max_value = 10000.0;

    // Maximum distance of movement of the search range for once
    tcase.max_range_movement = 10.0;

    // Maximum distance of movement of objects for once
    tcase.max_objects_movement  = 10.0;
    
    //tcase.objects_insertion = false;
    //tcase.objects_deletion = false;

    // Start test
    bool noerror = range_search_test<int, 2>(A, B, tcase);

    if(noerror == true){
        std::cout<<"CurtainRail : No error found"<<std::endl;
    }
}