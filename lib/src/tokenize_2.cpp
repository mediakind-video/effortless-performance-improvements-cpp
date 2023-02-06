#include <tokenize.hpp>

std::vector<std::string_view> tokenize_2(const std::string& s)
{
  std::vector<std::string_view> result;
  // Expect four fields in our input.
  result.reserve(4);

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

  return result;
}

