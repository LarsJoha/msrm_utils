#pragma once

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

namespace msrm_utils {

class Cylinder{
public:
    Cylinder(const Eigen::Vector3d& cp1, const Eigen::Vector3d& cp2, double radius);
    bool contains(const Eigen::Vector3d& p) const;
    void set_cp1(const Eigen::Vector3d& cp1);
    void set_cp2(const Eigen::Vector3d& cp1);
    void set_radius(double radius);

    Eigen::Vector3d get_cp1() const;
    Eigen::Vector3d get_cp2() const;
    double get_radius() const;
    Eigen::Vector3d get_axis() const;
    double get_length() const;
    Eigen::Matrix<double,3,3> get_frame(Eigen::Vector3d p) const;
private:
    void calculate_properties();
    Eigen::Vector3d m_cp1;
    Eigen::Vector3d m_cp2;
    double m_radius;
    double m_length;
};


}
