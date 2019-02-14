#include "thread_pool.h"


Executor::Executor(std::size_t size)
        : m_queue()
{
    logger_.start();

    for (std::size_t i = 0; i < size; ++i) {
        workers.emplace_back([this]() {
            this->thread_wait();
        });
    }
}


Executor::~Executor() {
    this->shut_down();
}

void Executor::shut_down() {
    stop_execute = true;
    cnd_add_task.notify_all();

    for (auto &worker: workers) {
        if (worker.joinable()) { worker.join(); }
    }
}

void Executor::add_task(const std::shared_ptr<BaseTask>& task_ptr) {

    if (stop_execute) { return ; }

    {
        std::unique_lock<std::mutex> locker(m_queue);
        tasks.emplace(task_ptr);

        logger_.add_task(tasks.size());

    }
    cnd_add_task.notify_one();
}

std::shared_ptr<BaseTask> Executor::get_task() {
    std::shared_ptr<BaseTask> task(nullptr);

    {
        std::unique_lock<std::mutex> locker(m_queue);

        while(!stop_execute && tasks.empty()) {
            cnd_add_task.wait(locker);
        }

        if (stop_execute) {
            return nullptr;
        }

        task = tasks.front();
        tasks.pop();
    }

    return task;
}

void Executor::thread_wait() {
    while(true)
    {
        std::shared_ptr<BaseTask> task = this->get_task();
        if (task == nullptr) { return; }

        if (task->can_run()) {

            {
                std::unique_lock<std::mutex> locker(m_queue);
                logger_.run(tasks.size());
            }


            task->run();
        } else {

            {
                std::unique_lock<std::mutex> locker(m_queue);
                logger_.when(tasks.size());
            }

            this->add_task(task);
        }
    }
}
