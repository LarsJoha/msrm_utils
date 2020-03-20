#pragma once

#include<iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <set>
#include <map>

#include <simple-websocket-server/server_ws.hpp>
#include <jsonrpccxx/client.hpp>
#include <jsonrpccxx/server.hpp>
#include <jsonrpccxx/iclientconnector.hpp>
#include <httplib/httplib.h>

#include "cpp_utils/json.hpp"

namespace cpp_utils{

/**
 * Pings a specified IP address.
 * @param ipaddr IP address to ping.
 * @return
 */
int ping(const char* ipaddr);

/**
 * Checks whether the given IP address is valid.
 * @param ipaddr IP address to check.
 * @return True if ipaddr is valid, false otherwise.
 */
bool is_valid_ip_address(const char *ipaddr);

/**
 * Returns the IP address of the computer for the indicated interface.
 * @param iface Network interface to check.
 * @return IP address as std::string for the specified interface.
 */
std::string get_own_ip(const std::string& iface);

/**
 * Returns the IP address of the indicated hostname.
 * @param hostname Target hostname.
 * @return IP address as std::string for the specified hostname.
 */
std::string get_ip_by_hostname(const std::string& hostname);

/**
 * Returns the subnets for all network interfaces.
 * @return A vector of subnets as strings.
 */
std::map<std::string, std::string> get_subnets();

/**
 * Returns all network interfaces.
 * @return A vector of all network interfaces as strings.
 */
std::vector<std::string> get_ifaces();

/**
 * Makes a call via rpc-protocol to a target server with a specified method and parameters.
 * @param[in] url URL of target server, must be of form "http://<hostname>:<port>", hostname being either the hostname or
 * the ip of the server and port being the port on which the server listens.
 * @param[in] method Name of the method on the server.
 * @param[in] request Request to the method in json format.
 * @param[out] response Response from the method in json format.
 * @param[in] timeout Amount of time to wait for a response from the server until the call is considered a failure.
 * @return True if response is nominal, false if server is not reachable, method is not available, or request is invalid.
 */
bool rpc_call(const std::string& address, unsigned port, const std::string& method, const jsonrpccxx::positional_parameter &request,  nlohmann::json &response, long timeout=1000, unsigned id=0);

/**
 * Makes a call via rpc-protocol to a target server with a specified method and parameters. This function is intended for threaded use,
 * hence it does not have const references as parameters.
 * @param url URL of target server, must be of form "http://<hostname>:<port>", hostname being either the hostname or
 * the ip of the server and port being the port on which the server listens.
 * @param method Name of the method on the server.
 * @param request Request to the method in json format.
 * @param[out] response Response from the method in json format.
 * @param[in] timeout Amount of time to wait for a response from the server until the call is considered a failure.
 * @return True if response is nominal, false if server is not reachable, method is not available, or request is invalid.
 */
bool rpc_call_thr(const std::string &address, unsigned port, const std::string &method, nlohmann::json request, nlohmann::json &response,  long timeout=1000, unsigned id=0);

/**
 * Converts a given IP address into a default format. E.g. 192.168.4.24 -> 192.168.xx4.x24
 * @param ip IP to convert.
 * @return Returns the IP in default format.
 */
std::string convert_ip_to_default_format(const std::string &ip);

/**
 * Converts a given IP address in default format into its normal format. E.g. -> 192.168.xx4.x24 -> 192.168.4.24
 * @param ip IP in default format to convert.
 * @return Returns the IP in normal format.
 */
std::string convert_ip_from_default_format(const std::string& ip);

/**
 * Checks if a given ip address has a valid format.
 * @param ip IP to check.
 * @return True if ip has a valid format, false otherwise.
 */
bool check_if_valid_ip(const std::string& ip);


class CppHttpLibServerConnector {
public:
    explicit CppHttpLibServerConnector(jsonrpccxx::JsonRpcServer& server, const std::string& address, int port);
    virtual ~CppHttpLibServerConnector();

    bool start_listening();
    void stop_listening();

private:
    std::thread _thread;
    jsonrpccxx::JsonRpcServer& _server;
    httplib::Server _httpServer;
    std::string _address;
    int _port;
};

class CppHttpLibClientConnector : public jsonrpccxx::IClientConnector {
public:
    explicit CppHttpLibClientConnector(const std::string &host, int port, double timeout=5);
    std::string Send(const std::string &request) override;
private:
    httplib::Client _httpClient;
    double _timeout;
};

class UDP{
public:
    UDP();
    ~UDP();

    bool initialize_sender();
    bool initialize_receiver();
    bool send_message(const char* msg);
    bool receive_message(char* msg);
    void terminate_sender();
    void terminate_receiver();

private:

    int _s_send;
    unsigned _port_send;
    std::string _ip_send;
    bool _broadcast;
    bool _multicast;

    int _s_receive;
    unsigned _port_recv;


    unsigned _n_package;
    unsigned _n_package_last;
    char _buf[512];
    struct sockaddr_in _si_other,_si_me;
    unsigned _slen;
    unsigned _bufferlength;
    unsigned _packagesize;
    unsigned _cnt_lost_packages;
};

class JsonWebsocketServer{
public:
    JsonWebsocketServer(std::string address="localhost", unsigned port=9000, unsigned thread_pool_size=1, std::string endpoint="");
    ~JsonWebsocketServer();

    void start_listening();
    void stop_listening();

    bool bind_method(std::string name, std::function<nlohmann::json(const nlohmann::json& request)> method, std::set<std::string> arguments);

private:

    std::pair<bool, std::string> message_preprocessing(nlohmann::json &message);
    bool check_if_method_exists(std::string method);
    bool check_arguments(const nlohmann::json& request, const std::set<std::string> &arguments, nlohmann::json& response);

    SimpleWeb::SocketServer<SimpleWeb::WS> _server;
    std::map<std::string, std::function<nlohmann::json(nlohmann::json)> > _methods;
    std::map<std::string, std::set<std::string> > _arguments;

    std::thread _thread;
};

}
