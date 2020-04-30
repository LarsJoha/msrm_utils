#pragma once

#include <chrono>

namespace msrm_utils {

/*! Scoped timer for quick benchmarking */
template<typename d>class ScopedTimer{
public:
    /**
     * @brief ScopedTimer The constructor starts the timer.
     * @param id An id for the timer.
     */
    ScopedTimer(const std::string &id="default_timer"):
        m_id(id),m_t0(std::chrono::high_resolution_clock::now()){
    }
    /**
     * @brief ~ScopedTimer The destructor automatically stops the timer by calling stop()
     */
    ~ScopedTimer(){
        stop();
    }
    /**
     * @brief stop This method stops the timer and outputs the passed time to the terminal in the specified format.
     * @return Returns the passed time in the specified format.
     */
    double stop(){
        auto t1 = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<d>(t1-m_t0).count();
        std::cout<<"Scoped timer "<<m_id<<" meassured a duration of "<<duration<<".\n";
        return duration;
    }
private:
    std::chrono::high_resolution_clock::time_point m_t0;
    const std::string m_id;
};

}
