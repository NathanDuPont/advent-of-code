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
  vector<tuple<int, int>> rules;
  map<int, set<int>> rule_mapping;
  vector<vector<int>> updates;

  ifstream fs("2024/05/data/input.txt");
  if (fs.is_open()) {
    string l;
    bool is_parsing_rules = true;

    while (getline(fs, l)) {
      // Remove newline
      if (!l.empty() && l[l.length() - 1] == '\n') {
        l.erase(l.length() - 1);
      }

      if (is_parsing_rules) {
        if (l.empty()) {
          is_parsing_rules = false;
          continue;
        }

        vector<int> rule_tokens = parse_line(l, "|");
        rules.push_back(make_tuple(rule_tokens.at(0), rule_tokens.at(1)));

        int num_before = rule_tokens.at(0);
        int num_after = rule_tokens.at(1);

        // We store each number in a map, tracking all numbers that
        // must come before it
        if (rule_mapping.find(num_before) == rule_mapping.end()) {
          set<int> v = {num_after};
          rule_mapping.insert(make_pair(num_before, v));
        } else {
          rule_mapping[num_before].insert(num_after);
        }
      } else {
        vector<int> update_tokens = parse_line(l, ",");
        updates.push_back(update_tokens);
      }
    }

    fs.close();
  } else {
    LOG(FATAL) << "Error: Unable to open file." << endl;
  }

  // For each sequence of updates, check if they are valid and return
  // the median value.
  int valid_median_sum = 0;
  int invalid_median_sum = 0;
  vector<int> invalid_sequences;
  for (auto update : updates) {
    set<int> valid_pages;
    bool is_valid = true;
    for (auto v : update) {
      if (valid_pages.size() == 0) {
        valid_pages.insert(v);
        continue;
      }

      if (rule_mapping.find(v) == rule_mapping.end()) {
        valid_pages.insert(v);
        continue;
      }

      set<int> dependencies = rule_mapping.at(v);

      for (auto d : dependencies) {
        if (valid_pages.find(d) != valid_pages.end()) {
          // We have broken a dependency, mark as invalid.
          is_valid = false;
          break;
        }
      }

      if (!is_valid) {
        break;
      }

      valid_pages.insert(v);
    }

    if (is_valid) {
      valid_median_sum += update.at((update.size() / 2));
    } else {
      // Loop through again and create a corrected order
      vector<int> valid_sequence;
      set<int> visited_pages;
      for (auto v : update) {
        if (visited_pages.size() == 0) {
          valid_sequence.push_back(v);
          visited_pages.insert(v);
          continue;
        }

        if (rule_mapping.find(v) == rule_mapping.end()) {
          valid_sequence.push_back(v);
          visited_pages.insert(v);
          continue;
        }

        set<int> dependencies = rule_mapping.at(v);

        size_t earliest_position = valid_sequence.size();
        for (auto d : dependencies) {
          if (visited_pages.find(d) != visited_pages.end()) {
            // We have broken a dependency, place it before all of it's
            // dependencies.
            size_t offset =
                distance(valid_sequence.begin(),
                         find(valid_sequence.begin(), valid_sequence.end(), d));

            earliest_position =
                offset < earliest_position ? offset : earliest_position;
          }
        }

        valid_sequence.insert(valid_sequence.begin() + earliest_position, v);
        visited_pages.insert(v);
      }

      invalid_median_sum += valid_sequence.at((valid_sequence.size() / 2));
    }
  }

  VLOG(1) << "Answer1: " << valid_median_sum
          << " | Answer2: " << invalid_median_sum << endl;
}

int main(int argc, char *argv[]) {
  configure(argc, argv);
  solve();
  return 0;
}