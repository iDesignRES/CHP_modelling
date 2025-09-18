#pragma once

#include <string>
#include <vector>

/**
 * @brief Splits a string into a vector of substrings based on a delimiter
 * character.
 *
 * This function takes an input string and divides it into substrings wherever
 * the specified delimiter character occurs. The resulting substrings are
 * returned in a vector.
 *
 * @param str The input string to be divided.
 * @param c The delimiter character used to split the string.
 * @return std::vector<std::string> A vector containing the substrings resulting
 * from the split.
 */
std::vector<std::string> divide_string(std::string str, char c);

/**
 * @brief Constructs the full path to a database file located in the "Database"
 * directory
 *
 * This function uses the path of the current source file (__FILE__) to
 * determine the directory, then appends "Database/" and the provided sub_path
 * to build the full path to the database file.
 *
 * @param sub_path The relative path or filename of the database file within the
 * "Database" directory.
 * @return std::string The full path to the database file. Returns an empty
 * string if the current file path cannot be determined.
 */
std::string get_database_path(std::string sub_path);