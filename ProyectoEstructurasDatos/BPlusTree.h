#pragma once
#include <map>
#include <vector>
#include <string>
#include "Libro.h"

using namespace std;

/*
  Simplified B+ style wrapper using std::map keyed by ISBN.
  Supports insert, buscar por ISBN, listar, buscar por prefijo de titulo, borrar.
*/
class BPlusTree {
private:
    map<string, Libro> indice; // key: ISBN

public:
    void insertar(const Libro& libro) {
        indice[libro.ISBN] = libro;
    }

    // Busca por ISBN
    Libro* buscar(const string& isbn) {
        auto it = indice.find(isbn);
        if (it != indice.end()) return &it->second;
        return nullptr;
    }

    // Buscar por rango de título (prefijo)
    vector<Libro*> buscarPorTituloPrefijo(const string& prefijo) {
        vector<Libro*> res;
        for (auto it = indice.begin(); it != indice.end(); ++it) {
            if (it->second.titulo.rfind(prefijo, 0) == 0) { // empieza con prefijo
                res.push_back(&it->second);
            }
        }
        return res;
    }

    vector<Libro*> listarTodos() {
        vector<Libro*> res;
        for (auto& p : indice) res.push_back(&p.second);
        return res;
    }

    // Persistencia: serializar todos
    vector<Libro> dumpAll() const {
        vector<Libro> list;
        for (auto& kv : indice) list.push_back(kv.second);
        return list;
    }

    void clear() { indice.clear(); }

    bool erase(const string& isbn) {
        return indice.erase(isbn) > 0;
    }
};
