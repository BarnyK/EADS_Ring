//
//  main.cpp
//  EADS_Ring
//
//  Created by Winky Face on 08/05/2019.
//  Copyright © 2019 Barnaba Krupowicz. All rights reserved.
//
#include <iostream>
#include "templates.hpp"

int main() {
    // additions
    Ring<int> rin;
    rin.push_back(1);
    rin.push_back(2);
    rin.push_back(3);
    rin.push_back(3);
    rin.push_back(6);
    rin.insert_after(3, 4, 1);  // supposed to insert 4 after 2nd occurance of 3
    rin.insert_before(3, 4, 1);
    rin.push_front(0);
    rin.print();
    std::cout << std::endl;
    
    // remove one
    rin.remove(3, 1);
    rin.print();
    std::cout << std::endl;
    
    // remove all
    rin.remove_all(4);
    rin.print();
    std::cout << std::endl;
    
    // Count
    std::cout << "Number of 6s in  ring: " << rin.count(6) << std::endl;
    
    // Shift
    rin.shift_head(2);
    rin.print();
    
    // Find
    if(rin.find(4))
        std::cout << "There is a 4 in this ring" << std::endl;
    else{
        std::cout << "There is no 4 in this ring" << std::endl;
    }
    rin.erase();
     
    Ring<int> x,y;
    rin.push_back(1);
    rin.push_back(2);
    rin.push_back(3);
    rin.push_back(4);
    rin.push_back(5);
    std::cout << "Source" << std::endl;
    rin.print();
    split(rin, x, 1, 3, y, 0, 6);
    std::cout << "First result" << std::endl;
    x.print();
    std::cout << "Second result" << std::endl;
    y.print();
}
