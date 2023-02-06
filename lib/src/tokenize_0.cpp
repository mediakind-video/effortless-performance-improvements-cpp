#include <tokenize.hpp>

std::vector<std::string> tokenize_0(const std::string& s)
{
  std::vector<std::string> result;

  std::string::size_type from = 0;
  std::string::size_type colon = s.find(':');

  while (colon != std::string::npos)
    {
      result.push_back(s.substr(from, colon - from));
      from = colon + 1;
      colon = s.find(':', from);
    }

  result.push_back(s.substr(from));

  return result;
}

