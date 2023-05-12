#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

vector<vector<string> > for_csv;
vector<vector<string> > for_header;
vector<string> one_line;
vector<string> column;
string word, tablename, outfile_name;

void getout() {
    ifstream config;
    config.open("config.txt");
    getline(config, outfile_name);
}

void print_header() {
    for (int k = 0; k < for_header.size(); k++) {
        cout << for_header[k][0] << " " << for_header[k][1] << " "
             << for_header[k][2] << " " << endl;
    }
}

void print_csv() {
    cout << "+";
    for (int r = 0; r < for_csv.size(); r++) {
        cout << setw(stoi(for_header[r][2]) + 1) << setfill('-') << "+";
    }
    cout << endl << "|";

    for (int k = 0; k < for_csv.size(); k++) {
        int half = (stoi(for_header[k][2]) + for_csv[k][0].size()) / 2;
        cout << setw(half) << setfill(' ');
        cout << for_csv[k][0];
        cout << setw(stoi(for_header[k][2]) - half + 1) << setfill(' ');
        cout << "|";
    }
    cout << endl;

    for (int k = 2; k < for_csv[0].size(); k++) {
        cout << "+";

        for (int r = 0; r < for_csv.size(); r++) {
            cout << setw(stoi(for_header[r][2]) + 1) << setfill('-') << "+";
        }
        cout << endl << "|";
        for (int r = 0; r < for_csv.size(); r++) {
            cout << setw(stoi(for_header[r][2])) << setfill(' ');
            cout << for_csv[r][k] << "|";
        }
        cout << endl;
    }
    cout << "+";
    for (int r = 0; r < for_csv.size(); r++) {
        cout << setw(stoi(for_header[r][2]) + 1) << setfill('-') << "+";
    }
    cout << endl;
}

void read_txt(string name) {
    for (int k = 0; k < for_header.size(); k++) {
        for_csv.push_back(for_header[k]);
    }
    for (int k = 0; k < for_csv.size(); k++) {
        for_csv[k].erase(for_csv[k].begin() + 1);
    }
    ifstream txt;
    txt.open(name);
    string line;
    int s = 0;

    while (getline(txt, line)) {
        vector<string> object;
        if (s % 2 == 0 || s == 1) {
            s++;
            continue;
        } else {
            string ss;
            for (int k = 0; k < line.size(); k++) {
                if (line[k] == ' ' || line[k] == '|') {
                    if (ss.empty())
                        continue;
                    else {
                        object.push_back(ss);
                        ss.clear();
                    }
                } else
                    ss += line[k];
            }
            s++;
        }
        for (int k = 0; k < object.size(); k++)
            for_csv[k].push_back(object[k]);
    }
}

void here(int k) {
    cout << "here" << k << endl;
}

void write_header(string name, int r) {
    ofstream csv;
    csv.open(name);
    for (int i = 0; i < (3 - r); i++) {
        for (int k = 0; k < for_header.size(); k++) {
            if (k != for_header.size() - 1)
                csv << for_header[k][i] << ",";
            else
                csv << for_header[k][i] << endl;
        }
    }
    csv.close();
}

void write_csv(string name) {
    ofstream csv;
    csv.open(name);
    for (int i = 0; i < for_csv[0].size(); i++) {
        for (int k = 0; k < for_csv.size(); k++) {
            if (k != for_csv.size() - 1)
                csv << for_csv[k][i] << ",";
            else
                csv << for_csv[k][i] << endl;
        }
    }
}

vector<vector<string> > label;
void turn() {
    vector<string> sml_label;
    string str, str2, strtmp;
    for (int k = 0; k < for_csv.size(); k++) {
        str = "<";
        str2 = "</";
        strtmp = for_csv[k][0];
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

void write_xml(string name) {
    ofstream xml;
    xml.open(name);
    xml << "<?xml version=\"1.0\"?>" << endl;
    xml << "<ROOT>" << endl;
    xml << "<HEADER>" << endl;
    for (int k = 0; k < label.size(); k++) {
        xml << "\t" << label[k][0] << for_csv[k][0] << label[k][1] << endl;
    }
    xml << "</HEADER>" << endl;
    for (int k = 0; k < for_csv[0].size(); k++) {
        xml << endl << "<INFO>" << endl;
        for (int r = 0; r < label.size(); r++) {
            xml << "\t" << label[r][0] << for_csv[r][k] << label[r][1] << endl;
        }
        xml << "</INFO>" << endl;
    }
    xml << "</ROOT>";
}

void write_json(string name) {
    ofstream json;
    json.open(name);
    string ss, rownum;
    json << "{" << endl;
    json << "\"header\": [";
    for (int k = 0; k < for_csv.size(); k++) {
        ss = "\"" + for_csv[k][0] + "\"";
        if (k < for_csv.size() - 1) {
            ss += ",";
        }
        cout << ss << endl;
        json << ss;
    }
    json << "]," << endl;
    for (int k = 1; k < for_csv[0].size(); k++) {
        rownum = "\"row_" + to_string(k) + "\": [";
        json << rownum;
        for (int g = 0; g < for_csv.size(); g++) {
            int pos = 0;
            while ((pos = for_csv[g][k].find("\"", pos + 2)) != string::npos) {
                for_csv[g][k].insert(pos, "\\");
            }
            ss = "\"" + for_csv[g][k] + "\"";
            if (g < for_csv.size() - 1) {
                ss += ",";
            }
            cout << ss << endl;
            json << ss;
        }
        if (k < for_csv[0].size() - 1)
            json << "]," << endl;
        else
            json << "]" << endl;
    }
    json << "}" << endl;
}

void write_out() {
    if (outfile_name.substr(outfile_name.size() - 3, 3) == "csv")
        write_csv(outfile_name);
    else if (outfile_name.substr(outfile_name.size() - 3, 3) == "xml") {
        turn();
        write_xml(outfile_name);
    } else if (outfile_name.substr(outfile_name.size() - 4, 4) == "json")
        write_json(outfile_name);
}

void get_header(string name) {
    ifstream csv;
    string line;
    csv.open(name);
    while (getline(csv, line)) {
        int k = 0, flag = 0;
        size_t pos;
        while ((pos = line.find(",", flag)) != string::npos) {
            if (for_header.size() == k) {
                vector<string> v;
                for_header.push_back(v);
            }
            for_header[k].push_back(line.substr(flag, pos - flag));
            flag = pos + 1;
            k++;
        }
        if (flag == line.size())
            for_header[k].push_back("");
        else {
            if (for_header.size() == k) {
                vector<string> v;
                for_header.push_back(v);
            }
            for_header[k].push_back(line.substr(flag, line.size() - flag));
        }
    }
}

void get_csv(string name) {
    ifstream csv;
    string line;
    csv.open(name);
    while (getline(csv, line)) {
        int k = 0, flag = 0;
        size_t pos;
        while ((pos = line.find(",", flag)) != string::npos) {
            if (for_csv.size() == k) {
                vector<string> v;
                for_csv.push_back(v);
            }
            for_csv[k].push_back(line.substr(flag, pos - flag));
            flag = pos + 1;
            k++;
        }
        if (flag == line.size())
            for_csv[k].push_back("");
        else {
            if (for_csv.size() == k) {
                vector<string> v;
                for_csv.push_back(v);
            }
            for_csv[k].push_back(line.substr(flag, line.size() - flag));
        }
    }
}

void create_table() {
    tablename = one_line[2];
    int prime = -1;
    for (int k = 0; k < one_line.size(); k++) {
        if (one_line[k] == "PRIMARY")
            prime = k;
    }
    if (prime < 0) {
        cout << "no primary key" << endl;
        return;
    }
    if (one_line[prime + 1] == "KEY,")
        one_line[prime - 1] += ",";
    if (one_line[prime + 1] == "KEY);")
        one_line[prime - 1] += ");";
    vector<string>::iterator iter = one_line.begin() + prime;
    one_line.erase(iter);
    one_line.erase(iter);
    prime = (prime - 3) / 2 - 1;

    for (int k = 3; k < one_line.size(); k++) {
        if (k % 2 != 0) {
            if (k == 3) {
                column.push_back(one_line[k].substr(1));
            } else
                column.push_back(one_line[k]);
        } else {
            size_t pos1 = one_line[k].find("(");
            size_t pos2 = one_line[k].find(")");
            if (pos1 != string::npos && pos2 != string::npos) {
                string type = one_line[k].substr(0, pos1);
                string length = one_line[k].substr(pos1 + 1, pos2 - pos1 - 1);
                column.push_back(type);
                column.push_back(length);
                for_header.push_back(column);
                column.clear();
            } else {
                column.push_back(one_line[k].substr(0, one_line[k].size() - 2));
                column.push_back("");
                for_header.push_back(column);
                column.clear();
            }
        }
    }
    for_header[prime][1] = "primary";
    string header_name = tablename + "_header.csv";
    write_header(header_name, 0);
    string filename = tablename + ".csv";
    write_header(filename, 1);
}

void insert() {
    vector<string> object;
    int n = one_line.size();
    if (one_line[3] == "VALUES") {
        for (int k = 4; k < n; k++) {
            string s;
            if (k == 4)
                s = one_line[k].substr(1, one_line[k].size() - 2);
            else if (k == n - 1)
                s = one_line[k].substr(0, one_line[k].size() - 2);
            else
                s = one_line[k].substr(0, one_line[k].size() - 1);

            if (for_header[k - 4][1] == "primary") {
                for (int r = 2; r < for_csv[k - 4].size(); r++) {
                    if (s == for_csv[k - 4][r]) {
                        cout << "primary key repetitive error" << endl;
                        return;
                    }
                }
            }

            if (for_header[k - 4][2] != "" &&
                s.size() > stoi(for_header[k - 4][2])) {
                cout << "oversize error" << endl;
                return;
            } else
                for_csv[k - 4].push_back(s);
        }
    } else {
        int reallen = (n - 4) / 2;
        vector<string> insert_type;
        for (int k = 3; k < reallen + 3; k++) {
            string s;
            if (k == 3)
                s = one_line[k].substr(1, one_line[k].size() - 2);
            else if (k == n - 1)
                s = one_line[k].substr(0, one_line[k].size() - 2);
            else
                s = one_line[k].substr(0, one_line[k].size() - 1);
            insert_type.push_back(s);
        }
        for (int k = reallen + 4; k < n; k++) {
            string s;
            if (k == reallen + 4)
                s = one_line[k].substr(1, one_line[k].size() - 2);
            else if (k == n - 1)
                s = one_line[k].substr(0, one_line[k].size() - 2);
            else
                s = one_line[k].substr(0, one_line[k].size() - 1);
            object.push_back(s);
        }
        vector<int> pos(reallen);
        for (int k = 0; k < insert_type.size(); k++) {
            for (int r = 0; r < for_header.size(); r++) {
                if (insert_type[k] == for_header[r][0]) {
                    pos[k] = r;
                    break;
                }
                if (r == for_header.size() - 1) {
                    cout << "typename not found" << endl;
                    return;
                }
            }
        }
        vector<string> adjust(for_csv.size());
        for (int k = 0; k < insert_type.size(); k++) {
            if (for_header[pos[k]][2] != "" &&
                object[k].size() > stoi(for_header[pos[k]][2])) {
                cout << "oversize error" << endl;
                return;
            } else if (for_header[pos[k]][1] == "primary") {
                for (int r = 2; r < for_csv[pos[k]].size(); r++) {
                    if (object[k] == for_csv[pos[k]][r]) {
                        cout << "primary key repetitive error" << endl;
                        return;
                    }
                    adjust[pos[k]] = object[k];
                }
            } else
                adjust[pos[k]] = object[k];
        }
        for (int k = 0; k < adjust.size(); k++) {
            for_csv[k].push_back(adjust[k]);
        }
    }
}

void update() {
    int n = one_line.size(), num_con = 0, num_set, a, b;
    one_line[n - 1] = one_line[n - 1].substr(0, one_line[n - 1].size() - 1);
    vector<pair<string, string> > change;
    for (int k = 0; k < n; k++) {
        if (one_line[k] == "AND" || one_line[k] == "OR")
            num_con++;
        if (one_line[k] == "SET")
            a = k;
        if (one_line[k] == "WHERE")
            b = k;
    }
    num_set = (b - a - 1) / 3;
    num_con++;
    for (int k = 0; k < num_set; k++) {
        if (k == num_set - 1)
            change.push_back(
                make_pair(one_line[3 + 3 * k], one_line[5 + 3 * k]));
        else
            change.push_back(make_pair(
                one_line[3 + 3 * k],
                one_line[5 + 3 * k].substr(0, one_line[5 + 3 * k].size() - 1)));
    }

    vector<vector<string> > condition(num_con, vector<string>(3));
    for (int k = 0; k < num_con; k++) {
        condition[k][0] = one_line[b + k * 4];
        condition[k][1] = one_line[b + 1 + k * 4];
        condition[k][2] = one_line[b + 3 + k * 4];
    }

    vector<int> changeid(change.size());
    for (int k = 0; k < change.size(); k++) {
        for (int r = 0; r < for_header.size(); r++) {
            if (change[k].first == for_header[r][0]) {
                changeid[k] = r;
                break;
            }
            if (r == for_header.size() - 1) {
                cout << "update typename not found" << endl;
                return;
            }
        }
    }

    for (int k = 0; k < change.size(); k++) {
        if (for_header[changeid[k]][2] != "" &&
            change[k].second.size() > stoi(for_header[changeid[k]][2])) {
            cout << "oversize error" << endl;
            return;
        }
        if (for_header[changeid[k]][1] == "primary") {
            for (int r = 2; r < for_csv[changeid[k]].size(); r++) {
                if (change[k].second == for_csv[changeid[k]][r]) {
                    cout << "primary key repetitive error" << endl;
                    return;
                }
            }
        }
    }

    vector<int> con_id(condition.size());
    for (int k = 0; k < condition.size(); k++) {
        for (int r = 0; r < for_header.size(); r++) {
            if (condition[k][1] == for_header[r][0]) {
                con_id[k] = r;
                break;
            }
            if (r == for_header.size() - 1) {
                cout << "condition typename not found" << endl;
                return;
            }
        }
    }

    vector<set<int> > update_set(condition.size());
    for (int k = 0; k < condition.size(); k++) {
        for (int r = 0; r < for_csv[con_id[k]].size(); r++) {
            if (condition[k][2] == for_csv[con_id[k]][r])
                update_set[k].insert(r);
        }
    }
    set<int> tmp;
    for (int k = 1; k < update_set.size(); k++) {
        if (condition[k][0] == "AND") {
            set_intersection(update_set[0].begin(), update_set[0].end(),
                             update_set[k].begin(), update_set[k].end(),
                             inserter(tmp, tmp.begin()));
            update_set[0] = tmp;
        } else if (condition[k][0] == "OR") {
            set_union(update_set[0].begin(), update_set[0].end(),
                      update_set[k].begin(), update_set[k].end(),
                      inserter(tmp, tmp.begin()));
            update_set[0] = tmp;
        }
    }

    vector<int> which;
    for (const auto& s : update_set[0]) {
        which.push_back(s);
    }

    for (int k = 0; k < which.size(); k++) {
        for (int r = 0; r < change.size(); r++)
            for_csv[changeid[r]][which[k]] = change[r].second;
    }
}

void del() {
    int n = one_line.size(), num_con;
    num_con = (n - 3) / 4;
    one_line[n - 1] = one_line[n - 1].substr(0, one_line[n - 1].size() - 1);
    vector<vector<string> > condition(num_con, vector<string>(3));
    for (int k = 0; k < num_con; k++) {
        condition[k][0] = one_line[3 + k * 4];
        condition[k][1] = one_line[4 + k * 4];
        condition[k][2] = one_line[6 + k * 4];
    }

    vector<int> con_id(condition.size());
    for (int k = 0; k < condition.size(); k++) {
        for (int r = 0; r < for_header.size(); r++) {
            if (condition[k][1] == for_header[r][0]) {
                con_id[k] = r;
                break;
            }
            if (r == for_header.size() - 1) {
                cout << "condition typename not found" << endl;
                return;
            }
        }
    }

    vector<set<int> > update_set(condition.size());
    for (int k = 0; k < condition.size(); k++) {
        for (int r = 0; r < for_csv[con_id[k]].size(); r++) {
            if (condition[k][2] == for_csv[con_id[k]][r])
                update_set[k].insert(r);
        }
    }
    set<int> tmp;
    for (int k = 1; k < update_set.size(); k++) {
        if (condition[k][0] == "AND") {
            set_intersection(update_set[0].begin(), update_set[0].end(),
                             update_set[k].begin(), update_set[k].end(),
                             inserter(tmp, tmp.begin()));
            update_set[0] = tmp;
        } else if (condition[k][0] == "OR") {
            set_union(update_set[0].begin(), update_set[0].end(),
                      update_set[k].begin(), update_set[k].end(),
                      inserter(tmp, tmp.begin()));
            update_set[0] = tmp;
        }
    }
    vector<int> which;
    for (const auto& s : update_set[0]) {
        which.push_back(s);
    }
    for (int k = which.size() - 1; k >= 0; k--) {
        for (int r = 0; r < for_csv.size(); r++)
            for_csv[r].erase(for_csv[r].begin() + which[k]);
    }
}

void select_col() {
    int n = one_line.size(), num_con = 0, col_num, col_id, a = -1;
    for (int k = 0; k < n; k++) {
        if (one_line[k] == "WHERE") {
            a = k;
            break;
        }
    }
    one_line[n - 1] = one_line[n - 1].substr(0, one_line[n - 1].size() - 1);
    if (a > 0)
        num_con = (n - a) / 4;

    if (a > 0)
        tablename = one_line[a - 1];
    else
        tablename = one_line[n - 1];
    get_header(tablename + "_header.csv");
    get_csv(tablename + ".csv");

    if (a < 0)
        col_num = n - 3;
    else if (one_line[a + 2] == "BETWEEN")
        col_num = n - 9;
    else
        col_num = n - 4 * (num_con)-3;

    vector<vector<string> > condition(num_con, vector<string>(3));
    vector<int> con_id(num_con), which;
    vector<set<int> > update_set(num_con);
    set<int> tmp;

    if (a > 0) {
        if (one_line[a + 2] == "BETWEEN") {
            int lowbound = min(stoi(one_line[a + 3]), stoi(one_line[a + 5]));
            int uppbound = max(stoi(one_line[a + 3]), stoi(one_line[a + 5]));

            for (int r = 0; r < for_header.size(); r++) {
                if (one_line[a + 1] == for_header[r][0]) {
                    con_id[0] = r;
                    break;
                }
                if (r == for_header.size() - 1) {
                    cout << "condition typename not found" << endl;
                    return;
                }
            }

            for (int r = 2; r < for_csv[con_id[0]].size(); r++) {
                if (stoi(for_csv[con_id[0]][r]) <= uppbound &&
                    stoi(for_csv[con_id[0]][r]) >= lowbound)
                    update_set[0].insert(r);
            }
        } else {
            for (int k = 0; k < num_con; k++) {
                condition[k][0] = one_line[a + k * 4];
                condition[k][1] = one_line[a + 1 + k * 4];
                condition[k][2] = one_line[a + 3 + k * 4];
            }

            for (int k = 0; k < condition.size(); k++) {
                for (int r = 0; r < for_header.size(); r++) {
                    if (condition[k][1] == for_header[r][0]) {
                        con_id[k] = r;
                        break;
                    }
                    if (r == for_header.size() - 1) {
                        cout << "condition typename not found" << endl;
                        return;
                    }
                }
            }

            for (int k = 0; k < condition.size(); k++) {
                for (int r = 0; r < for_csv[con_id[k]].size(); r++) {
                    if (condition[k][2] == for_csv[con_id[k]][r])
                        update_set[k].insert(r);
                }
            }
            for (int k = 1; k < update_set.size(); k++) {
                if (condition[k][0] == "AND") {
                    set_intersection(update_set[0].begin(), update_set[0].end(),
                                     update_set[k].begin(), update_set[k].end(),
                                     inserter(tmp, tmp.begin()));
                    update_set[0] = tmp;
                } else if (condition[k][0] == "OR") {
                    set_union(update_set[0].begin(), update_set[0].end(),
                              update_set[k].begin(), update_set[k].end(),
                              inserter(tmp, tmp.begin()));
                    update_set[0] = tmp;
                }
            }
        }

        which.push_back(0);
        for (const auto& s : update_set[0]) {
            which.push_back(s);
        }
    }

    if (a < 0) {
        for (int k = 0; k < for_csv[0].size(); k++) {
            if (k != 1)
                which.push_back(k);
        }
    }

    if (col_num == 1) {
        if (one_line[1] == "*") {
            for (int k = 0; k < which.size(); k++) {
                for (int r = 0; r < for_csv.size(); r++) {
                    cout << for_csv[r][which[k]] << " ";
                }
                cout << endl;
            }
        } else {
            string col_name;
            col_name = one_line[1];
            for (int r = 0; r < for_header.size(); r++) {
                if (col_name == for_header[r][0]) {
                    col_id = r;
                    break;
                }
                if (r == for_header.size() - 1) {
                    cout << "condition typename not found" << endl;
                    return;
                }
            }
            for (int k = 0; k < which.size(); k++) {
                cout << for_csv[col_id][which[k]] << endl;
            }
        }
    } else {
        vector<string> col_name(col_num);
        for (int k = 0; k < col_num - 1; k++)
            col_name[k] = one_line[k + 1].substr(0, one_line[k + 1].size() - 1);
        col_name[col_num - 1] = one_line[col_num];

        vector<vector<string> > print(col_num, vector<string>(which.size()));

        vector<int> col_id(col_num);
        for (int k = 0; k < col_num; k++) {
            for (int r = 0; r < for_header.size(); r++) {
                if (col_name[k] == for_header[r][0]) {
                    col_id[k] = r;
                    break;
                }
                if (r == for_header.size() - 1) {
                    cout << "condition typename not found" << endl;
                    return;
                }
            }
        }
        for (int r = 0; r < col_num; r++) {
            for (int k = 0; k < which.size(); k++) {
                print[r][k] = for_csv[col_id[r]][which[k]];
            }
        }

        for (int k = 0; k < print[0].size(); k++) {
            for (int r = 0; r < print.size(); r++)
                cout << print[r][k] << " ";
            cout << endl;
        }
    }
}

void select_func() {
    int n = one_line.size();
    string func_name, col_name;
    tablename = one_line[n - 1].substr(0, one_line[n - 1].size() - 1);
    get_header(tablename + "_header.csv");
    get_csv(tablename + ".csv");
    int col_id;
    size_t pos = one_line[1].find("(");
    func_name = one_line[1].substr(0, pos);
    if (func_name == "REPLACE") {
        col_name = one_line[1].substr(pos + 1, one_line[1].size() - pos - 2);
        string s1, s2;
        s1 = one_line[2].substr(0, one_line[2].size() - 1);
        s2 = one_line[3].substr(0, one_line[3].size() - 1);
        for (int r = 0; r < for_header.size(); r++) {
            if (col_name == for_header[r][0]) {
                col_id = r;
                break;
            }
            if (r == for_header.size() - 1) {
                cout << "condition typename not found" << endl;
                return;
            }
        }
        for (int k = 2; k < for_csv[col_id].size(); k++) {
            boost::replace_all(for_csv[col_id][k], s1, s2);
        }
        for (int k = 0; k < for_csv[col_id].size(); k++) {
            if (k == 1)
                continue;
            else
                cout << for_csv[col_id][k] << endl;
        }
        write_out();
        return;
    }

    col_name = one_line[1].substr(pos + 1, one_line[1].size() - pos - 2);

    for (int r = 0; r < for_header.size(); r++) {
        if (col_name == for_header[r][0]) {
            col_id = r;
            break;
        }
        if (r == for_header.size() - 1) {
            cout << "condition typename not found" << endl;
            return;
        }
    }

    if (func_name == "MAX") {
        int max = INT_MIN;
        for (int k = 2; k < for_csv[col_id].size(); k++) {
            if (for_csv[col_id][k] == "")
                continue;
            if (stoi(for_csv[col_id][k]) > max)
                max = stoi(for_csv[col_id][k]);
        }
        cout << one_line[1] << " " << max << endl;
    } else if (func_name == "MIN") {
        int min = INT_MAX;
        for (int k = 2; k < for_csv[col_id].size(); k++) {
            if (for_csv[col_id][k] == "")
                continue;
            if (stoi(for_csv[col_id][k]) < min)
                min = stoi(for_csv[col_id][k]);
        }
        cout << one_line[1] << " " << min << endl;
    } else if (func_name == "COUNT") {
    } else if (func_name == "SUM") {
        int sum = 0;
        for (int k = 2; k < for_csv[col_id].size(); k++) {
            if (for_csv[col_id][k] == "")
                continue;
            sum += stoi(for_csv[col_id][k]);
        }
        cout << one_line[1] << " " << sum << endl;
    } else if (func_name == "AVG") {
        int sum = 0, no_count = 0;
        for (int k = 2; k < for_csv[col_id].size(); k++) {
            if (for_csv[col_id][k] == "") {
                no_count++;
                continue;
            }
            sum += stoi(for_csv[col_id][k]);
        }
        double avg = double(sum) / (for_csv[col_id].size() - 2 - no_count);
        cout << one_line[1] << " " << avg << endl;
    } else {
        cout << "no this function" << endl;
        return;
    }
}

int main() {
    ifstream sql;
    getout();
    sql.open("command.txt");
    int flag = 0;
    while (sql >> word)
        one_line.push_back(word);
    cin >> flag;

    if (one_line[0] == "CREATE")
        create_table();

    if (one_line[0] == "INSERT") {
        tablename = one_line[2];
        get_header(tablename + "_header.csv");
        if (flag > 0)
            get_csv(tablename + ".csv");
        else
            read_txt(tablename + ".txt");
        insert();
        write_out();
    }
    if (one_line[0] == "UPDATE") {
        tablename = one_line[1];
        get_header(tablename + "_header.csv");
        if (flag > 0)
            get_csv(tablename + ".csv");
        else
            read_txt(tablename + ".txt");
        update();
        write_out();
    }
    if (one_line[0] == "DELETE") {
        tablename = one_line[2];
        get_header(tablename + "_header.csv");
        if (flag > 0)
            get_csv(tablename + ".csv");
        else
            read_txt(tablename + ".txt");
        del();
        write_out();
    }
    if (one_line[0] == "DROP") {
        tablename = one_line[2].substr(0, one_line[2].size() - 1);
        remove((tablename + "_header.csv").c_str());
        remove((tablename + ".csv").c_str());
    }
    if (one_line[0] == "TRUNCATE") {
        tablename = one_line[2].substr(0, one_line[2].size() - 1);
        get_header(tablename + "_header.csv");
        write_header(tablename + ".csv", 1);
    }
    if (one_line[0] == "SELECT") {
        size_t pos = one_line[1].find("(");
        if (pos == string::npos)
            select_col();
        else
            select_func();
    }
    sql.close();
    print_csv();
}
