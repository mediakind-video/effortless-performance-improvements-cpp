#pragma once

#include <string>

void replace_0(std::string& s, const char* from, const char* to);
void replace_1
(std::string& s, const char* const * from, const char* const* to, size_t n);
