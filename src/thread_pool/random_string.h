#pragma once
#include <iterator>
#include <ctime>
#include <memory>
#include <bits/shared_ptr.h>
#include <random>
#include <stack>


void set_random(std::string& s) {
    static const char alphanum[] =
            "0123456789"
            "abcdefghiklmnopqrstuvwxyz"
            "ABSDEFGHIKLMNOPQRSTUVWXYZ";

    double lower_bound = 0;
    double upper_bound = 100000000;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::random_device rd;
    std::mt19937 re(rd());

    for (std::size_t i = 0; i < s.size(); ++i) {
        int rand = std::ceil(unif(re));
        s[i] = alphanum[rand % (sizeof(alphanum) - 1)];
    }
}

long int random_int() {
    double lower_bound = 0;
    double upper_bound = 100000000;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::random_device rd;
    std::mt19937 re(rd());

    return  std::ceil(unif(re));
}

std::shared_ptr<std::vector<std::string >> get_test_vector(long long int size)
{
    auto v = std::make_shared<std::vector<std::string>>(size);
    std::string s(' ', 5);
    for (long long int i = 0; i < size; ++i) {
        set_random(s);
        (*v)[i] = s;
    }
    return v;
}

std::shared_ptr<std::vector<long int>> get_test_vector_int(long long int size)
{
    auto v = std::make_shared<std::vector<long int>>(size);
    for (long long int i = 0; i < size; ++i) {
        (*v)[i] = random_int();
    }
    return v;
}
