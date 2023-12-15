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

std::vector<std::string> read_input(std::string_view path)
{
   std::ifstream input(path.data());

#ifdef __cpp_lib_generator
   return std::ranges::to< std::vector<std::string>>(read_input(input));
#else
   return read_input(input);
#endif
}

void trim(std::string& s)
{
   auto not_space = [](unsigned char c) { return !std::isspace(c); };

   // erase the the spaces at the back first
   // so we don't have to do extra work
   s.erase(
      std::ranges::find_if(s | std::views::reverse, not_space).base(),
      s.end());

   // erase the spaces at the front
   s.erase(
      s.begin(),
      std::ranges::find_if(s, not_space));
}