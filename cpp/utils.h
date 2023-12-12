// streams
#include <iostream>
#include <fstream>
// algorithms
#include <algorithm>
#include <ranges>
// formatting
#include <format>
#include <print>
// containers
#include <string>
#include <string_view>
#include <vector>
#include <map>
// utilities
#include <cassert>

#ifdef __cpp_lib_generator
#include <generator>

std::generator<std::string> read_input(std::istream& input)
{
   std::string line;
   while (std::getline(input, line))
   {
      co_yield line;
   }
}
#else
std::vector<std::string> read_input(std::istream& input)
{
   std::vector<std::string> lines;
   std::string line;
   while (std::getline(input, line))
   {
      lines.push_back(line);
   }
   return lines;
}
#endif