#include "cpp_utils/conversion.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("Conversions are tested","[conversions]"){
    Eigen::Matrix<double,3,2> a;
    a<<1,2,
        3,4,
        5,6;
    std::array<double,6> b = convert_to_array<double,3,2>(a);
    REQUIRE(b.size()==a.rows()*a.cols());
    REQUIRE((b[0]==a(0,0) && b[1]==a(1,0) && b[2]==a(2,0) && b[3]==a(0,1) && b[4]==a(1,1) && b[5]==a(2,1)));
}

}
