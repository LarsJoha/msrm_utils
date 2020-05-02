#include "msrm_utils/geometry.hpp"

namespace msrm_utils {

Cylinder::Cylinder(const Eigen::Vector3d &cp1, const Eigen::Vector3d &cp2, double radius):m_cp1(cp1),m_cp2(cp2),m_radius(radius){
    m_length = (m_cp2-m_cp1).norm();
}

bool Cylinder::contains(const Eigen::Vector3d &p) const{
    Eigen::Matrix<double,3,1> cp1_O=m_cp2-m_cp1;
    Eigen::Matrix<double,3,1> p_O=p-m_cp1;
    double dot_product=cp1_O.dot(p_O);
    if(dot_product < 0 || dot_product > pow(m_length,2)){
        return false;
    }else{
        double dsq = (pow(p_O(0),2) + pow(p_O(1),2) + pow(p_O(2),2)) - dot_product*dot_product/pow(m_length,2);

        if(dsq > pow(m_radius,2)){
            return false;
        }
        else{
            return true;
        }
    }
}

void Cylinder::set_cp1(const Eigen::Vector3d &cp1){
    m_cp1=cp1;
    calculate_properties();
}

void Cylinder::set_cp2(const Eigen::Vector3d &cp2){
    m_cp2=cp2;
    calculate_properties();
}

void Cylinder::set_radius(double radius){
    m_radius=radius;
    calculate_properties();
}

void Cylinder::calculate_properties(){
    m_length = (m_cp2-m_cp1).norm();
}

Eigen::Vector3d Cylinder::get_cp1() const{
    return m_cp1;
}

Eigen::Vector3d Cylinder::get_cp2() const{
    return m_cp2;
}

double Cylinder::get_radius() const{
    return m_radius;
}

Eigen::Vector3d Cylinder::get_axis() const{
    return m_cp2 - m_cp1;
}

double Cylinder::get_length() const{
    return m_length;
}

Eigen::Matrix<double,3,3> Cylinder::get_frame(Eigen::Vector3d p) const{
    Eigen::Vector3d x = p-m_cp1;
    Eigen::Vector3d z = get_axis();
    Eigen::Vector3d y = z.cross(x);
    Eigen::Matrix<double,3,3> frame;
    frame<<x,y,z;
    return frame.transpose();
}

}
