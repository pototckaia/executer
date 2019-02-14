
#include "logger.h"
#include <glog/logging.h>

logger::logger(bool e) :  enable(e)
{
    google::SetLogDestination(google::GLOG_INFO, "logger" );
//    google::InitGoogleLogging("");

}

void logger::start() {
    LOG_IF(INFO, enable)
            << std::endl << std::endl
            << "Executor" << std::endl;
}

void logger::add_task(int size) {
    LOG_IF(INFO, enable)
            << "Задача добавлена. Задач - " << size
            << std::endl;
}

void logger::when(int size) {

    LOG_IF(INFO, enable)
            << " Переброс задачи. Задач - " << size
            << std::endl;
}

void logger::run(int size) {

    LOG_IF(INFO, enable)
            << " Запуск задачи. Задач - " << size
            << std::endl;
}
