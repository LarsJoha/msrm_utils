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

/*! Interface for json method servers */
class IJsonMethodServer{
public:
    /**
     * @brief start_listening When calling this method the server should start listening.
     * @return Should return true if server start was succesful, false otherwise
     */
    virtual bool start_listening() = 0;
    /**
     * @brief stop_listening When calling this method the server should stop listening
     */
    virtual void stop_listening() = 0;
    /**
     * @brief bind_method Binds a function or method as a callback to the server.
     * @param name Name of the method when calling the server via network.
     * @param method Pointer to the function or method.
     * @param arguments Argument names of the function.
     * @return
     */
    virtual bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string> &arguments) = 0;
};

/*! An rpc method server based on json.*/
class JsonRPCServer: public IJsonMethodServer {
public:
    /**
     * @brief JsonRPCServer Constructor
     * @param address The address to bind to.
     * @param port The port to bind to.
     */
    JsonRPCServer(const std::string &address, unsigned port);
    /**
     * @brief ~JsonRPCServer The destructor automatically calls stop_listening
     */
    ~JsonRPCServer();

    /**
     * @brief start_listening The RPC server starts listening.
     * @return True if server could be started, false otherwise.
     */
    bool start_listening();
    /**
     * @brief stop_listening Stops the RPC server
     */
    void stop_listening();
    /**
     * @brief bind_method Binds a function or method as a callback to the server.
     * @param name Name of the method when calling the server via network.
     * @param method Pointer to the function or method.
     * @param arguments Argument names of the function.
     * @return
     */
    bool bind_method(const std::string& name, std::function<nlohmann::json(const nlohmann::json& request)> method, const std::vector<std::string> &arguments);

private:
    std::thread m_server_thread;
    jsonrpccxx::JsonRpc2Server m_server;
    httplib::Server m_http_server;
    const std::string m_address;
    unsigned m_port;
};

/*! An RPC client that connects to a server*/
class JsonRPCClient : public jsonrpccxx::IClientConnector {
public:
    /**
     * @brief JsonRPCClient Client constructor
     * @param host Hostname of server.
     * @param port Port of server.
     * @param timeout Time without answer from server after which the client aborts the call.
     */
    JsonRPCClient(const char* host, int port, double timeout);

    /**
     * @brief send Sends a json message to the server.
     * @param method The server method to call.
     * @param request The method parameters in a json object.
     * @return True if call was successful, false otherwise.
     */
    bool send(const std::string& method, const nlohmann::json& request);
    /**
     * @brief get_response Returns the response from the last call.
     * @param[out] response The response is written into the provided json object.
     */
    void get_response(nlohmann::json& response);
    /**
     * @brief call_method Factory method to directly call a server.
     * @param address Server address
     * @param port Server port
     * @param method Method on server
     * @param request Method parameters
     * @param[out] response Response from server
     * @param timeout Time without answer from server after which the client aborts the call.
     * @return True if call was successful, false otherwise.
     */
    static bool call_method(const std::string& address, unsigned port, const std::string& method, const nlohmann::json& request, nlohmann::json& response, unsigned timeout=1);
private:
    std::string Send(const std::string &request) override;
    jsonrpccxx::JsonRpcClient m_rpc_client;
    httplib::Client m_http_client;
    double m_timeout;
    nlohmann::json m_response;
};

/*! A websocket method server based on json.*/
class JsonWebsocketServer : public IJsonMethodServer{
public:
    /**
     * @brief JsonWebsocketServer Constructor
     * @param address Address to bind to.
     * @param port Port to bind to.
     * @param endpoint Server endpoint
     */
    JsonWebsocketServer(const std::string& address="localhost", unsigned port=9000, const std::string& endpoint="");
    /**
     * @brief ~JsonWebsocketServer The destructor automatically calls stop_listening
     */
    ~JsonWebsocketServer();

    /**
     * @brief start_listening The RPC server starts listening.
     * @return True if server could be started, false otherwise.
     */
    bool start_listening();
    /**
     * @brief stop_listening Stops the RPC server
     */
    void stop_listening();
    /**
     * @brief bind_method Binds a function or method as a callback to the server.
     * @param name Name of the method when calling the server via network.
     * @param method Pointer to the function or method.
     * @param arguments Argument names of the function.
     * @return
     */
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
    /**
     * @brief JsonRPCClient Client constructor
     * @param host Hostname of server.
     * @param port Port of server.
     * @param endpoint Server endpoint
     */
    JsonWebsocketClient(const std::string &address, unsigned port, const std::string &endpoint);
    /**
     * @brief send Sends a json message to the server.
     * @param method The server method to call.
     * @param request The method parameters in a json object.
     * @return True if call was successful, false otherwise.
     */
    bool send(const std::string& method, const nlohmann::json& request);
    /**
     * @brief get_response Returns the response from the last call.
     * @param[out] response The response is written into the provided json object.
     */
    void get_response(nlohmann::json& response);
    /**
     * @brief call_method Factory method to directly call a server.
     * @param address Server address
     * @param port Server port
     * @param endpoint Server endpoint
     * @param method Method on server
     * @param request Method parameters
     * @param[out] response Response from server
     * @return True if call was successful, false otherwise.
     */
    static bool call_method(const std::string& address, unsigned port, const std::string& endpoint, const std::string& method,const nlohmann::json& request,nlohmann::json& response);
private:
    SimpleWeb::SocketClient<SimpleWeb::WS> m_client;
    nlohmann::json m_response;
    std::atomic<bool> m_error_flag;
};

class JsonUDPServer : public IJsonMethodServer{
public:

    /**
     * @brief JsonUDPServer Constructor
     * @param port The port to bind to.
     */
    JsonUDPServer(unsigned port);
    /**
     * @brief ~JsonUDPServer The destructor automatically calls stop_listening
     */
    ~JsonUDPServer();
    /**
     * @brief start_listening The RPC server starts listening.
     * @return True if server could be started, false otherwise.
     */
    bool start_listening();
    /**
     * @brief stop_listening Stops the RPC server
     */
    void stop_listening();
    /**
     * @brief bind_method Binds a function or method as a callback to the server.
     * @param name Name of the method when calling the server via network.
     * @param method Pointer to the function or method.
     * @param arguments Argument names of the function.
     * @return
     */
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
    /**
     * @brief JsonRPCClient Client constructor
     * @param host Hostname of server.
     * @param port Port of server.
     * @param timeout Time without answer from server after which the client aborts the call.
     */
    JsonUDPClient(const std::string &address, unsigned port, unsigned timeout);
    /**
     * @brief ~JsonUDPClient The destructor closes the socket.
     */
    ~JsonUDPClient();
    /**
     * @brief send Sends a json message to the server.
     * @param method The server method to call.
     * @param request The method parameters in a json object.
     * @return True if call was successful, false otherwise.
     */
    bool send(const std::string& method, const nlohmann::json& request);
    /**
     * @brief get_response Returns the response from the last call.
     * @param[out] response The response is written into the provided json object.
     */
    void get_response(nlohmann::json& response);
    /**
     * @brief call_method Factory method to directly call a server.
     * @param address Server address
     * @param port Server port
     * @param endpoint Server endpoint
     * @param method Method on server
     * @param request Method parameters
     * @param[out] response Response from server
     * @return True if call was successful, false otherwise.
     */
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

/*! UDP client class for sending a payload to a peer.*/
class UDPStreamSender{
public:
    /**
     * @brief UDPStreamSender Constructor
     * @param address Server address
     * @param port Server port
     */
    UDPStreamSender(const std::string& address, unsigned port);
    /**
     * @brief ~UDPStreamSender The destructor automatically calls disconnect
     */
    ~UDPStreamSender();
    /**
     * @brief connect Connects to the server
     * @return True if connection is successful, false otherwise
     */
    bool connect();
    /**
     * @brief disconnect Disconnects the current connection
     * @return True if successful, false otherwise
     */
    bool disconnect();
    /**
     * @brief send Send a single message to the server
     * @param payload A payload of doubles
     * @return True if successful, false otherwise
     */
    bool send(const std::vector<double> payload);
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

/*! UDP client class for receiving a payload from a peer.*/
class UDPStreamReceiver{
public:
    /**
     * @brief UDPStreamReceiver Contructor
     * @param port Port on which to listen
     * @param buffer_size Buffer size for incoming messages
     * @param timeout_s Timeout in seconds
     * @param timeout_us Timeout in milliseconds
     * @param max_lost_packets Maximum allowed number of packet before connection is closed
     * @param payload_callback Callback to function that accepts the payload for unpacking.
     */
    UDPStreamReceiver(unsigned port, unsigned buffer_size, unsigned timeout_s, unsigned timeout_us,unsigned max_lost_packets,std::function<void(std::vector<double>&)> payload_callback);
    /**
     * @brief ~UDPStreamReceiver The destructor automatically calls disconnect
     */
    ~UDPStreamReceiver();
    /**
     * @brief connect Connects to the peer
     * @return True if connection was successful, false otherwise
     */
    bool connect();
    /**
     * @brief disconnect Disconnects current connection
     * @return True if connection was successful, false otherwise
     */
    bool disconnect();
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
