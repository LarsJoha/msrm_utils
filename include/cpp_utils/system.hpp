#pragma once

#include <string>

namespace cpp_utils {

/**
 * Returns the absolute path of the executable.
 * @param[in] argv Argument list that has been passed to a main function.
 * @return Absolute path of the executable as std::string.
 */
std::string get_path_executable(char **argv);

/**
 * Indicates if a specific process is currently running.
 * @param[in] process Name of the process.
 * @return True if the process is currently running, false if not.
 */
bool process_is_running(const std::string& process);

/**
 * Check if specified port on specified host is in use.
 * @param[in] port The port to check.
 * @param host The host on which to check the port.
 * @return True if port on host is in use, false otherwise.
 */
bool is_port_open(unsigned port, const std::string &host="localhost");

}
