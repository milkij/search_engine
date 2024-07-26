//
// Created by Александр Широков on 25.07.2024.
//

#ifndef SEARCH_ENGINE_TIMER_H
#define SEARCH_ENGINE_TIMER_H
#pragma once
#include <iostream>
#include <ctime>
#include <iomanip>


class Timer {
public:
    Timer();
    ~Timer();
private:
    std::time_t begin_time;
    std::time_t end_time;
    std::time_t delta_time;
};


#endif //SEARCH_ENGINE_TIMER_H
