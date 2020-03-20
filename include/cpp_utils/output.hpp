#pragma once

#include <iostream>

namespace cpp_utils {

/**
 * Prints a string in a red font with underline in the console.
 * @param str String to be printed.
 */
void print_critical_error(std::string str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a red font in the console.
 * @param str String to be printed.
 */
void print_error(std::string str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a yellow font in the console.
 * @param str String to be printed.
 */
void print_warning(std::string str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a green font in the console.
 * @param str String to be printed.
 */
void print_success(std::string str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a white font in the console.
 * @param str String to be printed.
 */
void print_info(std::string str, bool end_line=true, bool new_line=false);

/**
 * Prints a string in a blue font in the console.
 * @param str String to be printed.
 */
void print_debug(std::string str, bool end_line=true, bool new_line=false);

}
