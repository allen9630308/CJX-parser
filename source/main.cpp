#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ll_string.hpp"
#include "parser_lib.hpp"
using namespace std;
extern int flag;
extern vector<vector<string> > data;

int main() {
    string line, in_filename, out_filename, in_genre, out_genre;
    cin >> in_filename >> out_filename;
    fstream file;
    int lnn = 0;
    int pos = in_filename.find(".", 0);
    in_genre = in_filename.substr(pos + 1, in_filename.size() - (pos + 1));
    pos = out_filename.find(".", 0);
    out_genre = out_filename.substr(pos + 1, out_filename.size() - (pos + 1));
    file.open(in_filename);
    if (in_genre == "json") {
        while (getline(file, line)) {
            lnn++;
        }
    }
    file.close();
    file.open(in_filename);
    while (getline(file, line)) {
        line = full_strip(line);
        reads(line, in_genre, lnn);
    }
    if (flag != 0)
        cout << "input valid";
    turn();
    file.close();
    if (!output(data, out_filename, out_genre)) {
        cout << "error occur";
    }
}
