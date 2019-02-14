#pragma once

#include <thread>
#include <future>
#include <iterator>
#include <algorithm>

#include "dump.h"


namespace tst
{


template <class Iter>
void sort(Iter left, Iter right, std::random_access_iterator_tag) {
    if (std::distance(left, right) <= 1) {
        return;
    }
    auto pivot = *std::next(left, std::distance(left, right)/2);
    Iter middle1 = std::partition(left, right,
                                       [pivot](const auto& em){ return em < pivot; });
    Iter middle2 = std::partition(middle1, right,
                                       [pivot](const auto& em){ return !(em > pivot   ); });

    tst::sort(left, middle1, std::random_access_iterator_tag());
    tst::sort(middle2, right, std::random_access_iterator_tag());

}

template <typename Iter>
void sort(Iter left, Iter right) {
    tst::sort(left, right,
         typename std::iterator_traits<Iter>::iterator_category());
}


template <typename Iter>
void multi_sort(Iter left, Iter right, int count_thread, std::random_access_iterator_tag) {
    if (std::distance(left, right) <= 1) {
        return;
    }

    auto pivot = *std::next(left, std::distance(left, right)/2);
    Iter middle1 = std::partition(left, right,
                                  [pivot](const auto& em){ return em < pivot; });
    Iter middle2 = std::partition(middle1, right,
                                  [pivot](const auto& em){ return !(em > pivot   ); });


    std::future<void> han_1;
    std::future<void> han_2;

    if (count_thread <= 0) {
        tst::sort(left, middle1);
        tst::sort(middle2, right);
    }
    else if (count_thread == 1) {
        han_1 = std::async(
                    std::launch::async,
                    [&] { return tst::sort(left, middle1); }
        );

        tst::sort(middle2, right, std::random_access_iterator_tag());
    }
    else if (count_thread >= 2) {
        count_thread -= 2;
        int thread_1 = count_thread / 2;
        int thread_2 = count_thread - thread_1;

        han_1 = std::async(
                std::launch::async,
                multi_sort<Iter>,
                left, middle1, thread_1, std::random_access_iterator_tag()
        );

        han_2 = std::async(
                std::launch::async,
                multi_sort<Iter>,
                middle2, right, thread_2, std::random_access_iterator_tag()
        );
    }


    if (han_1.valid()) { han_1.get(); }
    if (han_2.valid()) { han_2.get(); }

}

template <typename Iter>
void multi_sort(Iter left, Iter right, int count_thread = 4) {
    std::future<void> hand;

    if (count_thread >= 1) {
        --count_thread;
        hand = std::async(std::launch::async, [&] () {
            tst::multi_sort(left, right, count_thread, typename std::iterator_traits<Iter>::iterator_category()
            );
        });

    } else {
        tst::sort(left, right);
    }
    if (hand.valid()) { hand.get(); }
}



} // tst
