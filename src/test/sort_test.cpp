#include <iostream>

#include "catch.hpp"

#include "dump.h"
#include "sort.h"

auto v_l = get_test_vector(container_size::little);

TEST_CASE("single sort little")
{
    std::vector<int> b = {-966, 5, -10, -15, 20, -15, -25, -1, 0, -966, 50};
    tst::sort(b.begin(), b.end());
    for (auto p = b.begin(), i = p + 1; i != b.end(); ++i, ++p) {
        REQUIRE((*p) <= (*i));
    }

    tst::sort(v_l->begin(), v_l->end());
    for (auto p = v_l->begin(), i = p + 1; i != v_l->end(); ++i, ++p) {
        REQUIRE((*p) <= (*i));
    }
}

TEST_CASE("multi sort little")
{
    tst::multi_sort(v_l->begin(), v_l->end(), 10);
    for (auto p = v_l->begin(), i = p + 1; i !=  v_l->end(); ++i, ++p) {
        REQUIRE((*p) <= (*i));
    }
}


auto v_b = get_test_vector(container_size::big);

TEST_CASE("single sort big")
{
    tst::sort(v_b->begin(), v_b->end());
    for (auto p = v_b->begin(), i = p + 1; i != v_b->end(); ++i, ++p) {
        REQUIRE((*p) <= (*i));
    }
}

TEST_CASE("multi sort big")
{
    tst::multi_sort(v_b->begin(), v_b->end(), 10);
    for (auto p = v_b->begin(), i = p + 1; i != v_b->end(); ++i, ++p) {
        REQUIRE((*p) <= (*i));
    }
}
