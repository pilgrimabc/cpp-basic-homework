#include "ListContainer.hpp"
#include "VectorContainer.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <utility>

TEST(Vector, move_assignment) {
    // arrange 
    SequenceContainer<int> vector1({1,2,3,4});
    SequenceContainer<int> vector2({4,5,6,7});
    SequenceContainer<int> expected({1,2,3,4});

    // act 
    vector2 = std::move(vector1);

    // assert 
    std::cout << "VECTOR MOVE ASSIGNMENT TEST" << std::endl;
    EXPECT_EQ(vector2,expected);
    EXPECT_EQ(vector1.size(), 0);
}

TEST(List, move_assignment) {
    // arrange 
    ListContainer<int> list1({1,2,3,4});
    ListContainer<int> list2({4,5,6,7});
    ListContainer<int> expected({1,2,3,4});

    // act 
    list2 = std::move(list1);

    // assert 
    std::cout << "LIST MOVE ASSIGNMENT TEST" << std::endl;
    EXPECT_EQ(list2,expected);
    EXPECT_EQ(list1.size(), 0);
}
