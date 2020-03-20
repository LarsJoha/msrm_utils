#pragma once

#include <array>

#include <eigen3/Eigen/Core>

namespace cpp_utils {

/**
 * Converts an Eigen::Matrix type into an std::array type assuming column major.
 * @param[in] m Variable of Eigen::Matrix type.
 * @return An std::array variable with stacked columns from input m.
 */
template<typename T,std::size_t S1,std::size_t S2> std::array<T,S1*S2> convert_to_array(Eigen::Matrix<T,S1,S2> m){
    std::array<T,S1*S2> d_arr;
    Eigen::Matrix<T,S1,S2>::Map(&d_arr[0], m.rows(),m.cols()) = m;
    return d_arr;
}

/**
 * Converts a scalar value into an std::array type with length 1.
 * @param[in] s Scalar type.
 * @return An std::array variable with a single element containing s.
 */
template<typename T,std::size_t S1,std::size_t S2> std::array<T,S1*S2> convert_to_array(T s){
    std::array<T,1> d_arr={s};
    return d_arr;
}

}
