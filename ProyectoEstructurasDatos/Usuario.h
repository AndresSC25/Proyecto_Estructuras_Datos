#pragma once
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Usuario {
    int ID{ 0 };
    string nombre;
    string email;

    Usuario() = default;
    Usuario(int id, const string& n, const string& e) : ID(id), nombre(n), email(e) {}

    string serialize() const {
        // formato: ID|nombre|email
        stringstream ss;
        ss << ID << "|" << nombre << "|" << email;
        return ss.str();
    }

    static Usuario deserialize(const string& line) {
        Usuario u;
        vector<string> parts;
        string cur;
        for (char ch : line) {
            if (ch == '|') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        parts.push_back(cur);
        if (parts.size() >= 3) {
            u.ID = stoi(parts[0]);
            u.nombre = parts[1];
            u.email = parts[2];
        }
        return u;
    }
};
