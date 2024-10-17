//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "assert.h"
#include <string>
class Timer {

public:

    Timer() : status(TimerStatus::off), time_cost_ms_(0) {}

    enum class TimerStatus {
        on, off
    };

    void StartTimer() {
        assert(status == TimerStatus::off);
        ResetTime();
        status = TimerStatus::on;
        time_start_ = std::chrono::steady_clock::now();

    };

    
    double StopTimer_ms() {
        assert(status == TimerStatus::on);
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        status = TimerStatus::off;
        time_cost_ms_ = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start_);
        return (double) time_cost_ms_.count() / 1000000;
    }
    double StopTimer_s() {
        assert(status == TimerStatus::on);
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        status = TimerStatus::off;
        time_cost_ms_ = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start_);
        return (double) time_cost_ms_.count() / 1000000000;
    }

    double StopTimer(std::string timelevel) {
        assert(status == TimerStatus::on);
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        status = TimerStatus::off;
        time_cost_ms_ = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start_);
        if(timelevel=="ns") return CountTime();
        else if(timelevel=="ms") return CountMilliseconds();
        else if(timelevel=="s") return CountSeconds();
        else if(timelevel=="us") return CountUs();
        else return -1;
    }


    double CountTime() {
//        return (double)time_cost_ms_.count()*std::chrono::milliseconds::period::num/std::chrono::milliseconds::period::den;
        return (double) time_cost_ms_.count();
//        return time_cost_ms_.count();
    }

    double CountSeconds() {
        return (double) time_cost_ms_.count() / 1000000000;
    }
    double CountUs() {
        return (double) time_cost_ms_.count() / 1000;
    }
    double CountMilliseconds() {
        return (double) time_cost_ms_.count() / 1000000;
    }

    void ResetTime() {
        time_cost_ms_ = std::chrono::nanoseconds(0);
    }

private:
    TimerStatus status;
    std::chrono::steady_clock::time_point time_start_;
    std::chrono::nanoseconds time_cost_ms_;
};


#endif //TIMER_HPP
