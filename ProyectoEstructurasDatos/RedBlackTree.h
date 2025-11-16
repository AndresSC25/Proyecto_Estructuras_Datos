#pragma once
#include <map>
#include <vector>
#include "Usuario.h"
using namespace std;

/*
  Wrapper simple sobre std::map<int, Usuario> (std::map suele ser RB-tree).
*/
class RedBlackTree {
private:
    map<int, Usuario> arbol;

public:
    void insertar(const Usuario& u) {
        arbol[u.ID] = u;
    }

    Usuario* buscar(int id) {
        auto it = arbol.find(id);
        if (it != arbol.end()) return &it->second;
        return nullptr;
    }

    vector<Usuario> listarTodos() const {
        vector<Usuario> out;
        for (auto& kv : arbol) out.push_back(kv.second);
        return out;
    }

    void clear() { arbol.clear(); }

    bool erase(int id) {
        return arbol.erase(id) > 0;
    }
};
