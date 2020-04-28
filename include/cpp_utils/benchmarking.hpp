#pragma once

#include <chrono>

namespace msrm_utils {

template<typename d>class ScopedTimer{
public:
    ScopedTimer(const std::string &id="default_timer"):
        m_id(id),m_t0(std::chrono::high_resolution_clock::now()){
    }
    ~ScopedTimer(){
        stop();
    }
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
