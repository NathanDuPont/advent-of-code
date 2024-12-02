#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

void configure() {
    // TODO any configuration options you want here
}

void solve() {
    filesystem::path input_path = filesystem::current_path() / "data/input.txt";
    cout << input_path << endl;
    ifstream input_file("2024/01/data/input.txt");
    
    if (input_file.is_open()) {
        string line;

        // Read the file line by line
        while (getline(input_file, line)) {
            //cout << line << endl;
        }

        input_file.close();
    } else {
        cerr << "Error: Unable to open file." << endl;
    }
}

int main() {
    configure();
    solve();
    return 0;
}