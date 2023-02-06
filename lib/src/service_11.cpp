#include <service.hpp>

#include "replace.hpp"
#include "tokenize.hpp"

#include <tsl/robin_map.h>

#include <algorithm>

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

static const char* custom_to_string(char* out, std::uint64_t v)
{
  --out;
  *out = '\0';

  while (v >= 10)
    {
      --out;
      *out = '0' + (v % 10);
      v /= 10;
    }

  --out;
  *out = '0' + v;

  return out;
}

std::string service(const std::string& in)
{
  const char* cursor = in.c_str();
  const char* const in_end = cursor + in.size();

  std::string result;
  // We expect the result to be smaller than the input.
  result.reserve(in.size());

  std::uint64_t line_count = 0;
  using entry_map = tsl::robin_map<std::string_view, std::string_view>;
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
          const char* placeholders[] =
            {
              "${param}",
              "${tag}",
              "${id}",
              "${line_count}"
            };
          constexpr std::size_t n =
            sizeof(placeholders) / sizeof(*placeholders);

          char buffer[std::numeric_limits<std::uint64_t>::digits10 + 1];
          const char* line_count_string =
            custom_to_string(std::end(buffer), line_count);

          const char* replacements[] =
            {
              "0",
              "123",
              "foobar",
              line_count_string
            };
          static_assert(sizeof(replacements) / sizeof(*replacements) == n, "");

          std::string token_str(tokens[1].begin(), tokens[1].end());
          replace_1(token_str, placeholders, replacements, n);

          tokens[1] = std::string_view(token_str.c_str(), token_str.size());
          concatenate_tokens(result, tokens);
          ++line_count;
        }
      else if (tokens[0] == "set")
        entries[tokens[1]] = tokens[2];
      else if (tokens[0] == "value")
        {
          const entry_map::const_iterator it = entries.find(tokens[1]);

          if (it != entries.end())
            tokens[1] = it->second;
          else
            {
              static constexpr std::string_view empty(nullptr, size_t(0));
              tokens[1] = empty;
            }

          concatenate_tokens(result, tokens);
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
