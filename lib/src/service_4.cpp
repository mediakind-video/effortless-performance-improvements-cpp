#include <service.hpp>

#include "replace.hpp"
#include "tokenize.hpp"

#include <unordered_map>
#include <sstream>

static std::string concatenate_tokens
(const std::vector<std::string_view>& tokens)
{
  std::string result;

  for (const std::string_view& token : tokens)
    if (result.empty())
      result = std::string(token.begin(), token.end());
    else
      result += ":" + std::string(token.begin(), token.end());

  return result;
}

std::string service(const std::string& in)
{
  std::istringstream iss(in);
  std::string line;
  std::string result;
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
          result += concatenate_tokens(tokens) + '\n';
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
          result += concatenate_tokens(tokens) + '\n';
          ++line_count;
        }
    }

  return result;
}
