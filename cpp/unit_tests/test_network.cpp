#include "cpp_utils/network.hpp"
#include "catch/catch.hpp"

namespace msrm_utils {

//TEST_CASE("ping","[network]"){
//    REQUIRE(ping("127.0.0.1"));
//    REQUIRE(ping("0.0.0.0"));
//    REQUIRE(ping("localhost"));
//    REQUIRE(!ping("128.0.0.1"));
//}

//TEST_CASE("ip address check","[network]"){
//    REQUIRE(!is_valid_ip_address("1"));
//    REQUIRE(is_valid_ip_address("1.1.1.1"));
//    REQUIRE(is_valid_ip_address("192.168.1.1"));
//    REQUIRE(is_valid_ip_address("192.168.1.0"));
//    REQUIRE(is_valid_ip_address("192.168.1.255"));
//    REQUIRE(!is_valid_ip_address("192.168.1.256"));
//    REQUIRE(!is_valid_ip_address("192.168.1.-1"));
//    REQUIRE(!is_valid_ip_address("192.168.1.a"));
//}

//TEST_CASE("port check","[network]"){
////    REQUIRE(!is_port_open("localhost",65536));
////    REQUIRE(!is_port_open("localhost",0));
//}

//TEST_CASE("own ip","[network]"){
//    REQUIRE(get_own_ip("lo")=="127.0.0.1");
//    REQUIRE(get_own_ip("-lo-").has_value()==false);
//}

//TEST_CASE("get ip by hostname","[network]"){
//    REQUIRE(get_ip_by_hostname("127.0.0.1")=="127.0.0.1");
//    REQUIRE(get_ip_by_hostname("localhost")=="127.0.0.1");
//    REQUIRE(get_ip_by_hostname("localhost2").has_value()==false);
//}

//TEST_CASE("get ip subnets","[network]"){
//    std::map<std::string,std::string> subnets=get_subnets();
//    auto lo = subnets.find("lo");
//    REQUIRE(lo!=subnets.end());
//    REQUIRE((*lo).second=="127.0.0.");
//}

//TEST_CASE("get interfaces","[network]"){
//    std::set<std::string> ifaces = get_ifaces();
//    REQUIRE(ifaces.find("lo")!=ifaces.end());
//}

//TEST_CASE("faulty rpc server","[network]"){
//    JsonRPCServer server("0.0.0.0",443);
//    REQUIRE(!server.start_listening());
//    JsonRPCServer server2("0.0.0.0",100000);
//    REQUIRE(!server2.start_listening());
//}

//TEST_CASE("rpc communication","[network]"){
//    JsonRPCServer server_fail("0.0.0.0",443);
//    REQUIRE(!server_fail.start_listening());
//    JsonRPCServer server("0.0.0.0",10000);
//    server.bind_method("add",[&](const nlohmann::json& request)->nlohmann::json{
//        double a,b;
//        nlohmann::json response;
//        request[0]["a"].get_to(a);
//        request[0]["b"].get_to(b);
//        response["c"]=a+b;
//        return response;
//    },{"request"});
//    REQUIRE(server.start_listening());
//    server.stop_listening();
//    REQUIRE(server.start_listening());
//    nlohmann::json response;
//    nlohmann::json request;
//    request["a"]=4.5;
//    request["b"]=2;
//    JsonRPCClient::call_method("localhost",10000,"add",request,response);
//    double c;
//    response["c"].get_to(c);
//    REQUIRE(c==6.5);
//    REQUIRE(!JsonUDPClient::call_method("191.168.2.2",10000,"add",request,response));
//    REQUIRE(!JsonRPCClient::call_method("localhost2",10000,"add",{request},response));
//    REQUIRE(!JsonRPCClient::call_method("localhost",10000,"add2",{request},response));
//    REQUIRE(!JsonRPCClient::call_method("localhost",10001,"add",{request},response));
//    server.stop_listening();
//}

//TEST_CASE("faulty web socket server","[network]"){
//    JsonWebsocketServer server("0.0.0.0",443,1,"test_server");
//    REQUIRE(!server.start_listening());
//    JsonWebsocketServer server2("0.0.0.0",100000,1,"test_server");
//    REQUIRE(!server2.start_listening());
//}

//TEST_CASE("web socket communication","[network]"){
//    JsonWebsocketServer server("0.0.0.0",10000,1,"test_server");
//    server.bind_method("add",[&](const nlohmann::json& request)->nlohmann::json{
//        double a,b;
//        nlohmann::json response;
//        request["a"].get_to(a);
//        request["b"].get_to(b);
//        response["c"]=a+b;
//        return response;
//    },{"a","b"});
//    REQUIRE(server.start_listening());
//    nlohmann::json response;
//    nlohmann::json request;
//    request["a"]=4.5;
//    request["b"]=2;
//    JsonWebsocketClient::call_method("localhost",10000,"test_server","add",request,response);
//    REQUIRE(response["result"]["c"]==6.5);
//    REQUIRE(!JsonUDPClient::call_method("191.168.2.2",10000,"add",request,response));
//    REQUIRE(!JsonWebsocketClient::call_method("localhost2",10000,"test_server","add",request,response));
//    REQUIRE(!JsonWebsocketClient::call_method("localhost",10000,"test_server","add2",request,response));
//    REQUIRE(!JsonWebsocketClient::call_method("localhost",10001,"test_server","add",request,response));
//    REQUIRE(!JsonWebsocketClient::call_method("localhost",10000,"test_server2","add",request,response));
//    server.stop_listening();
//}

//TEST_CASE("faulty udp server","[network]"){
//    JsonUDPServer server(100000);
//    REQUIRE(!server.start_listening());
//    JsonUDPServer server2(443);
//    REQUIRE(!server2.start_listening());
//}

//TEST_CASE("json udp communication","[network]"){
//    JsonUDPServer server(10000);
//    server.bind_method("add",[&](const nlohmann::json& request)->nlohmann::json{
//        double a,b;
//        nlohmann::json response;
//        request["a"].get_to(a);
//        request["b"].get_to(b);
//        response["c"]=a+b;
//        return response;
//    },{"a","b"});
//    REQUIRE(server.start_listening());
//    nlohmann::json response;
//    nlohmann::json request;
//    request["a"]=4.5;
//    request["b"]=2;
//    JsonUDPClient::call_method("localhost",10000,"add",request,response);
//    REQUIRE(response["result"]["c"]==6.5);
//    REQUIRE(!JsonUDPClient::call_method("191.168.2.2",10000,"add",request,response));
//    REQUIRE(!JsonUDPClient::call_method("localhost2",10000,"add",request,response));
//    REQUIRE(!JsonUDPClient::call_method("localhost",10000,"add2",request,response));
//    REQUIRE(!JsonUDPClient::call_method("localhost",10001,"add",request,response));
//    server.stop_listening();
//}

TEST_CASE("failed udp stream","[network]"){
    UDPStreamSender sender1("localhost2",8888);
    REQUIRE(!sender1.connect());
    UDPStreamSender sender2("localhost",100000);
    REQUIRE(!sender2.connect());
}

TEST_CASE("udp stream","[network]"){
    std::vector<double> data;
    data.resize(0);
    UDPStreamSender sender("localhost",8888);
    UDPStreamReceiver receiver(8888,4096,1,0,20,[&](std::vector<double>& payload)->void{
        std::cout<<"Received: ["<<payload[0]<<","<<payload[1]<<","<<payload[2]<<","<<payload[3]<<","<<payload[4]<<"]\n";
        data=payload;
    });
    REQUIRE(sender.connect());
    REQUIRE(receiver.connect());
    REQUIRE(sender.send({1,2,3,4,5}));
    sleep(1);
    REQUIRE((data[0]==1 && data[1]==2 && data[2]==3 && data[3]==4 && data[4]==5));
}

}
