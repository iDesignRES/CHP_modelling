#pragma once

#include <string>
#include <toml++/toml.hpp>
#include <vector>

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

/**
 * @brief Opens and parses a TOML file, returning its contents as a toml::table
 *
 * @param file The path to the TOML file to be opened and parsed.
 * @return toml::table The parsed contents of the TOML file as a toml::table.
 * @throws std::runtime_error if parsing fails, with the error message.
 */
toml::table get_toml_table(std::string file);

/**
 * @brief Prints a separation line to the standard output for better readability
 * in console logs.
 */
void print_separation_line();