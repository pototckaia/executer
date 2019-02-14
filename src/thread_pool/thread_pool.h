#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "task.h"
#include "logger.h"

static logger logger_(false);

class Executor {
public:
    explicit  Executor(std::size_t size = 4);
    ~Executor();

    template <typename F, typename ...Args>
    auto execute(F&& f, Args&&... args);

    template <typename Cond, typename F, typename ...Args>
    auto when(Cond&& cond, F&& f, Args&&... args);

    template <typename Callback, typename ...Conditions>
    auto whenAll(Callback&& f, Conditions&&... args);

//    template <typename F, typename ...Args>
//    auto whenAllRunnable(const std::stack<std::shared_future<void>>& cond,F&& f, Args&&... args);

    void shut_down();

private:
    void thread_wait();

    void add_task(const std::shared_ptr<BaseTask>&);
    std::shared_ptr<BaseTask> get_task();

    std::vector<std::thread> workers;
    std::queue<std::shared_ptr<BaseTask>> tasks; // must be thread-safe

    std::mutex m_queue;
    std::condition_variable cnd_add_task;
    std::atomic_bool stop_execute = false;
};

template <typename F, typename ...Args>
auto Executor::execute(F&& f, Args&&... args)
{
    auto task_ptr = std::make_shared<Task<F, Args...>>(
            std::forward<F>(f),
            std::forward<Args>(args)...
    );

    this->add_task(std::static_pointer_cast<BaseTask>(task_ptr));
    return task_ptr->get_future();
};


template <typename Cond, typename F, typename ...Args>
auto Executor::when(Cond&& cond, F&& f, Args&&... args)
{
    auto task_ptr = std::make_shared<TaskWithCondition<Cond, F, Args...>>(
            std::forward<Cond>(cond),
            std::forward<F>(f),
            std::forward<Args>(args)...
    );

    this->add_task(std::static_pointer_cast<BaseTask>(task_ptr));
    return task_ptr->get_future();

};

template <typename Callback, typename ...Conditions>
auto Executor::whenAll(Callback &&f, Conditions &&... args)
{
    auto task_ptr = std::make_shared<TaskWithMultiCondition<Callback, Conditions...>>(
            std::forward<Callback>(f),
            std::forward<Conditions>(args)...
    );

    this->add_task(std::static_pointer_cast<BaseTask>(task_ptr));
    return task_ptr->get_future();
};

//template <typename F, typename ...Args>
//auto Executor::whenAllRunnable(const std::stack<std::shared_future<void>>& cond,F&& f, Args&&... args)
//{
//    auto task_ptr = std::make_shared<TaskWithFutureVoid<F, Args...>>(
//            cond,
//            std::forward<F>(f),
//            std::forward<Args>(args)...
//    );
//
//    this->add_task(std::static_pointer_cast<BaseTask>(task_ptr));
//    return task_ptr->get_future();
//};
