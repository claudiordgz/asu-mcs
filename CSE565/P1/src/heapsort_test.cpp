#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "heapsort.hpp"
#include "rearrange_digits.hpp"

TEST(HeapifyTest, BasicTests) {
    std::vector<int> input1 = {1, 16, 5, 30, 27, 17, 20, 2, 60, 3, 95};
    std::vector<int> expected1 = {95, 60, 20, 30, 27, 17, 5, 2, 1, 3, 16};
    heapify(input1);
    EXPECT_EQ(input1, expected1);

    std::vector<int> input2 = {0, 1, 2};
    std::vector<int> expected2 = {2, 1, 0};
    heapify(input2);
    EXPECT_EQ(input2, expected2);

    std::vector<int> input3 = {};
    std::vector<int> expected3 = {};
    heapify(input3);
    EXPECT_EQ(input3, expected3);

    std::vector<int> input4 = {4, 6, 2, 5, 9, 8};
    std::vector<int> expected4 = {9, 6, 8, 5, 4, 2};
    heapify(input4);
    EXPECT_EQ(input4, expected4);

    std::vector<int> input5 = {1, 2, 3, 4, 5};
    std::vector<int> expected5 = {5, 4, 3, 1, 2};
    heapify(input5);
    EXPECT_EQ(input5, expected5);
}

TEST(HeapSortTest, BasicTests) {
    std::vector<int> arr1 = {3, 7, 4, 6, 1, 0, 9, 8, 9, 4, 3, 5};
    std::vector<int> expected1 = {9, 9, 8, 7, 6, 5, 4, 4, 3, 3, 1, 0};
    heapsort(arr1);
    EXPECT_EQ(arr1, expected1);

    std::vector<int> arr2 = {3, 3, 3, 4, 4, 4, 4, 5, 5, 5};
    std::vector<int> expected2 = {5, 5, 5, 4, 4, 4, 4, 3, 3, 3};
    heapsort(arr2);
    EXPECT_EQ(arr2, expected2);

    std::vector<int> arr3 = {99};
    std::vector<int> expected3 = {99};
    heapsort(arr3);
    EXPECT_EQ(arr3, expected3);

    std::vector<int> arr4 = {0, 1, 2, 5, 12, 21, 0};
    std::vector<int> expected4 = {21, 12, 5, 2, 1, 0, 0};
    heapsort(arr4);
    EXPECT_EQ(arr4, expected4);
}

TEST(HeapSortRearrangeTest, AdvancedTests) {
    std::vector<int> input1 = {1, 2, 3, 4, 5};
    std::pair<int, int> expected1 = {542, 31};
    auto result1 = rearrange_digits(input1, heapsort);
    EXPECT_EQ(result1, expected1);

    std::vector<int> input2 = {4, 6, 2, 5, 9, 8};
    std::pair<int, int> expected2 = {964, 852};
    auto result2 = rearrange_digits(input2, heapsort);
    EXPECT_EQ(result2, expected2);

    std::vector<int> input3 = {0, 1};
    std::pair<int, int> expected3 = {1, 0};
    auto result3 = rearrange_digits(input3, heapsort);
    EXPECT_EQ(result3, expected3);

    std::vector<int> input4 = {0, 1, 2};
    std::pair<int, int> expected4 = {21, 0};
    auto result4 = rearrange_digits(input4, heapsort);
    EXPECT_EQ(result4, expected4);
}
