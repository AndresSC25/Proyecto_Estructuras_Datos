#pragma once
#include <map>
#include <vector>
#include <string>
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
};
