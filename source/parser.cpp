#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int table[9][4] = {{2, 0, 1, 5}, {6, 0, 1, 5}, {4, 3, 3, 6},
                   {4, 3, 3, 6}, {2, 8, 6, 7}, {5, 5, 5, 5},
                   {6, 6, 6, 6}, {5, 5, 5, 5}, {0, 0, 0, 0}};
vector<vector<string> > data;

int flag = 0;
string nm_rt, nm_obj;

int charval(char c) {
    if (c == '\"')
        return 0;
    else if (c == ',')
        return 1;
    else
        return 2;
}

vector<vector<string> > label;
vector<string> labelx;
void turn() {
    vector<string> sml_label;
    string str, str2, strtmp;
    for (int k = 0; k < data[0].size(); k++) {
        str = "<";
        str2 = "</";
        strtmp = data[0][k];
        int pos = 0;
        while ((pos = strtmp.find(" ", pos)) != string::npos) {
            strtmp.replace(pos, 1, "_");
        }
        transform(strtmp.begin(), strtmp.end(), strtmp.begin(), ::toupper);
        str += strtmp;
        str += ">";
        str2 += strtmp;
        str2 += ">";
        sml_label.push_back(str);
        sml_label.push_back(str2);
        label.push_back(sml_label);
        sml_label.clear();
    }
}

bool output(vector<vector<string> > wait, string filename, string s) {
    ofstream outfile;
    outfile.open(filename.c_str());
    if (!outfile.is_open())
        return false;
    if (s == "csv") {
        for (int k = 0; k < wait.size(); k++) {
            for (int j = 0; j < wait[k].size(); j++) {
                int pos = 0;
                while ((pos = wait[k][j].find("\"", pos + 2)) != string::npos) {
                    wait[k][j].insert(pos, "\"");
                }
            }
        }
        string s;
        for (int k = 0; k < wait.size(); k++) {
            s.clear();
            for (int j = 0; j < wait[k].size(); j++) {
                s += "\"";
                s += wait[k][j];
                s += "\"";
                if (j != wait.size() - 2)
                    s += ",";
            }
            outfile << s << endl;
        }
    } else if (s == "xml") {
        outfile << "<?xml version=\"1.0\"?>" << endl;
        outfile << "<ROOT>" << endl;
        outfile << "<HEADER>" << endl;
        for (int k = 0; k < label.size(); k++) {
            outfile << "\t" << label[k][0] << wait[0][k] << label[k][1] << endl;
        }
        outfile << "</HEADER>" << endl;
        for (int k = 1; k < wait.size(); k++) {
            outfile << endl << "<INFO>" << endl;
            for (int r = 0; r < label.size(); r++) {
                outfile << "\t" << label[r][0] << wait[k][r] << label[r][1]
                        << endl;
            }
            outfile << "</INFO>" << endl;
        }
        outfile << "</ROOT>";
    } else if (s == "json") {
        string ss, rownum;
        outfile << "{" << endl;
        outfile << "\"header\": [";
        for (int k = 0; k < wait[0].size(); k++) {
            ss = "\"" + wait[0][k] + "\"";
            if (k < wait[0].size() - 1) {
                ss += ",";
            }
            outfile << ss;
        }
        outfile << "]," << endl;
        for (int k = 1; k < wait.size(); k++) {
            rownum = "\"row_" + to_string(k) + "\": [";
            outfile << rownum;
            for (int g = 0; g < wait[k].size(); g++) {
                int pos = 0;
                while ((pos = wait[k][g].find("\"", pos + 2)) != string::npos) {
                    wait[k][g].insert(pos, "\\");
                }
                ss = "\"" + wait[k][g] + "\"";
                if (g < wait[k].size() - 1) {
                    ss += ",";
                }
                outfile << ss;
            }
            if (k < wait.size() - 1)
                outfile << "]," << endl;
            else
                outfile << "]" << endl;
        }
        outfile << "}" << endl;
    } else
        return false;

    outfile.close();
    return true;
}

void react(int& n1, char c1, string& s1, vector<string>& v1) {
    switch (n1) {
        case 0:
            v1.push_back(s1);
            s1.clear();
            break;
        case 1:
        case 3:
        case 4:
            s1 += c1;
            break;
        case 5:
            v1.push_back(s1);
            break;
        case 6:
            v1.clear();
            break;
        case 7:
            s1.erase(s1.size() - 1, 1);
            v1.push_back(s1);
            s1.clear();
            n1 = 5;
            break;
        case 8:
            s1.erase(s1.size() - 1, 1);
            v1.push_back(s1);
            s1.clear();
            n1 = 0;
            break;
    };
}

vector<string> sectorx;

void reads(string s, string s_g, int nm) {
    if (s.size() == 0) {
        return;
    }
    if (s_g == "csv") {
        string sub;
        vector<string> sector;
        int state = 0;
        for (int pos = 0; pos < s.size(); pos++) {
            state = table[state][charval(s[pos])];
            react(state, s[pos], sub, sector);
        }
        state = table[state][3];
        react(state, '\0', sub, sector);
        data.push_back(sector);
    } else if (s_g == "xml") {
        static int flagv = 0, sround = 0;
        string sub;
        if (flagv == 0 && s.size() > 14 &&
            s.substr(0, 14) != "<?xml version=") {
            cout << "invalid input V" << endl;
            return;
        } else if (flagv == 0) {
            flagv = 1;
            return;
        }
        if (flag == 0 && flagv == 1) {
            nm_rt = s.substr(1, s.size() - 2);
            flag = 1;
            return;
        }
        if (flag == 1) {
            if (nm_rt == s.substr(2, s.size() - 3)) {
                flag = 0;
                return;
            } else {
                nm_obj = s.substr(1, s.size() - 2);
                sectorx.clear();
                flag = 2;
                return;
            }
        }
        if (flag == 2) {
            if (nm_obj == s.substr(2, s.size() - 3)) {
                flag = 1;
                data.push_back(sectorx);
                return;
            } else {
                if (nm_obj == "HEADER") {
                    int pos1 = s.find(">", 0);
                    sub = s.substr(2, pos1 - 2);
                    int pos2 = s.find("/", pos1);
                    if (sub != s.substr(pos2 + 1, sub.size())) {
                        cout << "invalid input T" << endl;
                        return;
                    } else {
                        labelx.push_back(sub);
                        sround++;
                        sectorx.push_back(s.substr(pos1 + 1, pos2 - pos1 - 2));
                    }
                } else {
                    sectorx.resize(sround);
                    int pos1 = s.find(">", 0);
                    sub = s.substr(2, pos1 - 2);
                    int pos2 = s.find("/", pos1);
                    if (sub != s.substr(pos2 + 1, sub.size())) {
                        cout << "invalid input T" << endl;
                        return;
                    }
                    for (int q = 0; q < labelx.size(); q++) {
                        if (sub != labelx[q])
                            continue;
                        else {
                            sectorx[q] = s.substr(pos1 + 1, pos2 - pos1 - 2);
                            break;
                        }
                        cout << "invalid input G" << endl;
                        return;
                    }
                }
            }
        }
    } else if (s_g == "json") {
        data.resize(nm - 2);
        if (s == "{" || s == "}")
            return;
        int pos = 0, pos2 = 0, sss;
        string pp;
        pos = s.find("_", 0);
        pos2 = s.find(":", 0);
        if (pos == -1 || pos2 == -1) {
            sss = 0;
        } else {
            pp = s.substr(pos + 1, pos2 - pos - 2);
            sss = stoi(pp);
        }

        pos = 0;
        pos2 = 0;
        pos = s.find("[", pos);
        s = s.substr(pos + 1, s.size() - pos - 1);
        pos = s.find("]", 0);
        s = s.substr(0, pos);
        while ((pos2 = s.find("\\", pos2)) != -1) {
            s.replace(pos2, 1, "\"");
        }

        string sub;
        vector<string> sector;
        int state = 0;
        for (int pos3 = 0; pos3 < s.size(); pos3++) {
            state = table[state][charval(s[pos3])];
            react(state, s[pos3], sub, sector);
        }
        state = table[state][3];
        react(state, '\0', sub, sector);
        data[sss] = sector;
    }
}
