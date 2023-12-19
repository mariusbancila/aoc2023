#include "../utils.h"
#include <regex>

// Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1

int total_points(std::string_view path)
{
   std::ifstream input(path.data());

   auto points = read_input(input) |
      std::views::transform([](auto& line) {
         std::regex regex(R"(Card\s+(\d+): (.+) \| (.+))");
         std::smatch match;
         std::regex_search(line, match, regex);

         int id = std::stoi(match[1]);
         
         auto winners = 
            std::ranges::to<std::vector<int>>(
               match[2].str() | 
               std::views::split(std::string(" ")) |
               std::views::filter([](auto value) { return !value.empty(); }) |
               std::views::transform([](auto value) {
                  std::string cardtext(value.begin(), value.end());
                  return std::stoi(cardtext);
               }));

         auto numbers =
            std::ranges::to<std::vector<int>>(
               match[3].str() |
               std::views::split(std::string(" ")) |
               std::views::filter([](auto value) { return !value.empty(); }) |
               std::views::transform([](auto value) {
                  std::string cardtext(value.begin(), value.end());
                  return std::stoi(cardtext);
               }));

         // count how many numbers are in the winners list
         int count = static_cast<int>(std::ranges::count_if(numbers, [&winners](int number) {
            return std::ranges::find(winners, number) != winners.end();
         }));

         return count > 0 ? std::pow(2, count - 1) : 0;
      });

   return static_cast<int>(std::ranges::fold_left(points, 0, std::plus{}));
}

int total_scratchboards(std::string_view path)
{
   std::ifstream input(path.data());

   auto scratchboards = std::ranges::to<std::vector<std::pair<int, int>>>(read_input(input) |
      std::views::transform([](auto& line) {
         std::regex regex(R"(Card\s+(\d+): (.+) \| (.+))");
         std::smatch match;
         std::regex_search(line, match, regex);

         int id = std::stoi(match[1]);

         auto winners =
            std::ranges::to<std::vector<int>>(
               match[2].str() |
               std::views::split(std::string(" ")) |
               std::views::filter([](auto value) { return !value.empty(); }) |
               std::views::transform([](auto value) {
                  std::string cardtext(value.begin(), value.end());
                  return std::stoi(cardtext);
                  }));

         auto numbers =
            std::ranges::to<std::vector<int>>(
               match[3].str() |
               std::views::split(std::string(" ")) |
               std::views::filter([](auto value) { return !value.empty(); }) |
               std::views::transform([](auto value) {
                  std::string cardtext(value.begin(), value.end());
                  return std::stoi(cardtext);
                  }));

         // count how many numbers are in the winners list
         int count = static_cast<int>(std::ranges::count_if(numbers, [&winners](int number) {
            return std::ranges::find(winners, number) != winners.end();
            }));

         return std::pair<int, int>(id, count);
      }));

   int originals = std::ranges::max_element(
      scratchboards, 
      std::less{}, 
      [](auto& point) { return point.first; })->first;

   std::map<int, int> counts =
      std::ranges::to<std::map<int, int>>(
         std::views::iota(1, originals + 1) |
         std::views::transform([](int const n) {
            return std::pair<int, int>(n, 1);
            }));

   for (auto [id, wins] : scratchboards)
   {
      for (int i = 1; i <= wins; ++i)
      {
         counts[id + i] += counts[id];
      }
   }

   int total = std::ranges::fold_left(counts | std::views::values, 0, std::plus{});

   return total;
}

int main()
{
   std::println("Day 4 - Part 1");
   {
      int points_test = total_points("..\\data\\day_04_test.txt");
      assert(points_test == 13);

      int points = total_points("..\\data\\day_04.txt");
      std::println("Total points: {}", points);
   }

   std::println("Day 4 - Part 2");
   {
      int scratchboards_test = total_scratchboards("..\\data\\day_04_test.txt");
      assert(scratchboards_test == 30);

      int scratchboards = total_scratchboards("..\\data\\day_04.txt");
      std::println("Total scratchboards: {}", scratchboards);
   }
}
