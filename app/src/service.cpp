#include <service.hpp>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

static void print_help(const char* program)
{
  printf("Usage: %s in_file out_file\n", program);
}

int main(int argc, char** argv)
{
  if ((argc == 1) || (argc != 3))
    {
      print_help(argv[0]);
      return 1;
    }

  if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)
      || (strcmp(argv[2], "-h") == 0) || (strcmp(argv[2], "--help") == 0))
    {
      print_help(argv[0]);
      return 0;
    }

  std::stringstream s;

  if (strcmp(argv[1], "-") == 0)
    s << std::cin.rdbuf();
  else
    {
      std::ifstream in(argv[1]);
      s << in.rdbuf();
    }

  const std::string input_string(s.str());
  std::chrono::steady_clock::time_point start;
  if (strcmp(argv[2], "-") == 0)
    {
      start = std::chrono::steady_clock::now();
      std::cout << service(input_string);
    }
  else
    {
      std::ofstream out(argv[2]);
      start = std::chrono::steady_clock::now();
      out << service(input_string);
    }

  const std::chrono::steady_clock::time_point end =
    std::chrono::steady_clock::now();

  printf
    ("time: %lld ns.\n",
     (long long)std::chrono::duration_cast<std::chrono::nanoseconds>
     (end - start)
     .count());

  return 0;
}
