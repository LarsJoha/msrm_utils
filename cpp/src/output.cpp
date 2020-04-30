#include "msrm_utils/output.hpp"

namespace msrm_utils {

void print_critical_error(const std::string& str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;4;31mCRITICAL ERROR: "<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

void print_error(const std::string& str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;31mERROR: "<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

void print_warning(const std::string& str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;33mWARNING: "<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

void print_success(const std::string& str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;32m"<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

void print_info(const std::string& str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;37m"<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

void print_debug(const std::string &str, bool end_line, bool new_line){
    if(new_line){
        std::cout<<std::endl;
    }
    std::cout<<"\033[1;34m"<<str<<"\033[0m";
    if(end_line){
        std::cout<<std::endl;
    }
}

}
