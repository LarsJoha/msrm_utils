#include "cpp_utils/json.hpp"

namespace cpp_utils {

bool find_json_value(const nlohmann::json& json, const std::string &key){
    try{
        if(json.contains(key)){
            return true;
        }
    }catch(nlohmann::detail::parse_error& e){
        std::cout<<e.what()<<std::endl;
    }
    return false;
}

bool overwrite_valid_json(const nlohmann::json &source, nlohmann::json &sink){
    if(sink.empty()){
        sink=source;
        return true;
    }
    if(source.size()!=sink.size()){
        //        return false;
    }
    if(source.is_object()){
        //        return overwrite_valid_json()
    }else if(source.is_array()){
        sink=nlohmann::json();
        for(unsigned i=0;i<source.size();i++){
            sink.push_back(source[i]);
        }
    }else{
        sink=source;
    }
    return true;
}


}
