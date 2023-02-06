#pragma once

#include <string_view>

#include <string>
#include <vector>

std::vector<std::string> tokenize_0(const std::string& s);
std::vector<std::string> tokenize_1(const std::string& s);
std::vector<std::string_view> tokenize_2(const std::string& s);
void tokenize_3(std::vector<std::string_view>& result, const std::string& s);
void tokenize_4
(std::vector<std::string_view>& result, const std::string_view& s);
