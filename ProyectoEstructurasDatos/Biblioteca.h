#pragma once
#include <string>
#include <vector>
#include <unordered_map>

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

    // prestamos: map id -> Prestamo (para persistencia y consulta)
    unordered_map<string, Prestamo> prestamosMap;
    Queue<string> colaPrestamos; // cola de IDs de prestamos

    // archivos
    string archivoLibros = "libros.txt";
    string archivoUsuarios = "usuarios.txt";
    string archivoPrestamos = "prestamos.txt";

public:
    Biblioteca() = default;

    // Operaciones CRUD Libros
    bool agregarLibro(const Libro& libro);
    Libro* buscarPorISBN(const string& isbn);
    vector<Libro*> buscarPorTituloPrefijo(const string& prefijo);
    vector<Libro*> listarLibros();
    bool modificarLibro(const string& isbn, const Libro& nuevo); // update fields
    bool eliminarLibro(const string& isbn);

    // Operaciones CRUD Usuarios
    bool registrarUsuario(const Usuario& usuario);
    Usuario* buscarUsuario(int id);
    vector<Usuario> listarUsuarios();
    bool modificarUsuario(int id, const Usuario& nuevo);
    bool eliminarUsuario(int id);

    // Hash autor
    vector<string> buscarISBNsPorAutor(const string& autor);

    // Recomendaciones (grafo)
    void agregarRelacionRecomendacion(const string& isbnA, const string& isbnB);
    vector<string> recomendarPara(const string& isbn);
    void eliminarRelacionRecomendacion(const string& isbnA, const string& isbnB);

    // Prestamos
    void solicitarPrestamo(const string& isbn, int usuarioID);
    void procesarColaPrestamos(); // procesa hasta vaciar
    bool marcarComoDevuelto(const string& idPrestamo); // marcar devuelto
    bool eliminarPrestamo(const string& idPrestamo);

    // Persistencia
    bool guardarLibros();
    bool guardarUsuarios();
    bool guardarPrestamos();
    bool cargarLibros();
    bool cargarUsuarios();
    bool cargarPrestamos();

    // utilitarios
    void clearAll();
};
