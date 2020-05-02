#include "msrm_utils/geometry.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {
TEST_CASE("cylinder check","[geometry]"){
    Eigen::Vector3d c_1,c_2,p_in,p_above,p_out1,p_out2,p_frame;
    c_1<<0,0,0;
    c_2<<0,0,1;
    p_in<<0,0,0.5;
    p_above<<0,0,1.5;
    p_out1<<0,1,0.25;
    p_out2<<0,-1,0.25;
    Cylinder c(c_1,c_2,0.5);
    REQUIRE(c.contains(p_in));
    REQUIRE(!c.contains(p_above));
    REQUIRE(!c.contains(p_out1));
    REQUIRE(!c.contains(p_out2));

    p_frame<<0,1,0;
    Eigen::Matrix<double,3,3> frame;
    frame<<0,1,0,-1,0,0,0,0,1;
    REQUIRE(c.get_frame(p_frame)==frame);

    Eigen::Vector3d c_11;
    c_11<<0,0,0.5;
    c.set_cp1(c_11);
    REQUIRE(c.get_length()==0.5);
}

}
