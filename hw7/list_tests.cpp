#include "ListContainer.hpp"

#include <gtest/gtest.h>
#include <iostream>

// 1. check the creation of the container
TEST(List, initialize_container) {
    //Arrange
    ListContainer<int> test;
    size_t expected{0};

    std::cout << "TEST #1: check the creation of the container" << std::endl;
    ASSERT_EQ(test.size(), expected);
}

// 2. insertion of the element to the end of the container
TEST(List, push_back) {
    // Arrange
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({1,2,3,4,5});

    // Act
    test.push_back(5);

    std::cout << "TEST #2: check the creation of the container" << std::endl;
    EXPECT_EQ(test,expected);
}

// 3. insertion of the element to the start of the container 
TEST(List, first_insert) {
    // arrange 
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({0,1,2,3,4});

    // act
    test.insert(0,0);

    // assert
    std::cout << "TEST #3: insertion of the element to the start of the container" << std::endl;
    EXPECT_EQ(test, expected);
}

// 4. insertion of the element to the middle of the container 
TEST(List, middle_insert) {
    // arrange 
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({1,2,5,3,4});

    // act
    test.insert(5,2);

    // assert
    std::cout << "TEST #4: insertion of the element to the middle of the container" << std::endl;
    EXPECT_EQ(test, expected);
}

// 5. delete element from the end
TEST(List, last_delete) {
    // arrange 
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({1,2,3});

    // act
    test.erase(3);

    // assert
    std::cout << "TEST #5: delete element from the end" << std::endl;
    EXPECT_EQ(test, expected);
}

// 6. delete element from the start
TEST(List, start_delete) {
    // arrange 
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({2,3,4});

    // act
    test.erase(0);

    // assert
    std::cout << "TEST #6: delete element from the first" << std::endl;
    EXPECT_EQ(test, expected);
}

// 7. delete element from the middle
TEST(List, middle_delete) {
    // arrange 
    ListContainer<int> test({1,2,3,4,5});
    ListContainer<int> expected({1,2,4,5});

    // act
    test.erase(2);

    // assert
    std::cout << "TEST #7: delete element from the middle" << std::endl;
    EXPECT_EQ(test, expected);
}

// 8. get an element of the container
TEST(List, element) {
    // arrange
    ListContainer<int> test({1,2,3,4});
    int expected{1};

    //act

    //assert 
    std::cout << "TEST #8: get an element of the container" << std::endl;
    EXPECT_EQ(test[0], expected);
}

// 9. get a size of the container
TEST(List, size) {
    // arrange
    ListContainer<int> test({1,2,3,4});
    size_t expected{4};

    //act

    //assert 
    std::cout << "TEST #9: get a size of the container" << std::endl;
    EXPECT_EQ(test.size(), expected);
}

// 10. change element of the container
TEST(List, change_element) {
    // arrange
    ListContainer<int> test({1,2,3,4});
    ListContainer<int> expected({1,2,3,5});

    //act
    test[3] = 5;

    //assert
    std::cout << "TEST #10: change element of the container" << std::endl;
    EXPECT_EQ(test, expected);
}
