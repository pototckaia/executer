#pragma once

#include <stack>
#include "thread_pool.h"


namespace tst {

template <typename Iter>
void mergeSort_simple(Iter left, Iter right) {
    if (std::distance(left, right) <= 1) {
        return;
    }
    auto middle = std::next(left, std::distance(left, right) / 2);

    mergeSort_simple(left, middle);
    mergeSort_simple(middle, right);

    std::merge(left, middle, middle, right, left);
}


template <typename Iter>
void iterative_mergeSort(Iter left, Iter right) {
    int n = std::distance(left, right);

    for (int m = 1; m <= n; m *= 2) {
        for (int i = 0; i < n - m; i += m * 2) {

            std::inplace_merge(
                    left + i,
                    left + i + m,
                    left + std::min(i + m * 2, n)
            );
        }
    }
}


template <typename T>
std::future<void> mergeSort(std::shared_ptr<std::vector<T>> array,  Executor& ex, int depth = 0) {
    if (array->size() == 1) {
        Task task([]{ return; });
        return task.get_future();
    }

    auto middle = std::next(array->begin(), array->size() / 2);

    auto left  = std::make_shared<std::vector<T>>(array->begin(), middle);
    auto right = std::make_shared<std::vector<T>>(middle, array->end());

    auto left_part  = [left,  &ex, depth] () { return mergeSort(left, ex, depth + 1); };
    auto right_part = [right, &ex, depth] () { return mergeSort(right, ex, depth + 1); };

    auto merge = [left, right, array] () mutable {
        std::merge(left->begin(), left->end(), right->begin(), right->end(), array->begin());
    };

    auto f_1 = left_part();
    auto f_2 = right_part();

    return ex.whenAll(merge, f_1.share(), f_2.share());

}

template <typename T>
std::future<void> mergeSort(T left, T right,  Executor& ex, int depth = 0) {
    int size = std::distance(left, right);

    if (size <= 1) {
        std::packaged_task<void()> task([]{ return; });
        task();
        return task.get_future();
    }

    if (depth > 5) {
        auto task = ex.execute([=]{ mergeSort_simple(left, right); });
        return std::move(task);
    }

    auto middle = std::next(left, size / 2);

    auto left_part  = [left,  middle, &ex, depth] () { return mergeSort(left, middle, ex, depth + 1); };
    auto right_part = [right, middle, &ex, depth] () { return mergeSort(middle, right, ex, depth + 1); };

    auto merge = [left, right, middle] () mutable {
        std::merge(left, middle, middle, right, left);
    };



    auto f_1 = left_part();
    auto f_2 = right_part();

    return ex.whenAll(merge, f_1.share(), f_2.share());

}



} // tst
