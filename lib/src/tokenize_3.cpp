#include <tokenize.hpp>

void tokenize_3(std::vector<std::string_view>& result, const std::string& s)
{
  result.clear();

  std::string::size_type from = 0;
  std::string::size_type colon = s.find(':');
  const char* const c_str = s.c_str();

  while (colon != std::string::npos)
    {
      result.push_back(std::string_view(c_str + from, colon - from));
      from = colon + 1;
      colon = s.find(':', from);
    }

  result.push_back(std::string_view(c_str + from, s.size() - from));
}
