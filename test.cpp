#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "range.hpp"

TEST_CASE("test provided : [2, 5], [8, 12], [1, 4], [7, 9] = [5, 7]") {
    std::vector<range> ranges = {{2, 5},
                                 {8, 12},
                                 {1, 4},
                                 {7, 9}};
    REQUIRE (range(5, 7) == fromVector(ranges).max());
}

TEST_CASE("test provided : [2, 6], [8, 12], [1, 4], [6, 9] = [0, 0]") {
    std::vector<range> ranges = {{2, 6},
                                 {8, 12},
                                 {1, 4},
                                 {6, 9}};
    REQUIRE (range{0, 0} == fromVector(ranges).max());
}

TEST_CASE("[2, 5], [3, 4] = [0, 0]") {
    std::vector<range> ranges = {{2, 5},
                                 {3, 4}};
    REQUIRE (range{0, 0} == fromVector(ranges).max());
}

TEST_CASE("[2, 5], [20, 24], [8, 9] = [9, 20]") {
    std::vector<range> ranges = {{2,  5},
                                 {20, 24},
                                 {8,  9}};
    REQUIRE (range{9, 20} == fromVector(ranges).max());
}

TEST_CASE("[2, 9], [14, 16], [21, 23] = [9, 14], [16, 21]") {
    std::vector<range> ranges = {{2,  9},
                                 {14, 16},
                                 {21, 23}};
    REQUIRE (range{9, 14} == fromVector(ranges).max());
}

TEST_CASE("[2, 9], [14, 16], [21, 23] = [9, 14], [1, 50]") {
    std::vector<range> ranges = {{2,  9},
                                 {14, 16},
                                 {21, 23},
                                 {1,  50}};
    REQUIRE (range{0, 0} == fromVector(ranges).max());
}

TEST_CASE("[2, 5], [0, 1] = [0, 0]") {
    std::vector<range> ranges = {{2, 5},
                                 {0, 1}};
    REQUIRE (range{0, 0} == fromVector(ranges).max());
}