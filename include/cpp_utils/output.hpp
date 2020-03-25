#pragma once

#include <iostream>

namespace cpp_utils {

/**
 * Prints a string in a red font with underline in the console.
 * @param str String to be printed.
 */
void print_critical_error(const std::string& str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a red font in the console.
 * @param str String to be printed.
 */
void print_error(const std::string& str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a yellow font in the console.
 * @param str String to be printed.
 */
void print_warning(const std::string& str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a green font in the console.
 * @param str String to be printed.
 */
void print_success(const std::string& str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a white font in the console.
 * @param str String to be printed.
 */
void print_info(const std::string& str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a blue font in the console.
 * @param str String to be printed.
 */
void print_debug(const std::string& str, bool end_line=true, bool new_line=false);

}
