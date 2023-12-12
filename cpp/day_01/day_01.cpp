#include "../utils.h"

int total_calibration(std::string_view path)
{
   std::ifstream input(path.data());

   auto line_values = read_input(input) |
      std::views::transform([](auto& line) {
         auto first = std::ranges::find_if(line, isdigit);
         auto last = std::ranges::find_if(line | std::views::reverse, isdigit);

         if (first != std::ranges::end(line) &&
            last != std::ranges::rend(line))
         {
            return (*first - '0') * 10 + (*last - '0');
         }
         return 0;
      });

   auto total = std::ranges::fold_left(line_values, 0, std::plus{});

   return total;
}

int total_calibration2(std::string_view path)
{
   std::map<std::string, int> digits
   {
      {"one", 1},
      {"two", 2},
      {"three", 3},
      {"four",4},
      {"five", 5},
      {"six", 6},
      {"seven",7},
      {"eight",8},
      {"nine",9}
   };
   
   auto has_text_digit = [&digits](auto range) {
      for (auto kvp : digits)
      {
         if (std::ranges::starts_with(range, kvp.first))
            return kvp.second;
      }

      return 0;
      };

   auto has_text_digit_reverse = [&digits](auto range) {
      for (auto kvp : digits)
      {
         if (std::ranges::starts_with(range, kvp.first | std::views::reverse))
            return kvp.second;
      }

      return 0;
      };

   std::ifstream input(path.data());

   auto line_values = read_input(input) |
      std::views::transform([&has_text_digit, &has_text_digit_reverse](auto& line) {      
         auto first = 0;
         for (auto it = std::ranges::begin(line); it != std::ranges::end(line); ++it)
         {
            if (std::isdigit(*it))
            {
               first = *it - '0';
               break;
            }
            else
            {
               first = has_text_digit(std::ranges::subrange(it, std::ranges::end(line)));
               if(first != 0)
                  break;
            }
         }

         auto last = 0;
         for (auto it = std::ranges::rbegin(line); it != std::ranges::rend(line); ++it)
         {
            if (std::isdigit(*it))
            {
               last = *it - '0';
               break;
            }
            else
            {
               last = has_text_digit_reverse(std::ranges::subrange(it, std::ranges::rend(line)));
               if(last != 0)
                  break;
            }
         }
      
         return first * 10 + last;
      });
   
   auto total = std::ranges::fold_left(line_values, 0, std::plus{});

   return total;
}

int main()
{
   std::println("Day 1 - Part 1");
   {
      auto test_cal = total_calibration("..\\data\\day_01_1_test.txt");
      assert(test_cal == 142);

      auto cal = total_calibration("..\\data\\day_01.txt");
      std::println("Calibration sum: {}", cal);
   }

   std::println("Day 1 - Part 2");
   {
      auto test_cal = total_calibration2("..\\data\\day_01_2_test.txt");
      assert(test_cal == 281);

      auto cal = total_calibration2("..\\data\\day_01.txt");
      std::println("Calibration sum: {}", cal);
   }
}

