#include <algorithm>
#include <fstream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void configure(int argc, char *argv[]) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // Optional: parse command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

struct pos {
  size_t x;
  size_t y;

  // Operator overloading for comparison (required for set)
  bool operator<(const pos &other) const {
    if (x == other.x) {
      return y < other.y;
    }
    return x < other.x;
  }
};

struct pos2 {
  size_t x;
  size_t y;
  int direction;

  // Operator overloading for comparison (required for set)
  bool operator<(const pos2 &other) const {
    if (x == other.x) {
      if (y == other.y) {
        return direction < other.direction;
      }
      return y < other.y;
    }
    return x < other.x;
  }
};

void move1(vector<string> map, pos p, set<pos> &visited, int direction) {
  // Directions:
  //   0
  // 3   1
  //   2

  // Mark the current location as visited
  visited.insert(p);

  // Check if you're on an edge and about to leave
  if ((direction == 0 && p.y == 0) ||
      (direction == 1 && p.x == map[0].size() - 1) ||
      (direction == 2 && p.y == map.size() - 1) ||
      (direction == 3 && p.x == 0)) {
    return;
  }

  // Check if you're in front of an obstacle and need to turn.
  // You know you're not an edge and look ahead 1 now
  if ((direction == 0 && map.at(p.y - 1).at(p.x) == '#') ||
      (direction == 1 && map.at(p.y).at(p.x + 1) == '#') ||
      (direction == 2 && map.at(p.y + 1).at(p.x) == '#') ||
      (direction == 3 && map.at(p.y).at(p.x - 1) == '#')) {
    // If so, rotate and move again
    return move1(map, p, visited, (direction + 1) % 4);
  }

  // Otherwise, move in your current direction.
  switch (direction) {
  case 0:
    p.y--;
    break;
  case 1:
    p.x++;
    break;
  case 2:
    p.y++;
    break;
  case 3:
    p.x--;
    break;
  }

  return move1(map, p, visited, direction);
}

void mark(vector<string> map, pos2 p, set<pos2> &visited) {
  // Directions:
  //   0
  // 3   1
  //   2

  // In mark mode, we are moving in reverse and marking everything as "visited"
  // with the opposite distance that we're travelling

  // Mark the current location as visited
  visited.insert(p);

  // Check if you're on an edge and about to leave
  if ((p.direction == 0 && p.y == map.size() - 1) ||
      (p.direction == 1 && p.x == 0) || (p.direction == 2 && p.y == 0) ||
      (p.direction == 3 && p.x == map[0].size() - 1)) {
    return;
  }

  // Check if you're on an adjacent edge. If not, check if
  // there is an obstacle adjacent to you, and continue marking
  // as if you bounced off said obstacle.
  if ((p.direction == 0 && p.x > 0 && map.at(p.y).at(p.x - 1) == '#') ||
      (p.direction == 1 && p.y > 0 && map.at(p.y - 1).at(p.x) == '#') ||
      (p.direction == 2 && p.x < map[0].size() - 1 &&
       map.at(p.y).at(p.x + 1) == '#') ||
      (p.direction == 3 && p.y < map.size() - 1 &&
       map.at(p.y + 1).at(p.x) == '#')) {
    // If so, mark everything that can bounce off this obstacle as well
    pos2 rotated_pos = {p.x, p.y, (p.direction + 3) % 4};
    mark(map, rotated_pos, visited);
  }

  // Check if you're in front of an obstacle and need to turn.
  // You know you're not an edge and look ahead 1 now
  if ((p.direction == 0 && map.at(p.y + 1).at(p.x) == '#') ||
      (p.direction == 1 && map.at(p.y).at(p.x - 1) == '#') ||
      (p.direction == 2 && map.at(p.y - 1).at(p.x) == '#') ||
      (p.direction == 3 && map.at(p.y).at(p.x + 1) == '#')) {
    // If so, we're done marking
    return;
  }

  // Otherwise, move in your current direction.
  switch (p.direction) {
  case 0:
    p.y++;
    break;
  case 1:
    p.x--;
    break;
  case 2:
    p.y--;
    break;
  case 3:
    p.x++;
    break;
  }

  return mark(map, p, visited);
}

void move2(vector<string> map, pos2 p, set<pos2> &visited,
           set<pos2> &obstacle_locs) {
  // Directions:
  //   0
  // 3   1
  //   2

  // Mark the current location as visited
  visited.insert(p);

  // Check if you're on an edge and about to leave
  if ((p.direction == 0 && p.y == 0) ||
      (p.direction == 1 && p.x == map[0].size() - 1) ||
      (p.direction == 2 && p.y == map.size() - 1) ||
      (p.direction == 3 && p.x == 0)) {
    return;
  }

  // Check if you're in front of an obstacle and need to turn.
  // You know you're not an edge and look ahead 1 now
  if ((p.direction == 0 && map.at(p.y - 1).at(p.x) == '#') ||
      (p.direction == 1 && map.at(p.y).at(p.x + 1) == '#') ||
      (p.direction == 2 && map.at(p.y + 1).at(p.x) == '#') ||
      (p.direction == 3 && map.at(p.y).at(p.x - 1) == '#')) {
    // If so, mark everything behind you as "visited" and continue.
    // This handles loops that aren't overlapping
    mark(map, p, visited);

    pos2 rotated_pos = {p.x, p.y, (p.direction + 1) % 4};
    return move2(map, rotated_pos, visited, obstacle_locs);
  }

  // If you've moved into a position you've already visited (excluding the
  // start), placing a blocker in the next square (as long as it's not on the
  // edge) will make you loop
  bool place_obstacle = false;
  pos2 rotated_pos = {p.x, p.y, (p.direction + 1) % 4};
  if (visited.find(rotated_pos) != visited.end())
    place_obstacle = true;

  // Otherwise, move in your current direction.
  switch (p.direction) {
  case 0:
    p.y--;
    break;
  case 1:
    p.x++;
    break;
  case 2:
    p.y++;
    break;
  case 3:
    p.x--;
    break;
  }

  if (place_obstacle) {
    obstacle_locs.insert(p);
    VLOG(2) << "Overlapping at (" << p.x << "," << p.y << ")" << endl;
  }

  return move2(map, p, visited, obstacle_locs);
}

vector<int> parse_line(string l, string del) {
  vector<int> tokens;
  size_t pos = 0;
  while ((pos = l.find(del)) != string::npos) {
    string t = l.substr(0, pos);
    tokens.push_back(stoi(t));
    l.erase(0, pos + del.length());
  }
  tokens.push_back(stoi(l));

  return tokens;
}

void solve() {
  pos start_pos = {0, 0};
  vector<string> map;
  ifstream fs("2024/06/data/input.txt");
  if (fs.is_open()) {
    string l;
    bool start_pos_found = false;
    while (getline(fs, l)) {
      // Remove newline
      if (!l.empty() && l[l.length() - 1] == '\n') {
        l.erase(l.length() - 1);
      }

      // Check if the starting position is in this row
      if (find(l.begin(), l.end(), '^') != l.end()) {
        start_pos.x = distance(l.begin(), find(l.begin(), l.end(), '^'));
        start_pos_found = true;
      }

      map.push_back(l);
      start_pos.y += start_pos_found ? 0 : 1;
    }

    fs.close();
  } else {
    LOG(FATAL) << "Error: Unable to open file." << endl;
  }

  {
    // Recurse to move the person, starting by moving up
    set<pos> visited;
    move1(map, start_pos, visited, 0);
    VLOG(1) << "Answer1: " << visited.size() << endl;
  }

  {
    // Recurse to move the person, starting by moving up
    pos2 start_pos2 = {start_pos.x, start_pos.y, 0};
    set<pos2> visited;
    set<pos2> obstacle_locs;
    move2(map, start_pos2, visited, obstacle_locs);
    VLOG(1) << "Answer2: " << obstacle_locs.size() << endl;
  }
}

int main(int argc, char *argv[]) {
  configure(argc, argv);
  solve();
  return 0;
}