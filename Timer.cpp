//
// Created by Александр Широков on 25.07.2024.
//

#include "Timer.h"

Timer::Timer() {
    begin_time = std::time(nullptr);
}

Timer::~Timer() {
    end_time = std::time(nullptr);
    delta_time = end_time-begin_time;
    std::tm localtime= *std::localtime(&delta_time);
    std::cout<<"executing time "<<std::put_time(&localtime,"%M min. %S sec.")<<std::endl;
}
