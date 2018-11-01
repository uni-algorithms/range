#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "range.hpp"

TEST_CASE("test provided : [2, 5], [8, 12], [1, 4], [7, 9] = [5, 7]") {
    coverage cov(range{2, 5});
    cov.addAll({range{8, 12}, range{1, 4}, range{7, 9}});
    REQUIRE (range{5, 7} == cov.max());
}

TEST_CASE("test provided : [2, 6], [8, 12], [1, 4], [6, 9] = [0, 0]") {
    coverage cov(range{2, 6});
    cov.addAll({range{8, 12}, range{1, 4}, range{6, 9}});
    REQUIRE (range{0, 0} == cov.max());
}

TEST_CASE("[2, 5], [3, 4] = [0, 0]") {
    coverage cov(range{2, 5});
    cov.addAll({range{2, 5}, range{3, 4}});
    REQUIRE (range{0, 0} == cov.max());
}

TEST_CASE("[2, 5], [20, 24], [8, 9] = [9, 20]") {
    coverage cov(range{2, 5});
    cov.addAll({range{20, 24}, range{8, 9}});
    REQUIRE (range{9, 20} == cov.max());
}

TEST_CASE("[2, 9], [14, 16], [21, 23] = [9, 14], [16, 21]") {
    coverage cov(range{2, 9});
    cov.addAll({range{14, 16}, range{21, 23}});
    REQUIRE (range{9, 14} == cov.max());
}

TEST_CASE("[2, 5], [0, 1] = [1, 2]") {
    coverage cov(range{2, 5});
    cov.addAll({range{0, 1}});
    REQUIRE (range{1, 2} == cov.max());
}