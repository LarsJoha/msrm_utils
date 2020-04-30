#include "msrm_utils/geometry.hpp"

namespace msrm_utils {

bool is_in_cylinder(const Eigen::Matrix<double,3,1>& cylinder_point_1, const Eigen::Matrix<double,3,1>& cylinder_point_2, double cylinder_length, const Eigen::Matrix<double,3,1> & test_point){
    Eigen::Matrix<double,3,1> cylinder_point_1_O=cylinder_point_2-cylinder_point_1;
    Eigen::Matrix<double,3,1> test_point_O=test_point-cylinder_point_1;
    double dot_product=cylinder_point_1_O.dot(test_point_O);
    if(dot_product < 0 || dot_product > pow(cylinder_length,2)){
        return false;
    }else{
        return true;
    }
}


}
