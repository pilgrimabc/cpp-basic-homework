#include "ListContainer.hpp"
#include "VectorContainer.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(Vector, copy) {
    // TEST copy assignment
    // Arrange
    SequenceContainer<int> vector1({1,2,3,4});

    //act
    SequenceContainer<int> vector2 = vector1;
    
    // assert
    std::cout << "VECTOR TEST: copy assignment\n" << std::endl;
    EXPECT_EQ(vector1, vector2);

    // TEST copy constructor
    SequenceContainer<int> vector3({1,2,3,4});
    SequenceContainer<int> vector4({5,6,7,8});
    
    // act 
    vector3 = vector4;

    //assert
     std::cout << "VECTOR TEST: copy constructor\n" << std::endl;
    EXPECT_EQ(vector3, vector4);
}

TEST(List, copy) {
    // TEST copy assignment
    // Arrange
    ListContainer<int> list1({1,2,3,4});

    //act
    ListContainer<int> list2 = list1;

    // assert
    std::cout << "LIST TEST: copy assignment\n" << std::endl;
    EXPECT_EQ(list1, list2);

    // TEST copy constructor
    ListContainer<int> list3({1,2,3,4});
    ListContainer<int> list4({5,6,7,8});
    
    // act 
    list3 = list4;

    //assert
     std::cout << "LIST TEST: copy constructor\n" << std::endl;
    EXPECT_EQ(list3, list4);
}