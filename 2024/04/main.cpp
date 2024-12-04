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

bool is_valid(vector<string> ls, int x, int dx, int y, int dy) {
  return ls.at(y + dy).at(x + dx) == 'M' &&
         ls.at(y + (2 * dy)).at(x + (2 * dx)) == 'A' &&
         ls.at(y + (3 * dy)).at(x + (3 * dx)) == 'S';
}

size_t problem1(vector<string> lines, vector<tuple<size_t, size_t>> x_indices,
                size_t width, size_t height) {
  size_t occurrences = 0;
  for (auto x_loc : x_indices) {
    size_t line_pos = get<0>(x_loc);
    size_t line_num = get<1>(x_loc);

    if (line_pos >= 3) {
      // Check left
      occurrences += is_valid(lines, line_pos, -1, line_num, 0) ? 1 : 0;

      if (line_num >= 3) {
        // Check diagonal left + up
        occurrences += is_valid(lines, line_pos, -1, line_num, -1) ? 1 : 0;
      }

      if (line_num <= height - 4) {
        // Check diagonal left + down
        occurrences += is_valid(lines, line_pos, -1, line_num, 1) ? 1 : 0;
      }
    }

    if (line_pos <= width - 4) {
      // Check right
      occurrences += is_valid(lines, line_pos, 1, line_num, 0) ? 1 : 0;

      if (line_num >= 3) {
        // Check diagonal right + up
        occurrences += is_valid(lines, line_pos, 1, line_num, -1) ? 1 : 0;
      }

      if (line_num <= height - 4) {
        // Check diagonal right + down
        occurrences += is_valid(lines, line_pos, 1, line_num, 1) ? 1 : 0;
      }
    }

    if (line_num >= 3) {
      // Check up
      occurrences += is_valid(lines, line_pos, 0, line_num, -1) ? 1 : 0;
    }

    if (line_num <= height - 4) {
      // Check down
      occurrences += is_valid(lines, line_pos, 0, line_num, 1) ? 1 : 0;
    }
  }
  return occurrences;
}

size_t problem2(vector<string> lines, vector<tuple<size_t, size_t>> a_indices,
                size_t width, size_t height) {
  size_t occurrences = 0;
  for (auto a_loc : a_indices) {
    size_t line_pos = get<0>(a_loc);
    size_t line_num = get<1>(a_loc);

    // We need to be at least 1 from the border to have a valid position
    if (line_pos >= 1 && line_pos <= height - 2 && line_num >= 1 &&
        line_num <= width - 2) {
      // Get values at the corner; we need to make sure there are 2 M's and 2
      // S's, and that the same letters aren't on any of the diagonals.
      char top_left = lines.at(line_num - 1).at(line_pos - 1);
      char top_right = lines.at(line_num - 1).at(line_pos + 1);
      char bottom_left = lines.at(line_num + 1).at(line_pos - 1);
      char bottom_right = lines.at(line_num + 1).at(line_pos + 1);

      string s;
      s.append(1, top_left)
          .append(1, top_right)
          .append(1, bottom_left)
          .append(1, bottom_right);

      // Invalid characters
      if (count(s.begin(), s.end(), 'M') != 2 ||
          count(s.begin(), s.end(), 'S') != 2) {
        continue;
      }

      // Characters are incorrectly arranged
      if (top_left == bottom_right) {
        continue;
      }

      // If we've gotten here, the occurrence is valid!
      occurrences++;
    }
  }
  return occurrences;
}

void solve() {
  size_t width = 0;
  size_t height = 0;
  vector<string> lines;
  vector<tuple<size_t, size_t>> x_indices;
  vector<tuple<size_t, size_t>> a_indices;

  ifstream fs("2024/04/data/input.txt");
  if (fs.is_open()) {
    string l;
    while (getline(fs, l)) {
      if (!width) {
        width = l.size();
      }

      // Remove newline
      if (!l.empty() && l[l.length() - 1] == '\n') {
        l.erase(l.length() - 1);
      }

      // Check all characters, find locations of all X values
      for (size_t i = 0; i < l.size(); i++) {
        char c = l.at(i);

        if (c == 'X') {
          x_indices.push_back(tuple<size_t, size_t>(i, height));
        } else if (c == 'A') {
          a_indices.push_back(tuple<size_t, size_t>(i, height));
        }
      }

      lines.push_back(l);
      height++;
    }
  } else {
    LOG(FATAL) << "Error: Unable to open file." << endl;
  }
  fs.close();

  size_t occurrences1 = problem1(lines, x_indices, width, height);
  VLOG(1) << "Answer1: " << occurrences1 << endl;

  size_t occurrences2 = problem2(lines, a_indices, width, height);
  VLOG(1) << "Answer2: " << occurrences2 << endl;
}

int main(int argc, char *argv[]) {
  configure(argc, argv);
  solve();
  return 0;
}