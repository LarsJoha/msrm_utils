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
#include <functional>

#include <simple-websocket-server/server_ws.hpp>
#include <simple-websocket-server/client_ws.hpp>
#include <jsonrpccxx/client.hpp>
#include <jsonrpccxx/server.hpp>
#include <jsonrpccxx/iclientconnector.hpp>
#include <httplib/httplib.h>

namespace msrm_utils{

/**
 * Pings a specified IP address.
 * @param ipaddr IP address to ping.
 * @return Returns true if ipaddr could be pinged, false otherwise.
 */
bool ping(const char* ipaddr);

/**
 * Checks whether the given IP address is valid.
 * @param ipaddr IP address to check.
 * @return True if ipaddr is valid, false otherwise.
 */
bool is_valid_ip_address(const char *ipaddr);

/**
 * Check if specified port on specified host is in use.
 * @param[in] port The port to check.
 * @param host The host on which to check the port.
 * @return True if port on host is in use, false otherwise.
 */
bool is_port_available(const char* host, unsigned port);

/**
 * Returns the IP address of the computer for the indicated interface.
 * @param iface Network interface to check.
 * @return IP address as std::string for the specified interface.
 */
std::optional<std::string> get_own_ip(const char* iface);

/**
 * Returns the IP address of the indicated hostname.
 * @param hostname Target hostname.
 * @return IP address as std::string for the specified hostname.
 */
std::optional<std::string> get_ip_by_hostname(const char* hostname);

/**
 * Returns the subnets for all network interfaces.
 * @return A vector of subnets as strings.
 */
std::map<std::string, std::string> get_subnets();

/**
 * Returns all network interfaces.
 * @return A vector of all network interfaces as strings.
 */
std::set<std::string> get_ifaces();

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
std::string convert_ip_from_default_format(const std::string &ip);

class IJsonMethodServer{
public:
    virtual bool start_listening() = 0;
    virtual void stop_listening() = 0;
    virtual bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string> &arguments) = 0;
};

class JsonRPCServer: public IJsonMethodServer {
public:
    JsonRPCServer(const std::string &address, unsigned port);
    ~JsonRPCServer();

    bool start_listening();
    void stop_listening();
    bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string> &arguments);

private:
    std::thread m_server_thread;
    jsonrpccxx::JsonRpc2Server m_server;
    httplib::Server m_http_server;
    const std::string m_address;
    unsigned m_port;
};

class JsonRPCClient : public jsonrpccxx::IClientConnector {
public:
    JsonRPCClient(const char* host, int port, double timeout);

    bool send(const std::string& method, const nlohmann::json& request);
    void get_response(nlohmann::json& response);
    static bool call_method(const std::string& address, unsigned port, const std::string& method, const nlohmann::json& request, nlohmann::json& response, unsigned timeout=1);
private:
    std::string Send(const std::string &request) override;
    jsonrpccxx::JsonRpcClient m_rpc_client;
    httplib::Client m_http_client;
    double m_timeout;
    nlohmann::json m_response;
};

class JsonWebsocketServer : public IJsonMethodServer{
public:
    JsonWebsocketServer(const std::string& address="localhost", unsigned port=9000, unsigned thread_pool_size=1, const std::string& endpoint="");
    ~JsonWebsocketServer();

    bool start_listening();
    void stop_listening();

    bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string>& arguments);

private:

    std::pair<bool, std::string> message_preprocessing(nlohmann::json &message);
    bool check_if_method_exists(const std::string& method);
    bool check_arguments(const nlohmann::json& request, const std::vector<std::string> &arguments, nlohmann::json& response);

    SimpleWeb::SocketServer<SimpleWeb::WS> m_server;
    std::map<std::string, std::function<nlohmann::json(nlohmann::json)> > m_method_callbacks;
    std::map<std::string, std::vector<std::string> > m_method_arguments;
    std::thread m_server_thread;
};

class JsonWebsocketClient{
public:
    JsonWebsocketClient(const std::string &address, unsigned port, const std::string &endpoint);
    bool send(const std::string& method, const nlohmann::json& request);
    void get_response(nlohmann::json& response);
    static bool call_method(const std::string& address, unsigned port, const std::string& endpoint, const std::string& method,const nlohmann::json& request,nlohmann::json& response);
private:
    SimpleWeb::SocketClient<SimpleWeb::WS> m_client;
    nlohmann::json m_response;
    std::atomic<bool> m_error_flag;
};

class JsonUDPServer : public IJsonMethodServer{
public:

    JsonUDPServer(unsigned port);
    ~JsonUDPServer();
    bool start_listening();
    void stop_listening();
    bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string> &arguments);
private:

    void listen();
    int get_first_byte(char *msg);
    std::string read_message(const std::string& msg);
    std::string call_method(nlohmann::json &message);
    bool check_if_method_exists(const std::string& method);
    bool check_arguments(const nlohmann::json& request, const std::vector<std::string> &arguments, nlohmann::json& response);
    std::pair<bool, std::string> message_preprocessing(nlohmann::json &message);

    std::map<std::string, std::function<nlohmann::json(nlohmann::json)> > m_method_callbacks;
    std::map<std::string, std::vector<std::string> > m_method_arguments;
    std::thread m_server_thread;

    std::atomic<bool> m_flag_keep_listening;

    unsigned m_port;
    int m_socket;
    struct sockaddr_in m_si_other,m_si_me;
    unsigned m_slen;
    unsigned m_buffer_size;
    unsigned m_header_size;
};

class JsonUDPClient{
public:
    JsonUDPClient(const std::string& address, unsigned port,unsigned timeout);
    ~JsonUDPClient();
    bool send(const std::string& method, const nlohmann::json& request);
    void get_response(nlohmann::json& response);
    static bool call_method(const std::string& address, unsigned port, const std::string& method, const nlohmann::json& request, nlohmann::json& response, unsigned timeout=1);
private:
    std::string m_address;
    unsigned m_port;
    unsigned m_timeout;
    int m_socket;
    struct sockaddr_in m_si_other,m_si_me;
    unsigned m_slen;
    unsigned m_buffer_size;
    unsigned m_header_size;

    nlohmann::json m_response;
};

class UDPStreamSender{
public:
    UDPStreamSender(const std::string& address, unsigned port);
    ~UDPStreamSender();
    bool connect();
    bool disconnect();
    bool send(const std::vector<double> payload);
    bool send(const char* header, const std::vector<double> payload);
private:
    std::string m_address;
    unsigned m_port;
    int m_socket;
    struct sockaddr_in m_si_other;
    unsigned m_slen;
    unsigned m_buffer_size;
    unsigned m_header_size;

    unsigned m_packet_cnt;
};

class UDPStreamReceiver{
public:
    UDPStreamReceiver(unsigned port, unsigned buffer_size, unsigned timeout_s, unsigned timeout_us,unsigned max_lost_packets,std::function<void(std::vector<double>&)> payload_callback);
    ~UDPStreamReceiver();
    bool connect();
    bool disconnect();
    void get_payload(std::vector<double>& payload);
private:
    void listen();

    unsigned m_port;
    int m_socket;
    struct sockaddr_in m_si_me;
    unsigned m_slen;
    unsigned m_buffer_size;
    unsigned m_header_size;

    unsigned m_packet_cnt;
    unsigned m_last_packet_number;
    unsigned m_lost_packets_cnt;
    unsigned m_max_lost_packets;
    unsigned m_timeout_s;
    unsigned m_timeout_us;

    std::thread m_listen_thread;
    std::atomic<bool> m_keep_listening;
    std::atomic<bool> m_flag_connected;
    std::atomic<bool> m_flag_valid_message;

    std::function<void(std::vector<double>&)> m_payload_callback;
};

}
