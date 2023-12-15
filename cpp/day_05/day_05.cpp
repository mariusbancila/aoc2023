#include "../utils.h"
#include <chrono>

// mappings
//   seed-to-soil
//   soil-to-fertilizer
//   fertilizer-to-water
//   water-to-light
//   light-to-temperature
//   temperature-to-humidity
//   humidity-to-location

// explanation
//   50 98 2
//   destination range start:   50
//   source range start:        98
//   range lenght:              2

struct range_descriptor
{
   std::size_t destination_start;
   std::size_t source_start;
   std::size_t length;
};

range_descriptor parse_range(std::string_view text)
{
   auto numbers = std::ranges::to<std::vector<std::size_t>>(
      text | std::views::split(std::string(" ")) |
      std::views::filter([](auto value) { return !value.empty(); }) |
      std::views::transform([](auto value) {
         std::string text(value.begin(), value.end());
         return std::stoull(text);
         }));

   return range_descriptor{ numbers[0], numbers[1], numbers[2] };
}

using range_map = std::vector<range_descriptor>;

using index_t = unsigned long long;

struct seed_data
{
   std::vector< index_t>   seeds;
   range_map               seed_to_soil;
   range_map               soil_to_fertilizer;
   range_map               fertilizer_to_water;
   range_map               water_to_light;
   range_map               light_to_temperature;
   range_map               temperature_to_humidity;
   range_map               humidity_to_location;
};

bool try_parse(std::string_view path, seed_data& data)
{
   std::vector<std::string> input = read_input(path);

   auto lineIt = input.begin();

   std::string line = *lineIt;

   if (line.starts_with("seeds: "))
   {
      line = line.substr(7);
      data.seeds = std::ranges::to<std::vector<index_t>>(
         line | std::views::split(std::string(" ")) |
         std::views::filter([](auto value) { return !value.empty(); }) |
         std::views::transform([](auto value) {
            std::string text(value.begin(), value.end());
            return std::stoull(text);
            }));
   }
   else
      return false;

   lineIt++;
   line = *++lineIt;
   if (line.starts_with("seed-to-soil map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.seed_to_soil.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("soil-to-fertilizer map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.soil_to_fertilizer.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("fertilizer-to-water map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.fertilizer_to_water.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("water-to-light map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.water_to_light.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("light-to-temperature map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.light_to_temperature.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("temperature-to-humidity map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.temperature_to_humidity.push_back(parse_range(line));
      }
   }
   else
      return false;

   line = *++lineIt;
   if (line.starts_with("humidity-to-location map:"))
   {
      for (; line = *++lineIt, !line.empty();)
      {
         data.humidity_to_location.push_back(parse_range(line));
      }
   }
   else
      return false;

   return true;
}

index_t find_mapped_index(index_t position, range_map const & map)
{
   for (auto const& range : map)
   {
      if (position >= range.source_start && position < range.source_start + range.length)
      {
         return range.destination_start + (position - range.source_start);
      }
   }

   return position;
}

index_t find_location(index_t seed, seed_data const & data)
{
   index_t soil = find_mapped_index(seed, data.seed_to_soil);
   index_t fertilizer = find_mapped_index(soil, data.soil_to_fertilizer);
   index_t water = find_mapped_index(fertilizer, data.fertilizer_to_water);
   index_t light = find_mapped_index(water, data.water_to_light);
   index_t temperature = find_mapped_index(light, data.light_to_temperature);
   index_t humidity = find_mapped_index(temperature, data.temperature_to_humidity);
   index_t location = find_mapped_index(humidity, data.humidity_to_location);

   return location;
}

index_t lowest_location_number(std::string_view path)
{
   seed_data data;
   if (!try_parse(path, data))
      return 0;

   auto locations = std::ranges::to<std::vector<index_t>>(
      data.seeds | std::views::transform([&data](auto seed) {
         return find_location(seed, data);
         }));

   auto pos = std::ranges::min_element(locations);

   if(pos != std::ranges::end(locations))
      return *pos;

   throw std::runtime_error("No lowest location found");
}

index_t lowest_location_number_2(std::string_view path)
{
   seed_data data;
   if (!try_parse(path, data))
      return 0;

   auto locations = std::ranges::to<std::vector<index_t>>(
      data.seeds | 
      std::views::chunk(2) |
      std::views::transform([&data](auto rp) {
         auto seed = rp[0];
         auto length = rp[1];

         index_t minimum = std::numeric_limits<index_t>::max();
         for (auto i = 0; i < length; ++i)
         {
            auto location = find_location(seed + i, data);
            if(location < minimum)
               minimum = location;
         }

         return minimum;
         }));

   auto pos = std::ranges::min_element(locations);

   if (pos != std::ranges::end(locations))
      return *pos;

   throw std::runtime_error("No lowest location found");
}

int main()
{
   // added one extra empty line to the input files to make the parsing easier

   std::println("Day 5 - Part 1");
   {
      index_t location_test = lowest_location_number("..\\data\\day_05_test.txt");
      assert(location_test == 35);

      index_t location = lowest_location_number("..\\data\\day_05.txt");
      std::println("Lowest location number: {}", location);
   }

   std::println("Day 5 - Part 2");
   {
      index_t location_test = lowest_location_number_2("..\\data\\day_05_test.txt");
      assert(location_test == 46);

      auto start = std::chrono::high_resolution_clock::now();
      index_t location = lowest_location_number_2("..\\data\\day_05.txt");
      auto end = std::chrono::high_resolution_clock::now();
      std::println("Lowest location number: {}", location);
      std::println("Elapsed time: {}s", std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
   }
}
