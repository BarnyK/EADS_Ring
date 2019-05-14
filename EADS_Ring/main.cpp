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
    Ring<int> rin;
    rin.push_back(1);
    rin.push_back(2);
    rin.push_back(3);
    rin.push_back(4);
    rin.push_back(5);
    rin.push_back(6);
    rin.push_back(2);
    rin.insert_after(2, 1, 1);
    Ring<int>::Iterator f = rin.begin();
    (*f).print();
    f++;
    (*f).print();
    f--;
    (*f).print();
    std::cout << std::endl;
    
    Ring<int> kap(rin);
    kap.print();
    std::cout << std::endl;
    kap.remove_all(2);
    kap.print();
    std::cout << std::endl;
    //kap.erase();
    kap.test();
    
}
