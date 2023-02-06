#include <replace.hpp>

#include <cstring>

void replace_1
(std::string& s, const char* const * from, const char* const* to, size_t n)
{
  const char separator = from[0][0];
  std::string::size_type pos = s.find_first_of(separator);

  while (pos != std::string::npos)
    {
      bool found = false;

      for (size_t i = 0; (i != n) && !found; ++i)
        {
          const size_t from_size = std::strlen(from[i]);

          if (std::strncmp(s.data() + pos, from[i], from_size) == 0)
            {
              s.replace(pos, from_size, to[i]);
              found = true;
              pos = s.find_first_of(separator, pos + std::strlen(to[i]));
            }
        }

      if (!found)
        pos = s.find_first_of(separator, pos + 1);
    }
}

