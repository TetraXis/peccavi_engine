#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

struct timer
{
public:
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void set_start()
    {
        m_StartTime = std::chrono::system_clock::now();
    }

    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsed_milliseconds() const
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return double(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count());
    }

    double elapsed_seconds() const
    {
        return elapsed_milliseconds() / 1000.0;
    }

    bool is_running() const
    {
        return m_bRunning;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};