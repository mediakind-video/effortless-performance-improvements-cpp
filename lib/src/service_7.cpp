#include <service.hpp>

#include "replace.hpp"
#include "tokenize.hpp"

#include <algorithm>
#include <unordered_map>

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

static std::string_view next_line(const char* &p, const char* last)
{
  const char* end = std::find(p, last, '\n');
  std::string_view result(p, end - p);

  if (end == last)
    p = end;
  else
    p = end + 1;

  return result;
}

std::string service(const std::string& in)
{
  const char* cursor = in.c_str();
  const char* const in_end = cursor + in.size();

  std::string result;
  // We expect the result to be smaller than the input.
  result.reserve(in.size());

  std::uint64_t line_count = 0;

  using entry_map = std::unordered_map<std::string, std::string>;
  entry_map entries;

  std::vector<std::string_view> tokens;
  // Expect at most four fields per line in our input.
  tokens.reserve(4);

  while (cursor != in_end)
    {
      const std::string_view line = next_line(cursor, in_end);
      tokenize_4(tokens, line);

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
