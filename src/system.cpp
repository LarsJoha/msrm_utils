#include "cpp_utils/system.hpp"

#include <unistd.h>
#include <linux/limits.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "cpp_utils/output.hpp"

namespace cpp_utils {

std::string get_path_executable(char **argv){
    char path_save[PATH_MAX];
    char abs_exe_path[PATH_MAX];
    char *p;

    if(!(p = strrchr(argv[0], '/')))
        char* rtn=getcwd(abs_exe_path, sizeof(abs_exe_path));
    else
    {
        *p = '\0';
        char* rtn=getcwd(path_save, sizeof(path_save));
        int r=chdir(argv[0]);
        rtn=getcwd(abs_exe_path, sizeof(abs_exe_path));
        r=chdir(path_save);
    }
    return std::string(abs_exe_path);
}

bool process_is_running(std::string process){
    std::string cmd = "pidof -x " + process + " > /dev/null";
    return system(cmd.c_str()) == 0;
}

bool is_port_open(unsigned port, std::string host){

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return false;
    }

    server = gethostbyname(host.c_str());

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

}
