#include "msrm_utils/system.hpp"
#include "msrm_utils/output.hpp"

#include <unistd.h>
#include <linux/limits.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory>
#include <array>
#include <stdexcept>
#include <cstdio>
#include <string>


namespace msrm_utils {

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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

bool process_is_running(const std::string &process){
    std::string cmd = "pidof -x " + process + " > /dev/null";
    return system(cmd.c_str()) == 0;
}

}
