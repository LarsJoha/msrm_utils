#pragma once

#include <array>
#include <eigen3/Eigen/Core>
#include <nlohmann/json.hpp>
#include "cpp_utils/output.hpp"

namespace cpp_utils {

/**
 * Appends the given Eigen::Matrix value to the indicated json array.
 * @param[in] paramJ Json value.
 * @param[in] param Eigen::Matrix value that is appended to the json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T, std::size_t S1,std::size_t S2> bool append_json_array(nlohmann::json& paramJ, const Eigen::Matrix<T,S1,S2>& param){
    try{
        for(unsigned i=0;i<param.cols();i++){
            for(unsigned j=0;j<param.rows();j++){
                paramJ.push_back(param(j,i));
            }
        }
        return true;
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
}

/**
 * Appends the given std::array value to the indicated json array.
 * @param[in] paramJ Json value.
 * @param[in] param std::array value that is appended to the json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T, std::size_t S> bool append_json_array(nlohmann::json& paramJ, const std::array<T,S>& param){
    try{
        for(unsigned i=0;i<param.size();i++){
            paramJ.push_back(param[i]);
        }
        return true;
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
}

/**
 * Converts the given Eigen::Matrix value to a json array.
 * @param[in] paramJ Json value.
 * @param[in] param Eigen::Matrix value that is converted to a json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T, std::size_t S1,std::size_t S2> bool write_json_array(nlohmann::json& paramJ, const Eigen::Matrix<T,S1,S2>& param){
    paramJ.clear();
    try{
        for(unsigned i=0;i<param.cols();i++){
            for(unsigned j=0;j<param.rows();j++){
                paramJ.push_back(param(j,i));
            }
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Converts the given std::array value to a json array.
 * @param[in] paramJ Json value.
 * @param[in] param std::array value that is converted to a json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T, std::size_t S> bool write_json_array(nlohmann::json& paramJ, const std::array<T,S>& param){
    paramJ.clear();
    try{
        for(unsigned i=0;i<param.size();i++){
            paramJ.push_back(param[i]);
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Converts the given std::vector value to a json array.
 * @param[in] paramJ Json value.
 * @param[in] param std::vector value that is converted to a json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T> bool write_json_array(nlohmann::json& paramJ, const std::vector<T>& param){
    paramJ.clear();
    try{
        for(unsigned i=0;i<param.size();i++){
            paramJ.push_back(param[i]);
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Converts the indicated json value into an Eigen::Matrix.
 * @param paramJ Json value to read from.
 * @param param Target Eigen::Matrix type. Has to be of matching size with the json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T> bool read_json_param(const nlohmann::json& paramJ, const std::string key, T& param){
    try{
        if(!paramJ.contains(key)){
            return false;
        }
        if(paramJ.is_null()){
            return false;
        }
        paramJ[key].get_to(param);
    }catch(const nlohmann::detail::parse_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }catch(const nlohmann::detail::exception& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Converts the indicated json value into an Eigen::Matrix.
 * @param paramJ Json value to read from.
 * @param param Target Eigen::Matrix type. Has to be of matching size with the json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T,std::size_t S1,std::size_t S2> bool read_json_param(const nlohmann::json& paramJ, const std::string key, Eigen::Matrix<T,S1,S2>& param){
    try{
        if(!paramJ.contains(key)){
            return false;
        }
        if(paramJ[key].is_null() || paramJ[key].empty()){
            return false;
        }
        if(paramJ[key].size()!=param.rows()*param.cols()){
            print_error("Can not copy json parameter, expected size ("+std::to_string(param.rows()*param.cols())
                        +") is different from actual one ("+std::to_string(paramJ.size())+").");
            return false;
        }
        for(unsigned i=0;i<param.cols();i++){
            for(unsigned j=0;j<param.rows();j++){
                paramJ[key][i*(unsigned)param.rows()+j].get_to(param(j,i));
            }
        }
    }catch(const nlohmann::detail::parse_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }catch(const nlohmann::detail::exception& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Converts the indicated json value into an std::array.
 * @param paramJ Json value to read from.
 * @param param Target std::array type. Has to be of matching size with the json value.
 * @return True if operation is successful, false otherwise.
 */
template<typename T> bool read_json_param(const nlohmann::json& paramJ, const std::string key, std::vector<T>& param){
    try{
        if(!paramJ.contains(key)){
            return false;
        }
        if(paramJ[key].is_null() || paramJ[key].empty()){
            return false;
        }
        param.resize(paramJ[key].size());
        for(unsigned i=0;i<paramJ[key].size();i++){
            paramJ[key][i].get_to(param[i]);
        }
    }catch(const nlohmann::detail::parse_error& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }catch(const nlohmann::detail::exception& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
    return true;
}

/**
 * Searches for a specific key in a given nlohmann::json object. Note that sub elements of the json object are not searched.
 * @param json Json value in which to search.
 * @param key Key to search for.
 * @return Returns true if key is found, false otherwise.
 */
bool find_json_value(const nlohmann::json &json, const std::string& key);

bool overwrite_valid_json(const nlohmann::json& source, nlohmann::json& sink);

}
