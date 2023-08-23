#include <iostream>
#include <string>
using namespace std;

bool visible(char c) {
    if (c == 127 || c < 33)
        return false;
    else
        return true;
}

bool upper(char c) {
    if (c > 64 && c < 91)
        return true;
    else
        return false;
}

bool lower(char c) {
    if (c > 96 && c < 123)
        return true;
    else
        return false;
}

char toupper(char c) {
    if (lower(c))
        return c - 32;
    else
        return c;
}

char tolower(char c) {
    if (upper(c))
        return c + 32;
    else
        return c;
}

string one_strip(string s) {
    int lflag = 0;
    string s2;
    for (int k = 0; k < s.size(); k++) {
        if (lflag == 0) {
            if (visible(s[k])) {
                lflag = 1;
                s2 += s[k];
            }
        } else {
            if (visible(s[k]) || s[k] == 32)
                s2 += s[k];
        }
    }
    return s2;
}

string rvs(string s) {
    string s2;
    for (int k = s.size() - 1; k > -1; k--) {
        s2 += s[k];
    }
    return s2;
}

string full_strip(string s) {
    string str = one_strip(s);
    str = rvs(str);
    str = one_strip(str);
    str = rvs(str);
    return str;
}
