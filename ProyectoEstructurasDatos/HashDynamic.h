#pragma once
#include <unordered_map>
#include <vector>
using namespace std;

// Hash dinámico (multimap-like): mapa autor -> lista de ISBNs (o punteros)
template <typename K, typename V>
class HashDynamic {
public:
    unordered_map<K, vector<V>> tabla;

    void insertar(const K& clave, const V& valor) {
        tabla[clave].push_back(valor);
    }

    vector<V> buscar(const K& clave) {
        if (tabla.count(clave)) return tabla[clave];
        return {};
    }

    void clear() { tabla.clear(); }
};
