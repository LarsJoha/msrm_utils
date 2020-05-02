#include "msrm_utils/network.hpp"

#include "msrm_utils/files.hpp"
#include "msrm_utils/output.hpp"
#include "msrm_utils/json.hpp"

#include <chrono>
#include <bits/stdc++.h>
#include <cerrno>
#include <cstring>

namespace msrm_utils{

bool ping(const char *ipaddr) {
    char *command = NULL;
    FILE *fp;
    int stat = 0;
    //  asprintf (&command, "%s %s -q 2>&1", "fping", ipaddr);
    int rtn = asprintf (&command, "%s %s -q -r 0 -t 5 2>&1", "fping", ipaddr);
    //    printf("%s\n",command);
    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to execute fping command\n");
        free(command);
        return -1;
    }
    stat = pclose(fp);
    free(command);
    if(WEXITSTATUS(stat)==0){
        return true;
    }else{
        return false;
    }
}

/*  Check if an ip address is valid */
bool is_valid_ip_address(const char *ipaddr){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
    return result != 0;
}

bool is_port_available(const char* host, unsigned port){
    if(port>65535){
        std::cout<<"Port number must be between 0 and 65535"<<std::endl;
        return false;
    }
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return false;
    }

    server = gethostbyname(host);

    if (server == NULL) {
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(port);
    bool open;
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        open=true;
    } else {
        open=false;
    }

    close(sockfd);
    return open;
}

std::optional<std::string> get_own_ip(const char *iface){
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    std::optional<std::string> ip;
    char* interface;

    getifaddrs(&ifAddrStruct);
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            interface=ifa->ifa_name;
            if(strcmp(interface,iface)==0){
                ip.emplace(std::string(addressBuffer));
            }
        } else if (ifa->ifa_addr->sa_family == AF_INET6) {
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            interface=ifa->ifa_name;
            //            if(interface==iface){
            //                ip=std::string(addressBuffer);
            //            }
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return ip;
}

std::optional<std::string> get_ip_by_hostname(const char *hostname){
    hostent * record = gethostbyname(hostname);
    if(record == nullptr)
    {
        return {};
    }
    in_addr * address = (in_addr * )record->h_addr;
    return std::string(inet_ntoa(* address));
}

std::map<std::string,std::string> get_subnets(){
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    std::string ip;
    std::string interface;

    std::map<std::string,std::string> ifaces;

    getifaddrs(&ifAddrStruct);
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            interface=std::string(ifa->ifa_name);
            ip=std::string(addressBuffer);
            std::string ip_tmp=convert_ip_to_default_format(ip);
            ip_tmp=ip_tmp.substr(0,ip_tmp.size()-3);
            ip=convert_ip_from_default_format(ip_tmp);
            ifaces.insert(std::pair<std::string,std::string>(std::string(ifa->ifa_name),ip));
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return ifaces;
}

std::set<std::string> get_ifaces(){
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    std::set<std::string> ifaces;

    getifaddrs(&ifAddrStruct);
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            ifaces.emplace(ifa->ifa_name);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return ifaces;
}


std::string convert_ip_to_default_format(const std::string& ip){
    std::vector<std::string> ip_parts = split_string(ip,".");
    std::string ip_in_format="";
    for(unsigned i=0;i<ip_parts.size();i++){
        if(ip_parts[i].size()==1){
            ip_parts[i].insert(0,"xx");
        }
        if(ip_parts[i].size()==2){
            ip_parts[i].insert(0,"x");
        }
        ip_in_format+=ip_parts[i]+".";
    }
    ip_in_format.erase(ip_in_format.size()-1);
    return ip_in_format;
}

std::string convert_ip_from_default_format(const std::string& ip){
    std::vector<std::string> ip_parts = split_string(ip,".");
    std::string ip_in_format="";
    for(unsigned i=0;i<ip_parts.size();i++){
        if(ip_parts[i][0]=='x' && ip_parts[i][1]=='x'){
            ip_parts[i].erase(0,2);
        }else if(ip_parts[i][0]=='x'){
            ip_parts[i].erase(0,1);
        }
        ip_in_format+=ip_parts[i]+".";
    }
    ip_in_format.erase(ip_in_format.size()-1);
    return ip_in_format;
}

JsonRPCServer::JsonRPCServer(const std::string& address, unsigned port) : m_address(address.c_str()), m_port(port) {
    m_http_server.Post("/", [&](const httplib::Request &req, httplib::Response &res) {
        res.status = 200;
        //        std::cout<<"REQUEST: "<<req.body<<std::endl;
        res.set_content(m_server.HandleRequest(req.body), "application/json");
        //        std::cout<<"RESPONSE: "<<res.body<<std::endl;
    });
}

JsonRPCServer::~JsonRPCServer(){
    this->stop_listening();
}

bool JsonRPCServer::start_listening(){
    if(m_port>65535){
        std::cout<<"Port number must be between 0 and 65535"<<std::endl;
        return false;
    }
    if (m_http_server.is_running())
        return false;
    if(!m_http_server.bind_to_port(m_address.c_str(),m_port,2)){
        return false;
    }
    m_server_thread = std::thread([this]() { m_http_server.listen_after_bind(); });
    std::chrono::time_point t_start = std::chrono::system_clock::now();
    while(!m_http_server.is_running()){
        std::chrono::time_point t_now = std::chrono::system_clock::now();
        double t = std::chrono::duration_cast<std::chrono::seconds>(t_now-t_start).count();
        if(t>5){
            return false;
        }
    }
    return true;
}

void JsonRPCServer::stop_listening(){
    if (m_http_server.is_running()) {
        m_http_server.stop();
        if(m_server_thread.joinable()){
            m_server_thread.join();
        }
    }
}

bool JsonRPCServer::bind_method(const std::string &name, std::function<nlohmann::json (const nlohmann::json &)> method, const std::vector<std::string> &arguments){
    return m_server.Add(name,jsonrpccxx::MethodHandle(method),arguments);
}

JsonRPCClient::JsonRPCClient(const char* host, int port, double timeout) : m_http_client(host, port), m_timeout(timeout),m_rpc_client(*this,jsonrpccxx::version::v2) {
    m_http_client.set_timeout_sec(timeout);
}

std::string JsonRPCClient::Send(const std::string &request){
    m_http_client.set_timeout_sec(m_timeout);
    auto res = m_http_client.Post("/", request, "application/json");
    if (!res || res->status != 200) {
        throw jsonrpccxx::JsonRpcException(-32003, "client connector error, received status != 200");
    }
    return res->body;
}

bool JsonRPCClient::send(const std::string &method, const nlohmann::json &request){
    try{
        m_response = m_rpc_client.CallMethod<nlohmann::json>(0,method,{request});
        return true;
    }catch(const nlohmann::json::exception& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }catch(const jsonrpccxx::JsonRpcException& e){
        std::cout<<e.what()<<std::endl;
        return false;
    }
}

void JsonRPCClient::get_response(nlohmann::json &response){
    response=m_response;
}

bool JsonRPCClient::call_method(const std::string &address, unsigned port, const std::string &method, const nlohmann::json &request, nlohmann::json &response, unsigned timeout){
    JsonRPCClient client(address.c_str(),port,timeout);
    bool result = client.send(method,request);
    client.get_response(response);
    return result;
}

JsonWebsocketServer::JsonWebsocketServer(const std::string& address, unsigned port, const std::string& endpoint){
    m_server.config.address=address;
    m_server.config.port=port;
    m_server.config.thread_pool_size=10;

    auto &echo = m_server.endpoint["^/"+endpoint+"/?$"];

    echo.on_message = [this](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::Connection> connection, std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::InMessage> message_raw) {

        nlohmann::json message, response;
        try{
            message = nlohmann::json::parse(message_raw->string());
            std::pair<bool,std::string> result=this->message_preprocessing(message);
            if(result.first){
                std::string method;
                message["method"].get_to(method);
                if(!this->check_if_method_exists(method)){
                    response["result"]="Method " + method + " not known.";
                    response["error"]=true;
                }else{
                    if(this->check_arguments(message["request"],m_method_arguments[method],response)){
                        response["result"]=m_method_callbacks[method](message["request"]);
                    }else{
                        response["error"]=true;
                    }
                }
            }else{
                response["result"]=result.second;
                response["error"]=true;
            }
        }catch(const nlohmann::detail::type_error& e){
            std::cout<<e.what()<<std::endl;
            response["result"]=e.what();
            response["error"]=true;
        }catch(const nlohmann::detail::parse_error& e){
            std::cout<<e.what()<<std::endl;
            response["result"]=e.what();
            response["error"]=true;
        }
        auto out_message = response.dump();

        // connection->send is an asynchronous function
        connection->send(out_message, [](const SimpleWeb::error_code &ec) {
            if(ec) {
                std::cout << "Server: Error sending message. " <<
                             // See http://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                             "Error: " << ec << ", error message: " << ec.message() << std::endl;
            }
        });
    };

    echo.on_open = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::Connection> connection) {
        //        std::cout << "Server: Opened connection " << connection.get() << std::endl;
    };

    // See RFC 6455 7.4.1. for status codes
    echo.on_close = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::Connection> connection, int status, const std::string & /*reason*/) {
        //        std::cout << "Server: Closed connection " << connection.get() << " with status code " << status << std::endl;
    };

    // Can modify handshake response headers here if needed
    echo.on_handshake = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
        return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.on_error = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WS>::Connection> connection, const SimpleWeb::error_code &ec) {
        std::cout << "Server: Error in connection " << connection.get() << ". "
                  << "Error: " << ec << ", error message: " << ec.message() << std::endl;
    };
}

JsonWebsocketServer::~JsonWebsocketServer(){
    stop_listening();
}

bool JsonWebsocketServer::start_listening(){
    if(!is_port_available("localhost",m_server.config.port)){
        std::cout<<"Port "+std::to_string(m_server.config.port)+" is unavailable."<<std::endl;
        return false;
    }
    m_server_thread = std::thread([this](){
        try{
            m_server.start();
        }catch(const boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e){
            std::cout<<e.what()<<std::endl;
            return;
        }
    });
    return true;
}

void JsonWebsocketServer::stop_listening(){
    m_server.stop();
    sleep(1);
    if(m_server_thread.joinable()){
        m_server_thread.join();
    }
}

std::pair<bool,std::string> JsonWebsocketServer::message_preprocessing(nlohmann::json &message){
    try{
        if(message.find("method")==message.end()) return std::pair<bool,std::string>(false,"Message header does not contain <method>.");
        if(!message["method"].is_string()) return std::pair<bool,std::string>(false,"Method is not a readable string.");
        if(message.find("request")==message.end()){
            message["request"]=nlohmann::json();
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return std::pair<bool,std::string>(false,"Json type error.");
    }
    return std::pair<bool,std::string>(true,"");
}

bool JsonWebsocketServer::check_if_method_exists(const std::string& method){
    if(m_method_callbacks.find(method)==m_method_callbacks.end()){
        return false;
    }else{
        return true;
    }
}

bool JsonWebsocketServer::bind_method(const std::string &name, std::function<nlohmann::json (const nlohmann::json& request)> method, const std::vector<std::string> &arguments){
    if(this->check_if_method_exists(name)){
        msrm_utils::print_error("Cannot bind method with name "+name+" since it already exists.");
        return false;
    }
    m_method_callbacks.insert(std::pair<std::string,std::function<nlohmann::json(const nlohmann::json&)> >(name, method));
    m_method_arguments.insert(std::pair<std::string,std::vector<std::string> >(name,arguments));
    return true;
}

bool JsonWebsocketServer::check_arguments(const nlohmann::json &request, const std::vector<std::string> &arguments, nlohmann::json &response){
    if(!request.is_object() && !request.is_null()){
        response["result"]="Request must be a json object (can be null).";
        return false;
    }
    for(const std::string& a : arguments){
        if(request.find(a)==request.end()){
            response["result"]="Could not find parameter "+a+" in request.";
            return false;
        }
    }
    return true;
}

JsonWebsocketClient::JsonWebsocketClient(const std::string& address, unsigned port, const std::string& endpoint):m_client(address+":"+std::to_string(port)+"/"+endpoint),m_response(nlohmann::json()),m_error_flag(false){

    m_client.on_message = [this](std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS>::Connection> connection, std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS>::InMessage> in_message) {
        m_response = nlohmann::json::parse(in_message->string());
        if(m_response.find("error")!=m_response.end()){
            m_error_flag=true;
            if(m_response.find("result")==m_response.end()){
                std::cout<<"Unknown error at json websocket server";
            }else{
                std::cout<<m_response["result"]<<std::endl;
            }
        }
        connection->send_close(1000);
    };

    m_client.on_close = [](std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS>::Connection> /*connection*/, int status, const std::string & /*reason*/) {
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    m_client.on_error = [this](std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS>::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
        m_error_flag=true;
        std::cout << "json websocket client: Error: " << ec << ", error message: " << ec.message() << std::endl;
    };
}

bool JsonWebsocketClient::send(const std::string &method, const nlohmann::json &request){
    nlohmann::json message;
    message["method"]=method;
    message["request"]=request;
    m_client.on_open = [&message](std::shared_ptr<SimpleWeb::SocketClient<SimpleWeb::WS>::Connection> connection) {
        connection->send(message.dump());
    };
    m_client.start();
    if(m_error_flag){
        return false;
    }
    return true;
}

void JsonWebsocketClient::get_response(nlohmann::json &response){
    response=m_response;
}

bool JsonWebsocketClient::call_method(const std::string &address, unsigned port, const std::string &endpoint, const std::string &method, const nlohmann::json &request, nlohmann::json &response){
    JsonWebsocketClient client(address,port,endpoint);
    bool result = client.send(method,request);
    client.get_response(response);
    return result;
}

JsonUDPServer::JsonUDPServer(unsigned port):m_port(port){

}

JsonUDPServer::~JsonUDPServer(){
    stop_listening();
}

bool JsonUDPServer::start_listening(){
    if(!is_port_available("localhost",m_port)){
        std::cout<<"Port "+std::to_string(m_port)+" is unavailable."<<std::endl;
        return false;
    }
    m_buffer_size=4096;
    m_slen = sizeof(m_si_other);
    if((m_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){ // If socket for incoming connection could not be created...
        std::cout<<"Could not create socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }

    // Set timeout of 10 ms for incoming UDP connection
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10000;
    if(setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) == -1){
        std::cout<<"Could not set timeout for socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }

    memset((char *) &m_si_me, 0, sizeof(m_si_me));
    m_si_me.sin_family = AF_INET;
    m_si_me.sin_port = htons(m_port);
    m_si_me.sin_addr.s_addr=htonl (INADDR_ANY);
    if(bind(m_socket , (struct sockaddr*)&m_si_me, sizeof(m_si_me)) == -1){
        std::cout<<"Could not bind socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }

    m_flag_keep_listening=true;
    m_server_thread = std::thread(&JsonUDPServer::listen,this);
    return true;
}

void JsonUDPServer::stop_listening(){
    m_flag_keep_listening=false;
    if(m_server_thread.joinable()){
        m_server_thread.join();
    }
    close(m_socket);
}

void JsonUDPServer::listen(){
    char buf[m_buffer_size];
    unsigned payload_size;
    while(m_flag_keep_listening){
        memset(&buf[0], 0, sizeof(buf));
        int reclen=recvfrom(m_socket, buf, m_buffer_size, MSG_WAITALL, (struct sockaddr *) &m_si_other, &m_slen);
        if(reclen<0){
            continue;
        }
        buf[reclen]='\0';
        char* msg = buf;
        char payload[payload_size];
        strncpy(payload,msg+m_header_size,payload_size);
        std::string response = read_message(std::string(buf));
        sendto(m_socket,response.c_str(),strlen(response.c_str()),MSG_CONFIRM,(const struct sockaddr*) &m_si_other, m_slen);
    }
}

std::string JsonUDPServer::read_message(const std::string &msg){
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    std::pair<bool, std::string> response = message_preprocessing(json_msg);
    if(!response.first){
        return response.second;
    }else{
        return call_method(json_msg);
    }
}

std::string JsonUDPServer::call_method(nlohmann::json &message){
    nlohmann::json response;
    try{
        std::pair<bool,std::string> result=this->message_preprocessing(message);
        if(result.first){
            std::string method;
            message["method"].get_to(method);
            if(!this->check_if_method_exists(method)){
                response["result"]="json websocket server: method " + method + " not known";
                response["error"]=true;
            }else{
                if(this->check_arguments(message["request"],m_method_arguments[method],response)){
                    response["result"]=m_method_callbacks[method](message["request"]);
                }else{
                    response["error"]=true;
                }
            }
        }else{
            response["result"]=result.second;
            response["error"]=true;
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        response["result"]=e.what();
    }catch(const nlohmann::detail::parse_error& e){
        std::cout<<e.what()<<std::endl;
        response["result"]=e.what();
    }
    return response.dump();
}

int JsonUDPServer::get_first_byte(char* msg){
    int i=0;
    unsigned payload_size=0;
    for(i;i<m_buffer_size;i++){ // For every element in the message
        if(msg[i]==127 && msg[i+1]==127 && msg[i+2]==127 && msg[i+3]==127){ // If start bytes have been found...
            payload_size=(unsigned)msg[i+m_header_size]; // Read payload size
            if(msg[i+payload_size+m_header_size-4]==126 && msg[i+payload_size+m_header_size-3]==126 && msg[i+payload_size+m_header_size-2]==126 && msg[i+payload_size+m_header_size-1]==126){ // If end bytes have been found in accordance with the payload size
                break;
            }
        }
    }
    if(i<m_buffer_size-m_header_size-payload_size){
        return i;
    }else{
        return -1;
    }
}

bool JsonUDPServer::bind_method(const std::string &name, std::function<nlohmann::json (const nlohmann::json &)> method, const std::vector<std::string> &arguments){
    if(this->check_if_method_exists(name)){
        msrm_utils::print_error("Cannot bind method with name "+name+" since it already exists.");
        return false;
    }
    m_method_callbacks.insert(std::pair<std::string,std::function<nlohmann::json(const nlohmann::json&)> >(name, method));
    m_method_arguments.insert(std::pair<std::string,std::vector<std::string> >(name,arguments));
    return true;
}



bool JsonUDPServer::check_if_method_exists(const std::string& method){
    if(m_method_callbacks.find(method)==m_method_callbacks.end()){
        return false;
    }else{
        return true;
    }
}

std::pair<bool,std::string> JsonUDPServer::message_preprocessing(nlohmann::json &message){
    try{
        if(message.find("method")==message.end()) return std::pair<bool,std::string>(false,"Message header does not contain <method>.");
        if(!message["method"].is_string()) return std::pair<bool,std::string>(false,"Method is not a readable string.");
        if(message.find("request")==message.end()){
            message["request"]=nlohmann::json();
        }
    }catch(const nlohmann::detail::type_error& e){
        std::cout<<e.what()<<std::endl;
        return std::pair<bool,std::string>(false,"Json type error.");
    }
    return std::pair<bool,std::string>(true,"");
}

bool JsonUDPServer::check_arguments(const nlohmann::json &request, const std::vector<std::string> &arguments, nlohmann::json &response){
    if(!request.is_object() && !request.is_null()){
        response["result"]="Request must be a json object (can be null).";
        return false;
    }
    for(const std::string& a : arguments){
        if(request.find(a)==request.end()){
            response["result"]="Could not find parameter "+a+" in request.";
            return false;
        }
    }
    return true;
}

JsonUDPClient::JsonUDPClient(const std::string &address, unsigned port, unsigned timeout):m_address(address),m_port(port),m_timeout(timeout){

}

JsonUDPClient::~JsonUDPClient(){
    close(m_socket);
}

bool JsonUDPClient::send(const std::string &method, const nlohmann::json &request){
    m_buffer_size=4096;
    m_slen=sizeof(m_si_other);
    if ((m_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) // If socket for outgoing connection could not be created...
    {
        std::cout<<"Could not create socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    struct timeval tv;
    tv.tv_sec = m_timeout;
    tv.tv_usec = 0;
    if(setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) == -1){
        std::cout<<"Could not set timeout: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    memset((char *) &m_si_other, 0, sizeof(m_si_other));
    m_si_other.sin_family = AF_INET;
    m_si_other.sin_port = htons(m_port);
    if(inet_aton(m_address.c_str(), &m_si_other.sin_addr)!=0){
        std::cout<<"Invalid address"<<std::endl;
        return false;
    }

    nlohmann::json msg_json;
    msg_json["method"]=method;
    msg_json["request"]=request;
    std::string payload = msg_json.dump();
    const char* msg = payload.c_str();
    int result = sendto(m_socket, msg, strlen(msg), MSG_CONFIRM, (struct sockaddr *) &m_si_other, m_slen);
    if(result<0){
        std::cout<<"Could not send message: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    char buf[m_buffer_size];
    memset(&buf[0], 0, sizeof(buf));
    int reclen = recvfrom(m_socket, (char *)buf, m_buffer_size, MSG_WAITALL, (struct sockaddr *) &m_si_other,&m_slen);
    if(reclen<0){
        return false;
    }
    buf[reclen] = '\0';
    m_response = nlohmann::json::parse(std::string(buf));
    if(m_response.find("error")!=m_response.end()){
        if(m_response.find("result")==m_response.end()){
            std::cout<<"Unknown error at json websocket server";
        }else{
            std::cout<<m_response["result"]<<std::endl;
        }
        return false;
    }else{
        return true;
    }
}

void JsonUDPClient::get_response(nlohmann::json &response){
    response=m_response;
}

bool JsonUDPClient::call_method(const std::string &address, unsigned port, const std::string &method, const nlohmann::json &request, nlohmann::json &response, unsigned timeout){
    JsonUDPClient client(address,port,timeout);
    bool result = client.send(method,request);
    client.get_response(response);
    return result;
}

UDPStreamSender::UDPStreamSender(const std::string &address, unsigned port):m_address(address),m_port(port),m_header_size(10),m_packet_cnt(0){

}

UDPStreamSender::~UDPStreamSender(){
    disconnect();
}

bool UDPStreamSender::connect(){
    m_slen=sizeof(m_si_other);
    if ((m_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) // If socket for outgoing connection could not be created...
    {
        std::cout<<"Could not create socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    memset((char *) &m_si_other, 0, sizeof(m_si_other));
    m_si_other.sin_family = AF_INET;
    m_si_other.sin_port = htons(m_port);
    if(inet_aton(m_address.c_str(), &m_si_other.sin_addr)!=0){
        std::cout<<"Invalid address"<<std::endl;
        return false;
    }
    m_packet_cnt = 0;
    return true;
}

bool UDPStreamSender::disconnect(){
    if(close(m_socket)==-1){
        std::cout<<"UDPStreamSender: Could not close socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    return true;
}

bool UDPStreamSender::send(const std::vector<double> payload){
    // Determine package size: Size of payload * 4 (4 bytes per double) + 4 start bytes + 4 end bytes + package counter + size of payload
    unsigned package_size=payload.size()*4+m_header_size;

    // Initialize message container and set content to zero.
    char msg[package_size];
    for(unsigned i=0;i<package_size;i++){
        msg[i]=0;
    }

    // Elements of message are assigned in a sequence, the counter (cnt_byte) is automatically incremented after each assignment via the increment operator ++
    unsigned cnt_byte=0;

    // The first 4 bytes are start bytes, they are used to identify the message.
    msg[cnt_byte++]=127;
    msg[cnt_byte++]=127;
    msg[cnt_byte++]=127;
    msg[cnt_byte++]=127;

    // Via the package counter one can check if packages have been lost.
    msg[cnt_byte++]=m_packet_cnt;
    m_packet_cnt++;
    if(m_packet_cnt>=255){ // If the package counter reaches 255, it is reset to 0...
        m_packet_cnt=0;
    }

    // The payload size tells the receiver where to find the end bytes and how much to copy.
    msg[cnt_byte++]=payload.size()*4;

    // A union is used to convert the doubles in the payload into bytes that can be sent via UDP.
    union {
        float f;
        char bytes[4];
    } q_union;
    // Every element is converted into 4 bytes (1 double = 4 bytes of memory)
    for(unsigned i=0;i<payload.size();i++){ // For every element in the payload...
        q_union.f = (float)payload[i];
        msg[cnt_byte++]=q_union.bytes[0];
        msg[cnt_byte++]=q_union.bytes[1];
        msg[cnt_byte++]=q_union.bytes[2];
        msg[cnt_byte++]=q_union.bytes[3];
    }
    // The end bytes are used to completely identify the message on the receiver side.
    msg[cnt_byte++]=126;
    msg[cnt_byte++]=126;
    msg[cnt_byte++]=126;
    msg[cnt_byte++]=126;

    // The message is sent out to the peer robot.
    int err=sendto(m_socket, msg, sizeof(msg) , 0 , (struct sockaddr *) &m_si_other, m_slen)<0;
    if(err<0){ // If an error occured during sending...
        std::cout<<"Could not send message: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    return true;
}

UDPStreamReceiver::UDPStreamReceiver(unsigned port, unsigned buffer_size, unsigned timeout_s, unsigned timeout_us,unsigned max_lost_packets,std::function<void(std::vector<double>&)> payload_callback):
    m_port(port),m_buffer_size(buffer_size),m_header_size(10),m_packet_cnt(0),m_timeout_s(timeout_s),m_timeout_us(timeout_us),m_max_lost_packets(max_lost_packets),m_payload_callback(payload_callback){

}

UDPStreamReceiver::~UDPStreamReceiver(){
    disconnect();
}

bool UDPStreamReceiver::connect(){
    m_slen = sizeof(m_si_me);
    if((m_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){ // If socket for incoming connection could not be created...
        std::cout<<"Could not create socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }

    // Set timeout of 10 ms for incoming UDP connection
    struct timeval tv;
    tv.tv_sec = m_timeout_s;
    tv.tv_usec = m_timeout_us;
    if(setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv)==-1){
        std::cout<<"Could not set socket options: "<<std::strerror(errno)<<std::endl;
        return false;
    }

    memset((char *) &m_si_me, 0, sizeof(m_si_me));
    m_si_me.sin_family = AF_INET;
    m_si_me.sin_port = htons(m_port);
    m_si_me.sin_addr.s_addr=htonl (INADDR_ANY);
    if(bind(m_socket , (struct sockaddr*)&m_si_me, sizeof(m_si_me))==-1){
        std::cout<<"Could not bind socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    m_keep_listening=true;
    m_listen_thread = std::thread(&UDPStreamReceiver::listen,this);
    return true;
}

bool UDPStreamReceiver::disconnect(){
    m_keep_listening=false;
    if(m_listen_thread.joinable()){
        m_listen_thread.join();
    }
    if(close(m_socket)==-1){
        std::cout<<"UDPStreamReceiver: Could not close socket: "<<std::strerror(errno)<<std::endl;
        return false;
    }
    return true;
}

void UDPStreamReceiver::listen(){
    char buf[m_buffer_size];

    // Variables are initialized
    bool lost_package=false;
    bool msg_corrupt=false;
    bool msg_buffer=false;
    bool msg_connection_wait=false;
    bool msg_connection_valid=false;
    bool msg_connection_lost=false;
    m_flag_valid_message=false;
    m_lost_packets_cnt=0;
    unsigned cnt_no_connection=0;
    unsigned payload_size=0;
    // Header size of message (contains 4 start bytes, 4 end bytes, 1 package counter, 1 payload size, naming is not quite correct)
    unsigned header_size=m_header_size;

    // Loop for incoming messages is started
    while(m_keep_listening) { // Runs forever if no errors occur...
        if(!msg_connection_wait){
            std::cout<<"Waiting for incoming messages...\n";
            msg_connection_wait=true;
        }
        // Current content from the UDP connection is read into the buffer
        int reclen=recvfrom(m_socket, buf, m_buffer_size, 0, (struct sockaddr *) &m_si_me, &m_slen);
        if(reclen<0 && m_flag_connected){ // If connection is already established but the received message is invalid...
            std::cout<<"UDPStreamReceiver: Could not receive message: "<<std::strerror(errno)<<"\n";
            cnt_no_connection++;
        }

        char* msg = buf;

        lost_package=true;
        unsigned i=0;
        for(i;i<m_buffer_size;i++){ // For every element in the message
            if(msg[i]==127 && msg[i+1]==127 && msg[i+2]==127 && msg[i+3]==127){ // If start bytes have been found...
                payload_size=(unsigned)msg[i+5]; // Read payload size
                if(msg[i+payload_size+header_size-4]==126 && msg[i+payload_size+header_size-3]==126 && msg[i+payload_size+header_size-2]==126 && msg[i+payload_size+header_size-1]==126){ // If end bytes have been found in accordance with the payload size
                    lost_package=false; // A valid message has been found.
                    break;
                }
            }
        }
        if(!((int)msg[i+4]==m_last_packet_number+1 || ((int)msg[i+4]==0 && m_last_packet_number==255))){ // If the last package counter is not one less than the current package counter...
            //                cpp_utils::print_warning("I am losing packets.");
        }
        if(i>=m_buffer_size-payload_size+header_size && reclen==payload_size+header_size && m_flag_connected){ // If the message cannot fit into the buffer but start bytes have been found...
            if(!msg_buffer){
                std::cout<<"UDPStreamReceiver: Message reaches over end of buffer. Start of message is byte "<<i<<".\n";
                msg_buffer=true;
            }
            cnt_no_connection++;
            lost_package=true;
        }
        if(reclen!=payload_size+header_size && m_flag_connected){ // If the length of the received message is not equal to required message size and connection has already been established...
            if(!msg_corrupt){
                std::cout<<"UDPStreamReceiver: Corrupted message. Received length is "<<reclen<<". Expected length is "<<payload_size+header_size<<".\n";
                msg_corrupt=true;
            }
            cnt_no_connection++;
            lost_package=true;
        }
        if(cnt_no_connection>0 && !lost_package){ // If packages have been lost and the current one is valid...
            std::cout<<"UDPStreamReceiver: Number of lost packages: "<<m_lost_packets_cnt<<"\n";
            cnt_no_connection=0;
            msg_buffer=false;
            msg_corrupt=false;
        }
        m_last_packet_number=(int)msg[i+4]; // Read the package counter

        if(cnt_no_connection>m_max_lost_packets){ // If 20 packages were invalid after a connection has already been established...
            if(!msg_connection_lost){
                std::cout<<"UDPStreamReceiver: Lost "<<m_max_lost_packets<<" packets in a row. I assume the network connection is faulty and will terminate.\n";
                msg_connection_lost=true;
            }
            m_keep_listening=false;
        }
        if(lost_package){ // If a package was lost...
            usleep(1000); // Sleep for 1 ms
            m_flag_valid_message=false; // Indicate an invalid message
            continue;
        }else{
            m_flag_connected=true; // The first time this line is reached, a connection is considered as established.
            m_flag_valid_message=true; // Indicate a valid message
            if(!msg_connection_valid){
                std::cout<<"UDPStreamReceiver: Communication has been established.\n";
                msg_connection_valid=true;
            }
        }

        std::vector<double> payload;
        payload.reserve(payload_size);
        // Use a union to convert the bytes in the message into doubles.
        union {
            float f;
            char bytes[4];
        } q_union;
        // Read payload
        for(unsigned j=0;j<payload_size;j+=4){ // For every 4 bytes in the message...
            q_union.bytes[0]=msg[i+j+6];
            q_union.bytes[1]=msg[i+j+7];
            q_union.bytes[2]=msg[i+j+8];
            q_union.bytes[3]=msg[i+j+9];
            payload.push_back(q_union.f); // Converted data element is pushed into the payload vector
        }
        // Unload the payload. This function has to be defined by the respective telepresence prototype.
        m_payload_callback(payload);
    }
    std::cout<<"UDPStreamReceiver: Incoming communication terminated.\n";
}

}
