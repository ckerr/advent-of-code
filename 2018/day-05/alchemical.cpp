#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
#include <string_view>

size_t solve_part_1(std::string_view const& instr, std::string_view const& linstr) {
  struct unit { char ch; char lower; };
  std::vector<unit> out;
  out.reserve(instr.size() + 1);
  auto* o = out.data();
  const auto* const obegin = o;

  for (auto in=instr.begin(), lin=linstr.begin(); in != instr.end(); ++in, ++lin) {
    o->ch = *in;
    o->lower = *lin;
    ++o;

    // remove reactions
    for (;;) {
      if (o - obegin < 2) break;
      if (o[-1].ch == o[-2].ch) break;
      if (o[-1].lower != o[-2].lower) break;
      o -= 2;
    }
  }

	return o - obegin;
}

int solve_part_2(std::string_view const& instr, std::string_view const& linstr) {
  auto shortest = std::optional<size_t>{};

  auto buf1 = std::vector<char>(instr.size());
  buf1.clear();
  auto buf2 = std::vector<char>(instr.size());
  buf2.clear();

  char* obegin = buf2.data();
  char* lobegin = buf2.data();
  for (char exclude : "abcdefghijklmnopqrstuvwxyz") {
    // char upper = std::toupper(exclude);
    // build the string
    char* o = obegin;
    char* lo = lobegin;
    for (auto in=instr.begin(), lin=linstr.begin(); in != instr.end(); ++in, ++lin) {
      if (exclude == *lin) continue;
      *o++ = *in;
      *lo++ = *lin;
    }

    // test it
    auto const length = solve_part_1(std::string_view(obegin, o - obegin), std::string_view(lobegin, lo - lobegin));
    if (!shortest || shortest > length) shortest = length;
  }

  return *shortest;
}

int main(){
	std::string input; // = "dabAcCaCBAcCcaDA";
  std::ifstream infile("input.txt");
  std::getline(infile, input);
  std::string lower;
  std::transform(std::begin(input), std::end(input), std::back_inserter(lower), [](char ch){return std::tolower(ch);});
  std::cout << "solution 1: " << solve_part_1(input, lower) << std::endl;
  std::cout << "solution 2: " << solve_part_2(input, lower) << std::endl;
  return 0;
}
