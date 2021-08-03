#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <regex>
#include <vector>

// #define TEST_RUN

struct Rect {
  int id = 0;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
};

struct Point {
  int x = 0;
  int y = 0;
};

bool operator< (Point const& a, Point const& b) {
  if (a.x != b.x) return a.x < b.x;
  if (a.y != b.y) return a.y < b.y;
  return false;
}

std::vector<Rect> readInput(char const* filename) {
  auto rects = std::vector<Rect> {};

  auto infile = std::ifstream { filename };
  auto const pattern = std::regex { "^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)$" };
  auto matches = std::smatch {};
  auto line = std::string {};
  while (std::getline(infile, line)) {
    // std::cout << '[' << line << ']' << std::endl;
    if (std::regex_search(line, matches, pattern)) {
      int pos = 1;
      auto const id = std::stoi(matches[pos++].str());
      auto const x = std::stoi(matches[pos++].str());
      auto const y = std::stoi(matches[pos++].str());
      auto const w = std::stoi(matches[pos++].str());
      auto const h = std::stoi(matches[pos++].str());
      // std::cout << " id " << id << " x " << x << " y " << y << " w " << w << " h " << h << std::endl;
      rects.push_back({ id, x, y, w, h });
    }
  }

  return rects;
}

// this is a pretty inefficient way of doing it
std::vector<Point> getPoints(Rect const& r) {
  auto points = std::vector<Point> {};
  for (int x = r.x; x < r.x + r.w; ++x)
    for (int y = r.y; y < r.y + r.h; ++y)
      points.push_back({x, y});
  return points;
}

void partOne(std::vector<Rect> const& rects) {
  auto claims = std::map<Point, int> {};
  for (auto const& rect: rects) {
    for (auto const& point : getPoints(rect)) {
      ++claims[point];
    }
  }

  auto constexpr minClaims = 2;
  auto const count = std::count_if(
    std::begin(claims),
    std::end(claims),
    [minClaims](auto it){return it.second >= minClaims;}
  );

  std::cout << count << std::endl;
}

void partTwo(std::vector<Rect> const& rects) {
  // point -> claim ids
  auto claims = std::map<Point, std::vector<int>> {};
  for (auto const& rect : rects) {
    for (auto const& point : getPoints(rect)) {
      claims[point].push_back(rect.id);
    }
  }

  // starting off, all claims are candidates
  auto candidates = std::set<int>{};
  for (auto const& rect : rects) candidates.insert(rect.id);

  // remove all claims that overlap with another claim
  for (auto const& it : claims) {
    if (it.second.size() > 1) {
      for (auto const& id : it.second) {
        candidates.erase(id);
      }
    }
  }

  // std::cout << "candidate count: " << candidates.size() << std::endl;
  for (auto const& id : candidates) {
    std::cout << "no overlap: " << id << std::endl;
  }
}

#ifdef TEST_RUN
 #define infile "test.txt"
#else
 #define infile "input.txt"
#endif

int main(){
  auto const rects = readInput(infile);
  partOne(rects);
  partTwo(rects);
}

