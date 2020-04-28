#include "cpp_utils/system.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("executable path","[system]"){
    REQUIRE(process_is_running("unit_tests"));
    REQUIRE(!process_is_running("unit_tests2"));
}

}
