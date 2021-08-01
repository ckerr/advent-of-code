#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct Coordinate {
  int col = 0;
  int row = 0;
};

bool operator< (Coordinate const& a, Coordinate const& b) {
  if (a.col != b.col) return a.col < b.col;
  if (a.row != b.row) return a.row < b.row;
  return false;
}

bool operator== (Coordinate const& a, Coordinate const& b) {
  return !(a < b) && !(b < a);
}

using Coordinates = std::vector<Coordinate>;

int axisDistance(int a, int b) {
  return int(std::fabs(a - b));
}

size_t getDistance(Coordinate const& a, Coordinate const& b) {
  return axisDistance(a.col, b.col) + axisDistance(a.row, b.row);
}

Coordinates getPerimeter(Coordinate const& center, int range) {
  if (range == 0) return { center };

  auto ret = Coordinates{};
  int row = center.row - range;
  for (int col = center.col - range; col <= center.col + range; ++col) ret.push_back({ col, row });
  row = center.row + range;
  for (int col = center.col - range; col <= center.col + range; ++col) ret.push_back({ col, row });
  int col = center.col - range;
  for (row = center.row - range + 1; row <= center.row + range - 1; ++row) ret.push_back({ col, row });
  col = center.col + range;
  for (row = center.row - range + 1; row <= center.row + range - 1; ++row) ret.push_back({ col, row });
  return ret;
}

Coordinates readCoordinates(std::string const& filename) {
  auto line = std::string {};
  auto input_file = std::ifstream { filename };
  auto coordinates = Coordinates {};
  while (std::getline(input_file, line)) {
    auto iss = std::istringstream { line };
    char comma;
    int col, row;
    iss >> col >> comma >> row;
    coordinates.push_back(Coordinate{col, row});
  }
  return coordinates;
}

Coordinate calculateCenter(Coordinates const& coordinates) {
  auto const first = coordinates.front();
  auto max_col = first.col;
  auto max_row = first.row;
  auto min_col = first.col;
  auto min_row = first.row;

  for (auto const& coordinate : coordinates) {
    min_row = std::min(min_row, coordinate.row);
    max_row = std::max(max_row, coordinate.row);
    min_col = std::min(min_col, coordinate.col);
    max_col = std::max(max_col, coordinate.col);
  }

  std::cerr << "min_col " << min_col << std::endl;
  std::cerr << "max_col " << max_col << std::endl;
  std::cerr << "min_row " << min_row << std::endl;
  std::cerr << "max_row " << max_row << std::endl;

  return {
    min_col + axisDistance(min_col, max_col) / 2,
    min_row + axisDistance(min_row, max_row) / 2
  };
}

std::optional<Coordinate>
findClosest(Coordinates const& candidates, Coordinate const& location) {
  auto distances = std::map<size_t, Coordinates>{};
  for (auto const& candidate : candidates) {
    auto const distance = getDistance(candidate, location);
    distances[distance].push_back(candidate);
  }

  auto winner = std::optional<Coordinate>{};
  auto const& winners = std::begin(distances)->second;
  if (std::size(winners) == 1) winner = winners.front();
  return winner;
}

std::optional<size_t>
calculateArea(Coordinate const& location, Coordinates const& coordinates) {
  auto area = size_t { 1 };

  for (int span = 1; ; ++span) {
    auto const oldArea = area;
    for (auto const& coordinate : getPerimeter(location, span)) {
      auto const closest = findClosest(coordinates, coordinate);
      if (closest && *closest == location) {
        ++area;
      }
    }

    if (area == oldArea) return area;
    if (span > 100) return std::optional<size_t>{};
  }
}

void partOne(Coordinates const& coordinates) {
  auto sizes = std::map<size_t, Coordinates> {};
  for (auto const& coordinate : coordinates) {
    auto const area = calculateArea(coordinate, coordinates);
    if (area) {
      std::cout << *area;
      sizes[*area].push_back(coordinate);
    } else {
      std::cout << "infinite";
    }
    std::cout << std::endl;
  }
  auto const winner = sizes.rbegin();
  std::cout << "max non-infinite size " << winner->first << std::endl;
}

int getDistanceSum(Coordinates const& coordinates, Coordinate const& location) {
  auto distance = size_t {};
  for (auto const& coordinate : coordinates)
    distance += getDistance(coordinate, location);
  return distance;
}

void partTwo(Coordinates const& coordinates) {
  auto const maxDistance = 10000;
  auto matches = std::set<Coordinate>{};
  int lastHit = 0;

  auto const center = calculateCenter(coordinates);
  for (int radius = 0; ; ++radius) {
    for (auto const& coordinate : getPerimeter(center, radius)) {
      auto const distance = getDistanceSum(coordinates, coordinate);
      std::cout << "distance " << distance << std::endl;
      if (distance < maxDistance) {
        matches.insert(coordinate);
        lastHit = radius;
      }
    }

    if (lastHit + 100 < radius) break;
  }

  std::cout << std::size(matches) << std::endl;
}

int main() {
  // auto const coordinates = readCoordinates("test.txt");
  auto const coordinates = readCoordinates("input.txt");
  partOne(coordinates);
  partTwo(coordinates);
}
