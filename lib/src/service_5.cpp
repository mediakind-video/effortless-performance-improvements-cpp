#include <service.hpp>

#include "replace.hpp"
#include "tokenize.hpp"

#include <unordered_map>
#include <sstream>

static void concatenate_tokens
(std::string& result, const std::vector<std::string_view>& tokens)
{
  result.append(tokens[0].begin(), tokens[0].end());

  for (std::size_t i = 1, n = tokens.size(); i != n; ++i)
    {
      result += ':';
      result.append(tokens[i].begin(), tokens[i].end());
    }

  result += '\n';
}

std::string service(const std::string& in)
{
  std::istringstream iss(in);
  std::string line;

  std::string result;
  // We expect the result to be smaller than the input.
  result.reserve(in.size());

  std::uint64_t line_count = 0;

  using entry_map = std::unordered_map<std::string, std::string>;
  entry_map entries;

  while (std::getline(iss, line))
    {
      std::vector<std::string_view> tokens = tokenize_2(line);

      if (tokens[0] == "params")
        {
          std::string token_str(tokens[1].begin(), tokens[1].end());

          replace_0(token_str, "${param}", "0");
          replace_0(token_str, "${tag}", "123");
          replace_0(token_str, "${id}", "foobar");
          replace_0
            (token_str, "${line_count}", std::to_string(line_count).c_str());

          tokens[1] = std::string_view(token_str.c_str(), token_str.size());
          concatenate_tokens(result, tokens);
          ++line_count;
        }
      else if (tokens[0] == "set")
        {
          const std::string key(tokens[1].begin(), tokens[1].end());
          const std::string value(tokens[2].begin(), tokens[2].end());
          entries[key] = value;
        }
      else if (tokens[0] == "value")
        {
          const std::string key(tokens[1].begin(), tokens[1].end());
          const entry_map::const_iterator it = entries.find(key);

          if (it != entries.end())
            result += "value:" + it->second + '\n';
          else
            result += "value:\n";

          ++line_count;
        }
      else
        {
          concatenate_tokens(result, tokens);
          ++line_count;
        }
    }

  return result;
}
