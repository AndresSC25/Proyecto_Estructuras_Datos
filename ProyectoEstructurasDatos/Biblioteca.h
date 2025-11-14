#pragma once
#include <string>
#include <vector>

#include "BPlusTree.h"
#include "RedBlackTree.h"
#include "HashDynamic.h"
#include "Graph.h"
#include "Queue.h"

#include "Libro.h"
#include "Usuario.h"
#include "Prestamo.h"

using namespace std;

class Biblioteca {
private:
    BPlusTree libros;
    RedBlackTree usuarios;
    HashDynamic<string, string> hashAutor; // autor -> lista de ISBNs
    Graph grafoRecomendaciones;
    Queue<Prestamo> colaPrestamos;

    // archivos
    string archivoLibros = "libros.txt";
    string archivoUsuarios = "usuarios.txt";

public:
    Biblioteca() = default;

    // Operaciones
    bool agregarLibro(const Libro& libro);
    bool registrarUsuario(const Usuario& usuario);
    vector<Libro*> buscarPorTituloPrefijo(const string& prefijo);
    vector<string> buscarISBNsPorAutor(const string& autor);
    void solicitarPrestamo(const string& isbn, int usuarioID);
    void procesarColaPrestamos(); // simula procesar la cola

    // Persistencia
    bool guardarLibros();
    bool guardarUsuarios();
    bool cargarLibros();
    bool cargarUsuarios();

    // utilitarios
    Libro* buscarPorISBN(const string& isbn);
    Usuario* buscarUsuario(int id);

    // recomendaciones
    void agregarRelacionRecomendacion(const string& isbnA, const string& isbnB);
    vector<string> recomendarPara(const string& isbn);
};
