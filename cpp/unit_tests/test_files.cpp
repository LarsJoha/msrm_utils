#include "msrm_utils/files.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

TEST_CASE("file input/output","[files]"){
    std::remove("test_data1.txt");
    std::remove("test_data2.txt");
    std::remove("test_data3.txt");
    std::array<double,10> data1 = {1,2,3,4,5,6,7,8,9,10};
    std::vector<double> data2 = {1,2,3,4,5,6,7,8,9,10};
    double data3=42;
    std::vector<double> data4,data5,data6;
    write_data_to_file<double,10>(data1,"test_data1.txt",true);
    write_data_to_file<double,10>(data1,"test_data1.txt");
    write_data_to_file<double>(data2,"test_data2.txt",true);
    write_data_to_file<double>(data2,"test_data2.txt");
    write_data_to_file<double>(data3,"test_data3.txt");
    write_endl_to_file("test_data3.txt");
    write_data_to_file<double>(data3,"test_data3.txt");
    REQUIRE(check_if_file_exists("test_data1.txt"));
    REQUIRE(check_if_file_exists("test_data2.txt"));
    REQUIRE(check_if_file_exists("test_data3.txt"));
    read_data_from_file("test_data1.txt",data4);
    read_data_from_file("test_data2.txt",data5);
    read_data_from_file("test_data3.txt",data6);
    bool check_data4,check_data5,check_data6;
    check_data4=check_data5=check_data6=true;
    for(unsigned i=0;i<data4.size();i++){
        if(i<10){
            if(data1[i]!=data4[i]){
                check_data4=false;
                break;
            }
        }else{
            if(data1[i-10]!=data4[i]){
                check_data4=false;
                break;
            }
        }
    }
    for(unsigned i=0;i<data5.size();i++){
        if(i<10){
            if(data2[i]!=data5[i]){
                check_data5=false;
                break;
            }
        }else{
            if(data2[i-10]!=data5[i]){
                check_data5=false;
                break;
            }
        }
    }
    REQUIRE(check_data4);
    REQUIRE(check_data5);
    REQUIRE((data3==data6[0] && data3==data6[1]));
    std::remove("test_data1.txt");
    std::remove("test_data2.txt");
    std::remove("test_data3.txt");
    REQUIRE(!check_if_file_exists("test_data1.txt"));
}

TEST_CASE("String operations","[files]"){
    std::string str="TEST_STRING";
    std::vector<std::string> str_split = split_string(str,"_");
    REQUIRE(str_split.size()==2);
    REQUIRE(str_split[0]=="TEST");
    REQUIRE(str_split[1]=="STRING");

    int str_int = str_to_int(str.c_str());
    REQUIRE(str_int==-2012964615);
}

}
