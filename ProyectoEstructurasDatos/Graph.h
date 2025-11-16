#pragma once
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Graph {
private:
    map<string, vector<string>> relaciones;

public:
    void agregarRelacion(const string& libroA, const string& libroB) {
        relaciones[libroA].push_back(libroB);
    }

    vector<string> recomendaciones(const string& libro) const {
        if (relaciones.count(libro)) return relaciones.at(libro);
        return {};
    }

    // eliminar una relacion A->B
    void eliminarRelacion(const string& libroA, const string& libroB) {
        if (!relaciones.count(libroA)) return;
        auto& v = relaciones[libroA];
        v.erase(remove(v.begin(), v.end(), libroB), v.end());
        if (v.empty()) relaciones.erase(libroA);
    }

    // eliminar todas las relaciones asociadas a un nodo (cuando borras un libro)
    void eliminarNodo(const string& libro) {
        // eliminar salientes
        relaciones.erase(libro);
        // eliminar entrantes
        for (auto it = relaciones.begin(); it != relaciones.end();) {
            auto& v = it->second;
            v.erase(remove(v.begin(), v.end(), libro), v.end());
            if (v.empty()) it = relaciones.erase(it);
            else ++it;
        }
    }

    void clear() { relaciones.clear(); }
};
