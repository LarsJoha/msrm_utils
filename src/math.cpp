#include "cpp_utils/math.hpp"

namespace msrm_utils {


Eigen::Matrix<double,3,3> invert_matrix(const Eigen::Matrix<double, 3, 3> &M){
    return M.transpose();
}

Eigen::Matrix<double,4,4> invert_transformation_matrix(const Eigen::Matrix<double, 4, 4> &M){
    Eigen::Matrix<double,3,3> R_inv=M.block<3,3>(0,0).transpose();
    Eigen::Matrix<double,3,1> t_inv=-R_inv*M.block<3,1>(0,3);
    Eigen::Matrix<double,3,4> T_inv_tmp;
    T_inv_tmp<<R_inv,t_inv;
    Eigen::Matrix<double,1,4> last_row;
    last_row<<0,0,0,1;
    Eigen::Matrix<double,4,4> T_inv;
    T_inv<<T_inv_tmp,
            last_row;
    return T_inv;
}

Eigen::Matrix<double,4,4> concatenate_matrix(const Eigen::Matrix<double, 3, 3> R, const Eigen::Matrix<double, 3, 1> v){
    Eigen::Matrix<double,1,4> h;
    h<<0,0,0,1;
    Eigen::Matrix<double,3,4> T_tmp;
    Eigen::Matrix<double,4,4> T;
    T_tmp<<R,v;
    T<<T_tmp,h;
    return T;
}

Eigen::Matrix<double,3,3> eulerRPY_to_mat(double alpha, double beta, double gamma){

    gamma*=M_PI/180.0;
    beta*=M_PI/180.0;
    alpha*=M_PI/180.0;

    Eigen::Matrix<double,3,3> R_z;
    R_z<<1,0,0,0,1,0,0,0,1;
    Eigen::Matrix<double,3,3> R_y;
    R_y<<1,0,0,0,1,0,0,0,1;
    Eigen::Matrix<double,3,3> R_x;
    R_x<<1,0,0,0,1,0,0,0,1;

    if(gamma!=0){
        R_z<<cos(gamma),sin(gamma),0,
                -sin(gamma),cos(gamma),0,
                0,0,1;
    }
    if(beta!=0){
        R_y<<cos(beta),0,-sin(beta),
                0,1,0,
                sin(beta),0,cos(beta);
    }
    if(alpha!=0){
        R_x<<1,0,0,
                0,cos(alpha),sin(alpha),
                0,-sin(alpha),cos(alpha);
    }
    Eigen::Matrix<double,3,3> tmp=R_y*R_x;
    return R_z*tmp;
}

bool is_orthonormal(Eigen::Matrix<double, 3, 3> M){
    for(unsigned i=0;i<3;i++){
        if(fabs(sqrt(pow(M(0,i),2)+pow(M(1,i),2)+pow(M(2,i),2))-1)>1e-3){
            return false;
        }
    }
    Eigen::Matrix<double,3,1> y_d=M.block<3,1>(0,2).cross(M.block<3,1>(0,0));
    for(unsigned i=0;i<3;i++){
        if(fabs(y_d(i)-M(i,1))>1e-3){
            return false;
        }
    }
    return true;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

}
