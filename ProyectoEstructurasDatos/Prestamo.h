#pragma once
#include <string>

using namespace std;

struct Prestamo {
    string idPrestamo;
    string ISBN_libro;
    int usuarioID;
    string fechaInicio;
    string fechaFin;
    bool procesado{ false }; // si ya fue procesado (salida de la cola)

    Prestamo() = default;
    Prestamo(const string& idp, const string& isbn, int uid, const string& fi, const string& ff = "")
        : idPrestamo(idp), ISBN_libro(isbn), usuarioID(uid), fechaInicio(fi), fechaFin(ff), procesado(false) {
    }

    string serialize() const {
        // id|ISBN|uid|fechaInicio|fechaFin|procesado
        stringstream ss;
        ss << idPrestamo << "|" << ISBN_libro << "|" << usuarioID << "|" << fechaInicio << "|" << fechaFin << "|" << (procesado ? "1" : "0");
        return ss.str();
    }

    static Prestamo deserialize(const string& line) {
        Prestamo p;
        vector<string> parts;
        string cur;
        for (char ch : line) {
            if (ch == '|') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        parts.push_back(cur);
        if (parts.size() >= 6) {
            p.idPrestamo = parts[0];
            p.ISBN_libro = parts[1];
            p.usuarioID = stoi(parts[2]);
            p.fechaInicio = parts[3];
            p.fechaFin = parts[4];
            p.procesado = (parts[5] == "1");
        }
        return p;
    }
};
