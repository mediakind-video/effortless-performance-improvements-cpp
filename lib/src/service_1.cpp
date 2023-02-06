#include <service.hpp>

#include "replace.hpp"
#include "tokenize.hpp"

#include <map>
#include <sstream>

static std::string concatenate_tokens(const std::vector<std::string>& tokens)
{
  std::string result;

  for (const std::string& token : tokens)
    if (result.empty())
      result = token;
    else
      result += ":" + token;

  return result;
}

std::string service(const std::string& in)
{
  std::istringstream iss(in);
  std::string line;
  std::string result;
  std::uint64_t line_count = 0;

  using entry_map = std::map<std::string, std::string>;
  entry_map entries;

  while (std::getline(iss, line))
    {
      std::vector<std::string> tokens = tokenize_0(line);

      if (tokens[0] == "params")
        {
          replace_0(tokens[1], "${param}", "0");
          replace_0(tokens[1], "${tag}", "123");
          replace_0(tokens[1], "${id}", "foobar");
          replace_0
            (tokens[1], "${line_count}", std::to_string(line_count).c_str());

          result += concatenate_tokens(tokens) + '\n';
          ++line_count;
        }
      else if (tokens[0] == "set")
        entries[tokens[1]] = tokens[2];
      else if (tokens[0] == "value")
        {
          const entry_map::const_iterator it = entries.find(tokens[1]);

          if (it != entries.end())
            result += "value:" + it->second + '\n';
          else
            result += "value:\n";

          ++line_count;
        }
      else
        {
          result += concatenate_tokens(tokens) + '\n';
          ++line_count;
        }
    }

  return result;
}
