#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>

static void print_help(const char* program)
{
  printf("Usage: %s record_count\n", program);
}

int main(int argc, char** argv)
{
  if (argc != 2)
    {
      print_help(argv[0]);
      return 1;
    }

  if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0))
    {
      print_help(argv[0]);
      return 0;
    }

  const long long count = std::atoll(argv[1]);
  std::random_device rd;
  std::mt19937 mt(rd());
  int next_key_value = 1;

  for (long long i = 0; i != count; ++i)
    switch (mt() % 4)
        {
        case 0:
          printf("params:${param},${tag},${id},${line_count}\n");
          break;
        case 1:
          printf("set:key-%d:value-%d\n", next_key_value, next_key_value);
          ++next_key_value;
          break;
        case 2:
          {
            const int ref = mt() % next_key_value;
            printf("value:key-%d\n", ref);
            break;
          }
        case 3:
          printf("unknown:abc:def:geh\n");
          break;
        }

  return 0;
}

