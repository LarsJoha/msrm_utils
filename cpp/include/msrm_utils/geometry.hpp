#pragma once

#include <eigen3/Eigen/Core>

namespace msrm_utils {

bool is_in_cylinder(const Eigen::Matrix<double,3,1>& cylinder_point_1, const Eigen::Matrix<double,3,1>& cylinder_point_2, double cylinder_length, const Eigen::Matrix<double,3,1> & test_point);



}
