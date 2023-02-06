#include <replace.hpp>

#include <cstring>

void replace_0(std::string& s, const char* from, const char* to)
{
  size_t pos = 0;

  while ((pos = s.find(from, pos)) != std::string::npos)
    {
      s.replace(pos, strlen(from), to);
      pos += strlen(to);
    }
}
