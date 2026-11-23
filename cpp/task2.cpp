#include "task2.h"
#include "structures/hash_table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

void task2(int argc, char** argv) {
    string file, query;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) file = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    StringSet set;
    vector<string> objects;

    if (!file.empty()) {
        ifstream input(file);
        string line;
        while (getline(input, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string op, val;
            if (ss >> op >> val && (op == "SETADD" || op == "SETDEL" || op == "SET_AT")) 
            	line = val;
            if (!set.contains(line)) {
                set.add(line);
                objects.push_back(line);
            }
        }
    }

    auto execute = [&](const string& line) {
        string op, val;
        stringstream ss(line);
        if (!(ss >> op >> val)) return;

        if (op == "SETADD") {
            if (!set.contains(val)) {
                set.add(val);
                objects.push_back(val);
            }
        } else if (op == "SETDEL") {
            if (set.contains(val)) {
                set.del(val);
                objects.erase(remove(objects.begin(), objects.end(), val), objects.end());
            }
        } else if (op == "SET_AT") {
            cout << (set.contains(val) ? "TRUE" : "FALSE") << endl;
        }
    };

    if (!query.empty()) {
        execute(query);
        stringstream ss(query);
        string op;
        ss >> op;
        if (op == "SETADD" || op == "SETDEL") {
            ofstream output(file);
            for (const string& s : objects) output << s << '\n';
        }
    } else {
        string line;
        while (getline(cin, line)) {
            execute(line);
            stringstream ss(line);
            string op;
            ss >> op;
            if (op == "SETADD" || op == "SETDEL") {
                ofstream output(file);
                for (const string& s : objects) output << s << '\n';
            }
        }
    }
    set.clear();
}
