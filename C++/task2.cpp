#include "task2.h"
#include "hash.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int ChainHash::h(const string& s) const {
    return (int)(hashString(s) % CAP);
}

void ChainHash::add(const string& s) {
    for (Node<string>* cur = table[h(s)].head; cur; cur = cur->next) {
        ++attempts;
        if (cur->value == s)
            return;
    }
    table[h(s)].FPushFront(s);
}

void ChainHash::del(const string& s) {
    table[h(s)].FDelValue(s);
}

bool ChainHash::contains(const string& s) {
    for (Node<string>* cur = table[h(s)].head; cur; cur = cur->next) {
        ++attempts;
        if (cur->value == s)
            return true;
    }
    return false;
}

void ChainHash::clear() {
    for (int i = 0; i < CAP; ++i)
        table[i].clear();
}

void task2(int argc, char** argv) {
    string file, query;
    for (int i = 2; i < argc; ++i) {
        string a = argv[i];
        if (a == "--file" && i + 1 < argc)
            file = argv[++i];
        else if (a == "--query" && i + 1 < argc)
            query = argv[++i];
        else {
            cout << "ОШИБКА: неизвестный аргумент '" << a << "'" << endl;
            return;
        }
    }

    ChainHash s;

    if (!file.empty()) {
        ifstream in(file);
        if (!in.is_open()) {
            cout << "ОШИБКА: не удалось открыть файл " << file << endl;
            return;
        }
        string line;
        while (getline(in, line)) {
            string op, value;
            stringstream ss(line);
            if (!(ss >> op >> value)) {
                if (!line.empty())
                    cout << "ОШИБКА: строка '" << line << "' — ожидалось <op> <value>" << endl;
                continue;
            }
            string extra;
            if (ss >> extra) {
                cout << "ОШИБКА: лишние данные в строке '" << line << "'" << endl;
                continue;
            }
            if (op == "SETADD")
                s.add(value);
            else if (op == "SETDEL")
                s.del(value);
            else
                cout << "ОШИБКА: неизвестная операция '" << op << "'" << endl;
        }
    }

    if (query.empty()) {
        string op, value;
        while (cin >> op >> value) {
            if (op == "SETADD")
                s.add(value);
            else if (op == "SETDEL")
                s.del(value);
            else if (op == "SET_AT")
                cout << (s.contains(value) ? "TRUE" : "FALSE") << endl;
            else
                cout << "ОШИБКА: неизвестная операция '" << op << "'" << endl;
        }
    }
    else {
        string op, value;
        stringstream ss(query);
        if (!(ss >> op >> value)) {
            cout << "ОШИБКА: запрос должен быть <op> <value>" << endl;
            return;
        }
        string extra;
        if (ss >> extra) {
            cout << "ОШИБКА: лишние данные в запросе" << endl;
            return;
        }

        if (op == "SETADD")
            s.add(value);
        else if (op == "SETDEL")
            s.del(value);
        else if (op == "SET_AT")
            cout << (s.contains(value) ? "TRUE" : "FALSE") << endl;
        else {
            cout << "ОШИБКА: неизвестная операция '" << op << "'" << endl;
            return;
        }

        if (!file.empty()) {
            ofstream out(file, ios::app);
            if (!out.is_open()) {
                cout << "ОШИБКА: не удалось открыть файл для записи" << endl;
                return;
            }
            out << query << '\n';
        }
    }

    s.clear();
}
