#include "../utils.h"

constexpr int xdir[]{ -1, 0, 1, -1, 1, -1, 0, 1 };
constexpr int ydir[]{ -1, -1, -1, 0, 0, 1, 1, 1 };

struct part_number
{
   int         number;
   std::size_t row;
   std::size_t start_col;
   std::size_t end_col;
   bool        adjacent;
};

bool is_symbol(char ch)
{
   return ch!= '.' && !std::isalpha(ch) && !std::isdigit(ch);
}

std::tuple<std::vector<std::string>, std::size_t, std::size_t> read_schema(std::string_view path)
{
   std::ifstream input(path.data());
   std::vector<std::string> schema = std::ranges::to<std::vector>(read_input(input));

   std::size_t rows = schema.size();
   std::size_t cols = schema[0].size();

   return { schema, rows, cols };
}

std::tuple<std::vector<std::string>, std::size_t, std::size_t, std::vector<part_number>> find_parts(std::string_view path)
{
   auto [schema, rows, cols] = read_schema(path);
   
   std::vector<part_number> parts;

   for (std::size_t row = 0; row < rows; ++row)
   {
      int number = 0;
      std::size_t length = 0;
      for (std::size_t col = 0; col < cols; ++col)
      {
         if (std::isdigit(schema[row][col]))
         {
            number = number * 10 + (schema[row][col] - '0');
            length++;
         }
         else if (number != 0)
         {
            parts.emplace_back(number, row, col - length, col - 1, false);
            number = 0;
            length = 0;
         }
      }

      // check numbers that are at the end of the row
      if (number != 0)
      {
         parts.emplace_back(number, row, cols - length, cols - 1, false);
         number = 0;
         length = 0;
      }
   }

   return { schema, rows, cols, parts };
}

int sum_of_parts(std::string_view path)
{
   auto [schema, rows, cols, parts] = find_parts(path);

   for (std::size_t row = 0; row < rows; ++row)
   {
      for (std::size_t col = 0; col < cols; ++col)
      {
         if (is_symbol(schema[row][col]))
         {
            for (int i = 0; i < 8; i++)
            {
               int x = static_cast<int>(col) + xdir[i];
               int y = static_cast<int>(row) + ydir[i];

               if (0 <= x && x < cols && 
                     0 <= y && y < rows && 
                     !is_symbol(schema[y][x]))
               {
                  for (auto& part : parts)
                  {
                     if (part.row == y && part.start_col <= x && x <= part.end_col)
                     {
                        part.adjacent = true;
                     }
                  }
               }               
            }
         }
      }
   }

   int sum = std::ranges::fold_left(
      parts, 
      0, 
      [](int sum, const part_number& part) {
         if (part.adjacent)
         {
            return sum + part.number;
         }
         return sum;
      });

   return sum;
}

int sum_of_gear_ratios(std::string_view path)
{
   auto [schema, rows, cols, parts] = find_parts(path);

   int total = 0;

   for (std::size_t row = 0; row < rows; ++row)
   {
      for (std::size_t col = 0; col < cols; ++col)
      {
         if (schema[row][col] == '*')
         {
            std::vector<int> numbers;

            for (int i = 0; i < 8; i++)
            {
               int x = static_cast<int>(col) + xdir[i];
               int y = static_cast<int>(row) + ydir[i];

               if (0 <= x && x < cols &&
                  0 <= y && y < rows &&
                  !is_symbol(schema[y][x]))
               {
                  for (auto& part : parts)
                  {
                     if (part.row == y && part.start_col <= x && x <= part.end_col && !part.adjacent)
                     {
                        numbers.push_back(part.number);
                        part.adjacent = true;
                     }
                  }
               }
            }

            if (numbers.size() == 2)
            {               
               total += numbers[0] * numbers[1];
            }
         }
      }
   }

   return total;
}

int main()
{
   std::println("Day 3 - Part 1");
   {
      int sum_test = sum_of_parts("..\\data\\day_03_test.txt");
      assert(sum_test == 4361);

      int sum = sum_of_parts("..\\data\\day_03.txt");
      std::println("Sum of ids: {}", sum);
   }

   std::println("Day 3 - Part 2");
   {
      long long sum_test = sum_of_gear_ratios("..\\data\\day_03_test.txt");
      assert(sum_test == 467835);

      long long sum = sum_of_gear_ratios("..\\data\\day_03.txt");
      std::println("Sum of gear ratios: {}", sum);
   }
}