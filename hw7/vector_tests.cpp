#include "VectorContainer.hpp"

#include <gtest/gtest.h>
#include <iostream>

// 1. check the creation of the container
TEST(Vector, initialize_container) {
    //Arrange
    SequenceContainer<int> vector;
    size_t expected{0};

    std::cout << "TEST #1: check the creation of the container" << std::endl;
    ASSERT_EQ(vector.size(), expected);
}

// 2. insertion of the element to the end of the container
TEST(Vector, push_back) {
    // Arrange
    SequenceContainer<int> vector({1,2,3,4});
    SequenceContainer<int> expected({1,2,3,4,5});

    // Act
    vector.push_back(5);

    std::cout << "TEST #2: check the creation of the container" << std::endl;
    EXPECT_EQ(vector,expected);
}

// 3. insertion of the element to the start of the container 
TEST(Vector, first_insert) {
    // arrange 
    SequenceContainer<int> test({1,2,3,4});
    SequenceContainer<int> expected({0,1,2,3,4});

    // act
    test.insert(0,0);

    // assert
    std::cout << "TEST #3: insertion of the element to the start of the container" << std::endl;
    EXPECT_EQ(test, expected);
}

// 4. insertion of the element to the middle of the container 
TEST(Vector, middle_insert) {
    // arrange 
    SequenceContainer<int> test({1,2,3,4});
    SequenceContainer<int> expected({1,2,5,3,4});

    // act
    test.insert(5,2);

    // assert
    std::cout << "TEST #4: insertion of the element to the middle of the container" << std::endl;
    EXPECT_EQ(test, expected);
}

// 5. delete element from the end
TEST(Vector, last_delete) {
    // arrange 
    SequenceContainer<int> test({1,2,3,4});
    SequenceContainer<int> expected({1,2,3});

    // act
    test.erase(3);

    // assert
    std::cout << "TEST #5: delete element from the end" << std::endl;
    EXPECT_EQ(test, expected);
}

// 6. delete element from the start
TEST(Vector, start_delete) {
    // arrange 
    SequenceContainer<int> test({1,2,3,4});
    SequenceContainer<int> expected({2,3,4});

    // act
    test.erase(0);

    // assert
    std::cout << "TEST #6: delete element from the first" << std::endl;
    EXPECT_EQ(test, expected);
}

// 7. delete element from the middle
TEST(Vector, middle_delete) {
    // arrange 
    SequenceContainer<int> test({1,2,3,4,5});
    SequenceContainer<int> expected({1,2,4,5});

    // act
    test.erase(2);

    // assert
    std::cout << "TEST #7: delete element from the middle" << std::endl;
    EXPECT_EQ(test, expected);
}

// 8. get an element of the container
TEST(Vector, element) {
    // arrange
    SequenceContainer<int> test({1,2,3,4});
    int expected{1};

    //act

    //assert 
    std::cout << "TEST #8: get an element of the container" << std::endl;
    EXPECT_EQ(test[0], expected);
}

// 9. get a size of the container
TEST(Vector, size) {
    // arrange
    SequenceContainer<int> test({1,2,3,4});
    size_t expected{4};

    //act

    //assert 
    std::cout << "TEST #9: get a size of the container" << std::endl;
    EXPECT_EQ(test.size(), expected);
}

// 10. change element of the container
TEST(Vector, change_element) {
    // arrange
    SequenceContainer<int> test({1,2,3,4});
    SequenceContainer<int> expected({1,2,3,5});

    //act
    test[3] = 5;

    //assert
    std::cout << "TEST #10: change element of the container" << std::endl;
    EXPECT_EQ(test, expected);
}
