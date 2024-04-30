#pragma once

#include <string.h>



/// @brief Returns the position of the last occurence of a char in a C-string
/// @param str The string to be searched
/// @param cha The char to which to search for
/// @param len The length of the input string
/// @return The position of the searched char, otherwise -1
short string_last_occurence(const char* str, char cha, size_t len);