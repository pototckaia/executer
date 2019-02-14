#include <iostream>

#include "catch.hpp"
#include "thread_pool.h"
#include "test_stuff.h"
#include "merge_sort_on_executor.h"
#include "random_string.h"

//TEST_CASE("create task") {
//
//    Task mul(multiply_return, 123.0, 13.0);
//    auto res = mul.get_future();
//    mul.run();
//    double r = res.get();
//    REQUIRE(r == 123 * 13);
//
//    Task t([] () { return multiply_return(123, 1222); });
//    REQUIRE(t.get_future().get() == 123 * 1222);
//}
//
//TEST_CASE("create task with condition") {
//    Task print_hello([]() {
//       simulate_hard_computation();
//       std::cout << "Hello" << std::endl;
//    });
//
//    auto hello = print_hello.get_future();
//    TaskWithCondition print_goodbye(hello.share(), [](){
//       simulate_hard_computation();
//       std::cout << "Bye" << std::endl;
//    });
//
//    REQUIRE(print_goodbye.can_run() == false);
//    print_goodbye.run();
//    print_hello.run();
//
//    REQUIRE(print_goodbye.can_run() == true);
//    print_goodbye.run();
//}
//
//TEST_CASE("is done") {
//    Task task_1(multiply_return, 123, 444);
//    Task task_2([]() -> char { return 'd'; });
//    Task task_3([] () -> std::string { return "dddddd";});
//    Task task_4([] () -> int { return 123; });
//    Task task_5([] () -> std::vector<int> { return {1, 2, 3, 4, 5}; });
//
//    REQUIRE(is_done(task_1.get_future(), task_2.get_future()) == false);
//
//    auto f_3 = task_3.get_future().share();
//    auto f_4 = task_4.get_future().share();
//    auto f_5 = task_5.get_future().share();
//    REQUIRE(is_done(f_3, f_4, f_5) == false);
//    task_3.run();
//    REQUIRE(is_done(f_3) == true);
//    REQUIRE(is_done(f_3, f_4, f_5) == false);
//    task_4.run();
//    REQUIRE(is_done(f_3, f_4) == true);
//    REQUIRE(is_done(f_3, f_4, f_5) == false);
//    task_5.run();
//    REQUIRE(is_done(f_3, f_4, f_5) == true);
//}
//
//TEST_CASE("task with a few conditions") {
//    Task task_1(multiply_return, 123, 444);
//    Task task_2([]() -> char { return 'd'; });
//    Task task_3([] () -> std::string { return "dddddd";});
//    Task task_4([] () -> int { return 123; });
//    Task task_5([] () -> std::vector<int> { return {1, 2, 3, 4, 5}; });
//
//    TaskWithMultiCondition t([] () -> double { return multiply_return(123, 1222); },
//            task_1.get_future(), task_2.get_future(), task_3.get_future(), task_4.get_future(), task_5.get_future());
//    REQUIRE(t.can_run() == false);
//    task_1.run();
//    REQUIRE(t.can_run() == false);
//    task_2.run();
//    REQUIRE(t.can_run() == false);
//    task_3.run();
//    REQUIRE(t.can_run() == false);
//    task_4.run();
//    REQUIRE(t.can_run() == false);
//    task_5.run();
//    REQUIRE(t.can_run() == true);
//    auto fut = t.get_future();
//    t.run();
//    REQUIRE(fut.get() == 123 * 1222);
//}
//
//
//auto sum = [] (double a, double b) -> double {
//    return a + b;
//};
//
//TEST_CASE("execute multiply") {
//    std::vector<double> expected_result;
//    std::vector<std::future<double>> futures;
//    Executor ex(4);
//
//    futures.push_back(ex.execute(multiply_return, 12.0, 45.0));
//    expected_result.push_back(12 * 45);
//
//    futures.push_back(ex.execute([]() -> double { return 123 + 3423; }));
//    expected_result.push_back(123 + 3423);
//
//    futures.push_back(ex.execute(sum, 123, 14555));
//    expected_result.push_back(123 + 14555);
//
//    for (std::size_t i = 19; i < 30; ++i) {
//        futures.push_back(ex.execute(multiply_return, i, 45));
//        expected_result.push_back(i * 45);
//    }
//
//    futures.push_back(ex.execute(std::bind(multiply_return, 4, 7)));
//    expected_result.push_back(4 * 7);
//
//    MultiplyClass m(123, 345);
//    futures.push_back(ex.execute(std::bind(&MultiplyClass::multiply, &m)));
//    expected_result.push_back(123 * 345);
//
//    futures.push_back(ex.execute(&MultiplyClass::multiply, &m));
//    expected_result.push_back(123 * 345);
//
//    futures.push_back(ex.execute(MultiplyInvoke(), 12, 90));
//    expected_result.push_back(12 * 90);
//
//    for (std::size_t i = 0; i < futures.size(); ++i) {
//        REQUIRE(futures[i].get() == expected_result[i]);
//    }
//
//    double res = 0;
//    auto fut = ex.execute(multiply_output, std::ref(res), 123, 45);
//    fut.wait();
//    REQUIRE(res == 123 * 45);
//
//}
//
//TEST_CASE("closure executor") {
//    Executor ex(4);
//
//    auto closure = [&ex] () {
//        std::vector<std::future<double>> futures;
//        for (std::size_t i = 0; i < 30; ++i) {
//            futures.push_back(ex.execute(multiply_return, i, 45));
//        }
//        for (auto &future: futures) {
//            future.wait();
//        }
//    };
//
//    auto closure_fut = ex.execute(closure);
//    closure_fut.wait();
//}
//
//TEST_CASE("executor when") {
//    Executor ex(4);
//    std::packaged_task<int()> task([](){ return 7; });
//    auto f1 = task.get_future().share();
//    auto f2 = ex.when(f1, multiply_return, 120.0, 11.0);
//
//    simulate_hard_computation();
//    REQUIRE(is_done(f2) == false);
//    task();
//    f2.wait();
//    REQUIRE(is_done(f2) == true);
//
//}
//
//TEST_CASE("executor when all") {
//    Executor ex(4);
//    Sequence s;
//    Task t_1([&](){  return s.first_element(); });
//    Task t_2([&](){  return s.second_element(); });
//    Task t_3([&](){  return s.third_element(); });
//    Task t_4([&](){  return s.forth_element(); });
//    auto f_5 = ex.whenAll([&]() { return s.fifth_element(); },
//               t_1.get_future(), t_2.get_future(), t_3.get_future(), t_4.get_future()
//    );
//    REQUIRE(is_done(f_5) == false);
//    t_1.run();
//    REQUIRE(is_done(f_5) == false);
//    t_2.run();
//    REQUIRE(is_done(f_5) == false);
//    t_3.run();
//    REQUIRE(is_done(f_5) == false);
//    t_4.run();
//    f_5.wait();
//    REQUIRE(is_done(f_5) == true);
//}
//
//


TEST_CASE("merge sort string") {

    Executor ex(4);
    auto a_ = get_test_vector(200000);
    auto f = tst::mergeSort(a_->begin(), a_->end(), ex);
    f.get();

    auto s = *a_;
    for (auto iter = 0, iter_next = 1; iter_next != a_->size(); ++iter, ++iter_next) {
        REQUIRE((*a_)[iter] <= (*a_)[iter_next]);
    }

}
