#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

using namespace std;

void configure(int argc, char* argv[]) {
    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // Optional: parse command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);
}

void solve() {
    ifstream fs("2024/03/data/input.txt");
    
    if (fs.is_open()) {
        string l;
        string del = " ";

        while (getline(fs, l)) {
            vector<string> tokens;

            // Remove newline
            if (!l.empty() && l[l.length() - 1] == '\n') {
                l.erase(l.length() - 1);
            }

            // Iterate through string, extracting tokens into a vector
            size_t pos = 0;
            while ((pos = l.find(del)) != string::npos) {
                string t = l.substr(0, pos);
                tokens.push_back(t);
                l.erase(0, pos + del.length());
            }
        }

        VLOG(1) << "Answer: " << endl;

        fs.close();
    } else {
        LOG(FATAL) << "Error: Unable to open file." << endl;
    }
}

int main() {
    solve();
    return 0;
}