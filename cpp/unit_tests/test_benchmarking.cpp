#include "msrm_utils/benchmarking.hpp"
#include "catch/catch.hpp"
#include <thread>
#include <chrono>

namespace msrm_utils {

TEST_CASE("scoped timer","[benchmarking]"){
    {
        ScopedTimer<std::chrono::seconds> t;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE(t.stop()==1);
    }
}
}
