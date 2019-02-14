#pragma once


#include <functional>
#include <chrono>
#include <thread>

void simulate_hard_computation() {
//    std::this_thread::sleep_for(std::chrono::seconds(1));
};

double multiply_return(const double a, const double b) {
    simulate_hard_computation();
    return a * b;
}

void multiply_output(double& res, const double a, const double b) {
    simulate_hard_computation();
    res = a * b;
}

struct MultiplyClass {
    MultiplyClass(double a, double b) : a_(a), b_(b) {}

    double multiply() {
        simulate_hard_computation();
        return a_ * b_;
    }
    double a_, b_;
};

struct MultiplyInvoke {
    double operator()(double a, double b) {
        simulate_hard_computation();
        return a * b;
    }
};


struct Sequence {
    double first = 0;
    int second = 0;
    char third = ' ';
    std::string fourth = "  ";
    std::vector<int> fifth{};

    bool check_until(std::size_t index) {
        bool prev_check = true;
        for (std::size_t i = 1; prev_check && i < 7 && i <= index; i++) {
            switch (i){
                case 1: {
                    prev_check &= this->first - 180.099 < 0.0000001;
                    break;
                }
                case 2: {
                        prev_check &= this->second == -34000;
                    break;
                }
                case 3: {
                    prev_check &= this->third == 'T';
                    break;
                }
                case 4: {
                    prev_check &= this->fourth == "Yfnccc";
                    break;
                }
                case 5: {
                    std::vector<int> pattern{10, 23, 33, 44, 56, 77, 88, 00, 3};
                    prev_check &= this->fifth == pattern;
                    break;
                }
            }

        }
        return prev_check;
    }

    double first_element() {
        this->first = 180.099;
        return this->first;
    }

    int second_element() {
        if (!this->check_until(1)) { throw "Нарушана последовательность"; }
        this->second = -34000;
        return this->second;
    }

    char third_element() {
        if (!this->check_until(2)) { throw "Нарушана последовательность"; }
        this->third = 'T';
        return this->third;
    }

    std::string& forth_element() {
        if (!this->check_until(3)) { throw "Нарушана последовательность"; }
        this->fourth = "Yfnccc";
        return this->fourth;
    }

    std::vector<int>& fifth_element() {
        if (!this->check_until(4)) { throw "Нарушана последовательность"; }
        this->fifth = {10, 23, 33, 44, 56, 77, 88, 00, 3};
        return this->fifth;
    }

};