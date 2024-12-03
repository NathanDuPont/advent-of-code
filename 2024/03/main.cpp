#include <fstream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

void configure(int argc, char *argv[]) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // Optional: parse command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

int problem1(string l) {
  regex re("mul\\((\\d+),(\\d+)\\)");

  int csum = 0;

  for (sregex_iterator itr = sregex_iterator(l.begin(), l.end(), re);
       itr != sregex_iterator(); itr++) {
    smatch match;
    match = *itr;
    VLOG(2) << "Match: " << match.str(0) << " | Num 1: " << match.str(1)
            << " | Num 2: " << match.str(2) << endl;

    int a = stoi(match.str(1));
    int b = stoi(match.str(2));
    csum += a * b;
  }

  return csum;
}

int problem2(string l, bool &is_enabled) {
  regex re("mul\\((\\d+),(\\d+)\\)|(do\\(\\))|(don't\\(\\))");

  int csum = 0;

  for (sregex_iterator itr = sregex_iterator(l.begin(), l.end(), re);
       itr != sregex_iterator(); itr++) {
    smatch match;
    match = *itr;
    VLOG(2) << "Match: " << match.str(0) << " | Num 1: " << match.str(1)
            << " | Num 2: " << match.str(2) << endl;

    string match_str = match.str(0);
    if (match_str == "don't()") {
      is_enabled = false;
      continue;
    } else if (match_str == "do()") {
      is_enabled = true;
      continue;
    }

    int a = stoi(match.str(1));
    int b = stoi(match.str(2));
    csum += is_enabled ? a * b : 0;
  }

  return csum;
}

void solve() {
  ifstream fs("2024/03/data/input.txt");

  if (fs.is_open()) {
    bool is_enabled = true;
    int csum = 0;
    string l;
    while (getline(fs, l)) {
      // Remove newline
      if (!l.empty() && l[l.length() - 1] == '\n') {
        l.erase(l.length() - 1);
      }

      // csum += problem1(l);
      csum += problem2(l, is_enabled);
    }

    VLOG(1) << "Answer: " << csum << endl;

    fs.close();
  } else {
    LOG(FATAL) << "Error: Unable to open file." << endl;
  }
}

int main(int argc, char *argv[]) {
  configure(argc, argv);
  solve();
  return 0;
}