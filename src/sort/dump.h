#pragma once

#include <vector>
#include <string>

namespace container_size
{
static long long int great =  10000000;
static long long int big =    1000000;
static long long int little = 10000;
} // container_size

void set_random(std::string& s);

std::shared_ptr<std::vector<std::string >> get_test_vector(long long int size);
