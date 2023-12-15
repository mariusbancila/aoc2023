#include "../utils.h"
#include <regex>

int sum_of_ids(std::string_view path, int red, int green, int blue)
{
   std::ifstream input(path.data());

   auto ids = read_input(input) |
      std::views::transform([red, green, blue](auto& line) {
         std::regex regex(R"(Game (\d+): (.*))");
         std::smatch match;
         std::regex_search(line, match, regex);

         int id = std::stoi(match[1]);

         for (auto set : match[2].str() | std::views::split(std::string("; ")))
         {
            std::string settext(set.begin(), set.end());
            trim(settext);

            for (auto pair : settext | std::views::split(std::string(", ")))
            {
               std::string pairtext(pair.begin(), pair.end());
               trim(pairtext);

               auto parts = std::ranges::to<std::vector<std::string>>(pairtext | std::views::split(' '));
               int count = std::stoi(parts[0]);
               if (parts[1] == "red")
               {
                  if (count > red) return 0;
               }
               else if (parts[1] == "green")
               {
                  if (count > green) return 0;
               }
               else if (parts[1] == "blue")
               {
                  if (count > blue) return 0;
               }
            }
         }

         return id;
      });

   auto sum = std::ranges::fold_left(ids, 0, std::plus{});

   return sum;
}

long long sum_of_powers(std::string_view path)
{
   std::ifstream input(path.data());

   auto ids = read_input(input) |
      std::views::transform([](auto& line) {
      std::regex regex(R"(Game (\d+): (.*))");
      std::smatch match;
      std::regex_search(line, match, regex);

      int id = std::stoi(match[1]);

      int maxred = 0;
      int maxgreen = 0;
      int maxblue = 0;

      for (auto set : match[2].str() | std::views::split(std::string("; ")))
      {
         std::string settext(set.begin(), set.end());
         trim(settext);

         for (auto pair : settext | std::views::split(std::string(", ")))
         {
            std::string pairtext(pair.begin(), pair.end());
            trim(pairtext);

            auto parts = std::ranges::to<std::vector<std::string>>(pairtext | std::views::split(' '));
            int count = std::stoi(parts[0]);
            if (parts[1] == "red")
            {
               maxred = std::max(maxred, count);
            }
            else if (parts[1] == "green")
            {
               maxgreen = std::max(maxgreen, count);
            }
            else if (parts[1] == "blue")
            {
               maxblue = std::max(maxblue, count);
            }
         }
      }

      return maxred * maxgreen * maxblue;
         });

   long long sum = std::ranges::fold_left(ids, 0ll, std::plus{});

   return sum;
}

int main()
{
   std::println("Day 2 - Part 1");
   {
      int sum_test = sum_of_ids("..\\data\\day_02_test.txt", 12, 13, 14);
      assert(sum_test == 8);

      int sum = sum_of_ids("..\\data\\day_02.txt", 12, 13, 14);
      std::println("Sum of ids: {}", sum);
   }

   std::println("Day 2 - Part 2");
   {
      long long sum_test = sum_of_powers("..\\data\\day_02_test.txt");
      assert(sum_test == 2286);

      long long sum = sum_of_powers("..\\data\\day_02.txt");
      std::println("Sum of powers: {}", sum);
   }
}
