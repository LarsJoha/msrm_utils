#include "cpp_utils/files.hpp"

namespace msrm_utils {

void write_endl_to_file(const std::string& file){
    std::ofstream s;
    s.open(file,std::ios::app);

    std::ostringstream oss;
    oss.imbue(std::locale::classic());
    oss.precision(std::numeric_limits<double>::digits10);
    oss<<std::endl;
    s<<oss.str();
}

void get_current_directory(char* path) {
    char buff[FILENAME_MAX];
    char* rtn=getcwd( path, FILENAME_MAX );
}

bool check_if_file_exists(const char *file){
    struct stat buf;
    return (stat(file, &buf) == 0);
}

std::vector<std::string> split_string(std::string s, const std::string &delimiter){
    size_t pos = 0;
    std::vector<std::string> token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token.emplace_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    token.emplace_back(s);
    return token;
}

}
