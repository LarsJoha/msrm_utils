#pragma once

#include <string>

namespace msrm_utils {

/**
 * @brief Executes the given system command.
 * @param cmd The command
 * @return Return of given command
 */
std::string exec(const char* cmd);

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

}
