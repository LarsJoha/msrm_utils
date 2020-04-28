#include "cpp_utils/benchmarking.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("scoped timer","[benchmarking]"){
    {
        ScopedTimer<std::chrono::seconds> t;
        sleep(1);
        REQUIRE(t.stop()==1);
    }
}
}
