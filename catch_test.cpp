//
//  main.cpp
//  project2
//
//  Created by Evan Waxman on 12/5/17.
//  Copyright © 2017 Evan Waxman. All rights reserved.
//

#include <stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include "BSTLEAF.h"

using namespace cop3530;

/******************************************
 *  compare
 *****************************************/
bool compare(const int& key1, const int& key2) {
    if (key1 < key2) {
        return true;
    }
    return false;
}

/******************************************
 *  is_equal
 *****************************************/
bool is_equal(const int& key1, const int& key2) {
    if (key1 == key2) {
        return true;
    }
    return false;
}

TEST_CASE("Insert components into tree") {
    BSTLEAF<int, char, compare, is_equal>* map = new BSTLEAF<int, char, compare, is_equal>;

    map->insert(5,'w');
    REQUIRE( map->lookup(5) == 'w');
    
    map->insert(3,'a');
    REQUIRE( map->lookup(3) == 'a');
    
    map->insert(9,'e');
    REQUIRE( map->lookup(9) == 'e');
    
    map->insert(1,'y');
    REQUIRE( map->lookup(1) == 'y');
    
    map->insert(0, 'A');
    REQUIRE( map->lookup(0) == 'A');

    map->insert(10, 'z');
    REQUIRE( map->lookup(10) == 'z');
    
    delete map;
}

TEST_CASE("Remove components from tree") {
    BSTLEAF<int, char, compare, is_equal>* map = new BSTLEAF<int, char, compare, is_equal>;
    
    map->insert(5,'w');
    map->insert(1,'A');
    map->insert(59,'x');
    map->insert(12,'P');
    map->insert(0,'e');
    map->insert(4,'q');
    map->insert(6,'l');
    
    map->remove(5);
    //REQUIRE_THROWS(map->lookup(5));
    
    map->remove(1);
    REQUIRE_THROWS( map->lookup(1));
    
    map->remove(59);
    REQUIRE_THROWS( map->lookup(59));
    
    map->remove(12);
    REQUIRE_THROWS( map->lookup(12));
    
    map->remove(0);
    REQUIRE_THROWS( map->lookup(0));
    
    map->remove(4);
    REQUIRE_THROWS( map->lookup(4));
    
    map->remove(6);
    REQUIRE_THROWS( map->lookup(6));
    
    delete map;
}
