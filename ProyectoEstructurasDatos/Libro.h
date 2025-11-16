#pragma once
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Libro {
    string ISBN;
    string titulo;
    string autor;
    string categoria;
    bool disponible{ true };

    Libro() = default;
    Libro(const string& isbn, const string& t, const string& a, const string& c)
        : ISBN(isbn), titulo(t), autor(a), categoria(c), disponible(true) {
    }

    string serialize() const {
        // formato: ISBN|titulo|autor|categoria|disponible
        stringstream ss;
        ss << ISBN << "|" << titulo << "|" << autor << "|" << categoria << "|" << (disponible ? "1" : "0");
        return ss.str();
    }

    static Libro deserialize(const string& line) {
        Libro l;
        vector<string> parts;
        string cur;
        for (char ch : line) {
            if (ch == '|') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        parts.push_back(cur);
        if (parts.size() >= 5) {
            l.ISBN = parts[0];
            l.titulo = parts[1];
            l.autor = parts[2];
            l.categoria = parts[3];
            l.disponible = (parts[4] == "1");
        }
        return l;
    }
};
