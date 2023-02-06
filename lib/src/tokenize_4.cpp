#include <tokenize.hpp>

void tokenize_4
(std::vector<std::string_view>& result, const std::string_view& s)
{
  result.clear();

  std::string_view::size_type from = 0;
  std::string_view::size_type colon = s.find(':');

  while (colon != std::string_view::npos)
    {
      result.push_back(s.substr(from, colon - from));
      from = colon + 1;
      colon = s.find(':', from);
    }

  result.push_back(s.substr(from));
}

