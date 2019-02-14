#include <benchmark/benchmark.h>
#include <glog/logging.h>
#include <iostream>
#include <random_string.h>

#include "sort.h"
#include "dump.h"
#include "merge_sort_on_executor.h"
#include "thread_pool.h"

int size = 700000;

static void CustomThreads(benchmark::internal::Benchmark* b) {
    b->Args({0});
    for (int i = 2; i <= 9; ++i)
        b->Args({i});
}


static auto test_vector_little = get_test_vector(size);
static void BM_Quick_Sort(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        auto v = test_vector_little;
        state.ResumeTiming();

        if (state.range(0) == 0) {
            std::sort(v->begin(), v->end());
        }
        else {
            tst::multi_sort(v->begin(), v->end(), state.range(0));
        }
    }
}

static void BM_Merge_Sort(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        auto v = test_vector_little;
        state.ResumeTiming();

        if (state.range(0) == 0) {
            tst::mergeSort_simple(v->begin(), v->end());
        }
        else {
            state.PauseTiming();
            Executor ex(state.range(0));
            state.ResumeTiming();

            auto f = tst::mergeSort(v->begin(),v->end(), ex);
            f.get();
        }
    }
}

auto v_ = get_test_vector_int(size);
static void BM_Merge_Sort_int(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        auto v_ = test_vector_little;
        state.ResumeTiming();

        if (state.range(0) == 0) {
            tst::mergeSort_simple(v_->begin(), v_->end());
        }
        else {
            state.PauseTiming();
            Executor ex(state.range(0));
            state.ResumeTiming();

            auto f = tst::mergeSort(v_->begin(), v_->end(), ex);
            f.get();
        }
    }
}

// BENCHMARK(BM_Quick_Sort)->Apply(CustomThreads);
BENCHMARK(BM_Merge_Sort)->Apply(CustomThreads);
BENCHMARK(BM_Merge_Sort_int)->Apply(CustomThreads);


int main(int argc, char** argv)
{

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
