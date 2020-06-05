/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef TIMER_HPP

#define TIMER_HPP

#include "configure.hpp"

static const double MEGA = 1000000.0;

class Timer
{
#if defined(_WIN32)
private:
    LARGE_INTEGER _start;
    LARGE_INTEGER _end;
    LARGE_INTEGER _frequency;
public:
    Timer() { QueryPerformanceFrequency(&_frequency); }
    inline void start() { QueryPerformanceCounter(&_start); }
    inline double end_s()
    {
        QueryPerformanceCounter(&_end);
        return (double)(_end.QuadPart - _start.QuadPart) / (double)_frequency.QuadPart;
    }
    inline double end_us()
    {
        QueryPerformanceCounter(&_end);
        return (double)(_end.QuadPart - _start.QuadPart) * MEGA / (double)_frequency.QuadPart;
    }

#elif defined(__linux__)
private:
    struct timeval _start;
    struct timeval _end;
public:
    inline void start() { gettimeofday(&_start, NULL); }
    inline double end_s()
    {
        gettimeofday(&_end, NULL);
        return (double)(_end.tv_sec - _start.tv_sec) + 
            (double)(_end.tv_usec - _start.tv_usec) / MEGA;
    }
    inline double end_us()
    {
        gettimeofday(&_end, NULL);
        return (double)(_end.tv_sec - _start.tv_sec) * MEGA + 
            (double)(_end.tv_usec - _start.tv_usec);
    }


#endif
};

#endif