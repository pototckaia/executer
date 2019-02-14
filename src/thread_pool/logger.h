#pragma once

class logger {
public:
    logger(bool);

    logger(const logger& log_) = default;

    void start();
    void add_task(int size);
    void when(int size);
    void run(int size);
private:
    bool enable;
};

