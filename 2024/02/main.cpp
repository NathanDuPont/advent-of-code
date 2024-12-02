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

size_t problem1(vector<string> values) {
    bool is_increasing = true;
    bool is_decreasing = true;
    bool is_within_range = true;
    int prev_num = -1;
    
    for (auto v : values) {
        int num = stoi(v);

        if (prev_num == -1) {
            prev_num = num;
            continue;
        }

        if (num > prev_num) {
            // Increasing, mark decreasing as false
            is_decreasing = false;
        } else if (num < prev_num) {
            // Decreasing, mark increasing as false
            is_increasing = false;
        }

        if (abs(num - prev_num) < 1 || abs(num - prev_num) > 3) {
            // Range is too large
            is_within_range = false;
        }

        prev_num = num;
    }

    if ((is_increasing || is_decreasing) && is_within_range) {
        return 1;
    }
    return 0;
}

size_t problem2(vector<string> original_values) {
    // Check if the sequence is naturally correct
    if (problem1(original_values)) {
        return 1;
    }

    // Otherwise, iterate through each value in the vector, skipping one at a time until
    // the sequence becomes valid.
    for (auto itr_a = original_values.begin(); itr_a != original_values.end(); itr_a++) {
        int idx_a = distance(original_values.begin(), itr_a);

        // Invoke via copy constructor.
        vector<string> values = original_values;
        values.erase(values.begin() + idx_a);

        // Check if the sequence is naturally correct
        if (problem1(values)) {
            return 1;
        }
    }
    return 0;
}

size_t problem2_overcomplicated(vector<string> values) {
    // Two pointer approach. We start with pointers on the first two numbers:
    // | |  
    // a b c
    //
    // If this doesn't work, walk the latter one:
    // |   |
    // a b c
    //
    // If this still doesn't work, also walk the first one:
    //
    //   | |
    // a b c
    //
    // If either of these conditions work, them mark the "safety" as being used, and move on.
    //
    // We also need to consider dropping the last value if its bad. If we get to the last value
    // and we can't make it work, and the "safety" has not been used, drop out early.
    bool prev_is_increasing = true;
    bool is_increasing = true;
    bool prev_is_decreasing = true;
    bool is_decreasing = true;
    bool prev_is_within_range = true;
    bool is_within_range = true;
    bool has_safety_been_used = false;

    for (auto itr_a = values.begin(); itr_a != values.end(); itr_a++) {
        int idx_a = distance(values.begin(), itr_a);

        bool previous_pair_range_invalid = false;

        for (auto itr_b = itr_a + 1; itr_b != values.end(); itr_b++) {
            int idx_b = distance(values.begin(), itr_b);

            int num = stoi(values[idx_a]);
            int next_num = stoi(values[idx_b]);

            cout << "One: " << num << " | Two: " << next_num << endl;

            if (next_num > num) {
                // Increasing, mark decreasing as false
                is_decreasing = false;
            } else if (next_num < num) {
                // Decreasing, mark increasing as false
                is_increasing = false;
            }

            if (abs(next_num - num) < 1 || abs(next_num - num) > 3) {
                // Range is too large
                is_within_range = false;
            }

            // Check which constraints have been violated, if any.
            if ((is_increasing || is_decreasing) && is_within_range) {
                // Constraints not violated! We now check if we skipped a number in the middle of the pointers.
                if (distance(itr_a, itr_b) > 1) {
                    // We need to confirm if a number was skipped.
                    // Ex. skipping 9:
                    //   |   |
                    // 1 2 9 3
                    // or the inverse, skipping 2:
                    // 9 8 2 7
                    // Confirm if the pointers are spaced, and increment appropriately.
                    if (previous_pair_range_invalid) {
                        // cout << "Bumping first pointer by an extra value" << endl;
                        //itr_a++;
                    }

                    // If skipping the second to last value makes the sequence work, mark
                    // it as safe.
                    if (next(itr_b) == values.end()) {
                        return 1;
                    }

                    itr_a++;
                    has_safety_been_used = true;
                } 
                
                // Move to next pair while updating previous constraints.
                prev_is_decreasing = is_decreasing;
                prev_is_increasing = is_increasing;
                prev_is_within_range = is_within_range;
                break;
            } else {
                // Constraints violated, check what to do now.
                cout << "Constraints violated: " << is_decreasing << is_increasing << is_within_range << endl;
                if (has_safety_been_used) {
                    // If we've already used the safety, then exit early as this pattern is unsafe.
                    return 0;
                } 

                // If the distance between the two pointers is 2, we need to break out and force
                // the outer iterator to move by 1. At this point, we mark the safety as being used
                // and keep going. If the next check fails, there is more than one problem with
                // the sequence.
                if (distance(itr_a, itr_b) > 1) {
                    // Before we move the first pointer, we need to make sure we don't just skip a range violation.
                    // Ex.
                    //   |   |
                    // 1 2 8 9
                    // or the inverse:
                    // 9 8 2 1
                    // By bumping the front pointer here, we can skip the problem of 2, but that still breaks the
                    // series. If we violated the range in both cases, the series is invalid.
                    if (previous_pair_range_invalid && itr_a != values.begin()) {
                        return 0;
                    }

                    // If the range was invalid in both cases, 

                    // cout << "Bumping first pointer" << endl;

                    is_decreasing = prev_is_decreasing;
                    is_increasing = prev_is_increasing;
                    is_within_range = prev_is_within_range;

                    has_safety_been_used = true;
                    break;
                }

                // At this point, one value is invalid and we can consider dumping it. Reset the constraints to
                // what they were before, and check surrounding values.
                previous_pair_range_invalid = !is_within_range;            
                is_decreasing = prev_is_decreasing;
                is_increasing = prev_is_increasing;
                is_within_range = prev_is_within_range;

                // We also want to check if the second pointer is at the end. If it is, and we haven't
                // used the safety, skip and mark as safe.
                if (next(itr_b) == values.end()) {
                    // If the safety has been used already and we don't have another number, this sequence
                    // is invalid. Otherwise, drop the last number to make it valid.
                    return has_safety_been_used ? 0 : 1;
                }
            } 
        }
    }
    
    return 1;
}

void solve() {
    ifstream input_file("2024/02/data/input.txt");
    
    if (input_file.is_open()) {
        string line;
        string delimiter = " ";
        size_t num_safe = 0;

        // Read the file line by line
        while (getline(input_file, line)) {
            // Store broken apart values
            vector<string> values;

            string original_line = line;

            // Remove newline
            if (!line.empty() && line[line.length()-1] == '\n') {
                line.erase(line.length()-1);
            }

            // Iterate through string, extracting numbers
            size_t pos = 0;
            string value;

            while ((pos = line.find(delimiter)) != string::npos) {
                value = line.substr(0, pos);
                values.push_back(value);
                line.erase(0, pos + delimiter.length());
            }

            values.push_back(line);
            
            // Now the values are loaded in, let's use them.
            // num_safe += problem1(values);
            int is_safe = problem2(values);
            num_safe += is_safe;

            if (is_safe) {
                VLOG(2) << original_line << " is safe!" << endl;
            } else {
                VLOG(2) << original_line << " is unsafe!" << endl;
            }
         }

        VLOG(1) << num_safe << endl;

        input_file.close();
    } else {
        LOG(FATAL) << "Error: Unable to open file." << endl;
    }
}

int main(int argc, char* argv[]) {
    configure(argc, argv);
    solve();
    return 0;
}