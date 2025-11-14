#pragma once
#include <map>
#include "Usuario.h"
using namespace std;

/*
  RedBlackTree: wrapper sobre std::map<int, Usuario>
  std::map suele estar implementado como árbol rojo-negro => garantiza log(n) en búsquedas/inserciones.
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
};
