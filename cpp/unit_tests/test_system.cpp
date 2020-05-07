#include "msrm_utils/system.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("is process running","[system]"){
    REQUIRE(process_is_running("unit_tests"));
    REQUIRE(!process_is_running("unit_tests2"));
}

//TEST_CASE("system command","[system]"){
//    std::string result = exec("ifconfig");
//    std::cout<<result<<std::endl;
//}


}
