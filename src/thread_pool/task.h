#pragma once


template <typename Future>
bool is_done(Future&& cond_) {
    return cond_.wait_for(std::chrono::microseconds(1)) == std::future_status::ready;
}

template <typename Future_1, typename Future_2, typename ...FuturesTail>
bool is_done(Future_1&& f_1, Future_2&& f_2, FuturesTail&&... tails) {
    return is_done<Future_1>(std::forward<Future_1>(f_1))
           && is_done<Future_2, FuturesTail...>(std::forward<Future_2>(f_2), std::forward<FuturesTail>(tails)...);
}

template<typename Tuple, typename std::size_t...I>
bool invoke_is_done(const Tuple& args, std::index_sequence<I...>) {
    return is_done(std::get<I>(args)...);
}

class BaseTask {
public:
    virtual void run() = 0;
    virtual bool can_run() = 0;
};

template <typename Functor, typename... ArgsType>
class Task : public BaseTask {
public:
    using return_type = typename std::invoke_result<Functor, ArgsType...>::type;
    using task_type = typename std::packaged_task<return_type()>;

    explicit Task(Functor&& f, ArgsType&&... args)
            : task_(std::bind(std::forward<Functor>(f), std::forward<ArgsType>(args)...)) {}

    virtual ~Task() = default;

    Task() = delete;
    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;

    void run() override {
        task_();
    }

    bool can_run() override {
        return true;
    }

    typename std::future<return_type> get_future() { return this->task_.get_future(); }

protected:
    task_type task_;
};


template <typename Future, typename Functor, typename... ArgsType>
class TaskWithCondition : public Task<Functor, ArgsType...> {
public:

    explicit TaskWithCondition(Future&& cond, Functor&& f, ArgsType&&... args)
            : Task<Functor, ArgsType...>(std::forward<Functor>(f), std::forward<ArgsType>(args)...),
              cond_(std::forward<Future>(cond)) {}

    bool can_run() override {
        return is_done(cond_);
    }

    void run() override {
        if (this->can_run()) { Task<Functor, ArgsType...>::run(); }
    }

private:
    Future cond_;
};

template <typename Callback, typename ...Futures>
class TaskWithMultiCondition : public Task<Callback> {
public:
    using return_type = typename std::invoke_result<Callback>::type;

    explicit TaskWithMultiCondition(Callback&& callback, Futures&&... futures)
            : Task<Callback>(std::forward<Callback>(callback)),
              conditions_(std::forward<Futures>(futures)...) {}

    bool can_run() override {
        return invoke_is_done(conditions_, std::index_sequence_for<Futures...>{});
    }

    void run() override {
        if (this->can_run()) { this->task_(); }
    }


private:
    std::tuple<Futures...> conditions_;
};

//template <typename Functor, typename... ArgsType>
//class TaskWithFutureVoid: public Task<Functor, ArgsType...> {
//public:
//
//    explicit TaskWithFutureVoid(std::stack<std::shared_ptr<void>> cond, Functor&& f, ArgsType&&... args)
//            : Task<Functor, ArgsType...>(std::forward<Functor>(f), std::forward<ArgsType>(args)...),
//              cond_(cond) {}
//
//    bool can_run() override {
//        bool is_done_ = true;
//        for (auto &cond: cond_ ) {
//            is_done_ &= is_done(cond);
//        }
//        return is_done_;
//    }
//
//    void run() override {
//        if (this->can_run()) { Task<Functor, ArgsType...>::run(); }
//    }
//
//private:
//    std::stack<std::shared_ptr<void>> cond_;
//};