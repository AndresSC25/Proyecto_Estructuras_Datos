#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// Hash dinámico (multimap-like): mapa autor -> lista de ISBNs
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

    void removeValue(const K& clave, const V& valor) {
        if (!tabla.count(clave)) return;
        auto& vec = tabla[clave];
        vec.erase(std::remove(vec.begin(), vec.end(), valor), vec.end());
        if (vec.empty()) tabla.erase(clave);
    }

    void clear() { tabla.clear(); }
};
