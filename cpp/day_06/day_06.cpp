#include "../utils.h"

unsigned long long total_combinations(std::string_view path)
{
   std::ifstream input(path.data());

   auto inputs = std::ranges::to<std::vector<std::string>>(read_input(input));

   auto times =
      std::ranges::to<std::vector<int>>(
         inputs[0].substr(5) |
         std::views::split(std::string(" ")) |
         std::views::filter([](auto value) { return !value.empty(); }) |
         std::views::transform([](auto value) {
            std::string text(value.begin(), value.end());
            return std::stoi(text);
            }));

   auto distances =
      std::ranges::to<std::vector<int>>(
         inputs[1].substr(9) |
         std::views::split(std::string(" ")) |
         std::views::filter([](auto value) { return !value.empty(); }) |
         std::views::transform([](auto value) {
            std::string text(value.begin(), value.end());
            return std::stoi(text);
            }));

   unsigned long long total = 1;

   for (size_t i = 0; i < times.size(); ++i)
   {
      auto count = std::ranges::count_if(
         std::ranges::iota_view{ 0, times[i] },
         [time=times[i], record=distances[i]](int const velocity) {
            return velocity * (time - velocity) > record; });

      total *= count;
   }
   

   return total;
}

unsigned long long total_combinations2(std::string_view path)
{
   std::ifstream input(path.data());

   auto inputs = std::ranges::to<std::vector<std::string>>(read_input(input));

   auto stime = inputs[0].substr(5);
   stime.erase(std::remove_if(stime.begin(), stime.end(), isspace), stime.end());

   auto sdistance = inputs[1].substr(9);
   sdistance.erase(std::remove_if(sdistance.begin(), sdistance.end(), isspace), sdistance.end());

   auto time = std::stoull(stime);
   auto distance = std::stoull(sdistance);

   unsigned long long lowvelocity = 0;
   for (; lowvelocity < time; ++lowvelocity)
   {
      if (lowvelocity * (time - lowvelocity) > distance)
      {
         break;
      }
   }

   unsigned long long highvelocity = time;
   for (; highvelocity > 0; --highvelocity)
   {
      if (highvelocity * (time - highvelocity) > distance)
      {
         break;
      }
   }

   return highvelocity - lowvelocity + 1;
}

int main()
{
   std::println("Day 6 - Part 1");
   {
      auto test_total = total_combinations("..\\data\\day_06_test.txt");
      assert(test_total == 288);

      auto total = total_combinations("..\\data\\day_06.txt");
      std::println("Total combinations: {}", total);
   }

   std::println("Day 6 - Part 2");
   {
      auto test_total = total_combinations2("..\\data\\day_06_test.txt");
      assert(test_total == 71503);

      auto total = total_combinations2("..\\data\\day_06.txt");
      std::println("Total combinations: {}", total);
   }
}
