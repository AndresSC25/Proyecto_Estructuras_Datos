#include "Biblioteca.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

bool Biblioteca::agregarLibro(const Libro& libro) {
    if (buscarPorISBN(libro.ISBN) != nullptr) {
        cout << "ISBN ya existente: " << libro.ISBN << "\n";
        return false;
    }
    libros.insertar(libro);
    hashAutor.insertar(libro.autor, libro.ISBN);
    cout << "Libro agregado: " << libro.titulo << " (ISBN: " << libro.ISBN << ")\n";
    return true;
}

bool Biblioteca::registrarUsuario(const Usuario& usuario) {
    if (buscarUsuario(usuario.ID) != nullptr) {
        cout << "Usuario con ID ya existe: " << usuario.ID << "\n";
        return false;
    }
    usuarios.insertar(usuario);
    cout << "Usuario registrado: " << usuario.nombre << " (ID: " << usuario.ID << ")\n";
    return true;
}

vector<Libro*> Biblioteca::buscarPorTituloPrefijo(const string& prefijo) {
    return libros.buscarPorTituloPrefijo(prefijo);
}

vector<string> Biblioteca::buscarISBNsPorAutor(const string& autor) {
    return hashAutor.buscar(autor);
}

void Biblioteca::solicitarPrestamo(const string& isbn, int usuarioID) {
    Libro* l = buscarPorISBN(isbn);
    Usuario* u = buscarUsuario(usuarioID);
    if (!l) { cout << "Libro no encontrado (ISBN " << isbn << ")\n"; return; }
    if (!u) { cout << "Usuario no encontrado (ID " << usuarioID << ")\n"; return; }
    if (!l->disponible) { cout << "Libro no disponible: " << l->titulo << "\n"; return; }

    // generar id de prestamo simple: ISBN-UID-timestamp
    auto t = std::time(nullptr);
    string idp = isbn + "-" + to_string(usuarioID) + "-" + to_string(t);

    // fecha inicio (YYYY-MM-DD) usando localtime_s
    std::tm tstruct;
    localtime_s(&tstruct, &t);

    stringstream ss;
    ss << put_time(&tstruct, "%Y-%m-%d");

    Prestamo p(idp, isbn, usuarioID, ss.str());
    colaPrestamos.push(p);
    l->disponible = false;
    cout << "Prestamo encolado: " << idp << " -> " << l->titulo << "\n";
}

void Biblioteca::procesarColaPrestamos() {
    cout << "Procesando cola de prestamos...\n";
    while (!colaPrestamos.empty()) {
        Prestamo p = colaPrestamos.front();
        cout << "Procesado prestamo: " << p.idPrestamo
            << " Libro: " << p.ISBN_libro
            << " UsuarioID: " << p.usuarioID << "\n";
        colaPrestamos.pop();
    }
    cout << "Cola procesada.\n";
}

bool Biblioteca::guardarLibros() {
    ofstream ofs(archivoLibros);
    if (!ofs) return false;
    auto all = libros.dumpAll();
    for (auto& l : all) ofs << l.serialize() << "\n";
    ofs.close();
    return true;
}

bool Biblioteca::guardarUsuarios() {
    ofstream ofs(archivoUsuarios);
    if (!ofs) return false;
    auto list = usuarios.listarTodos();
    for (auto& u : list) ofs << u.serialize() << "\n";
    ofs.close();
    return true;
}

bool Biblioteca::cargarLibros() {
    ifstream ifs(archivoLibros);
    if (!ifs) return false;
    string line;
    libros.clear();
    hashAutor.clear();
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Libro l = Libro::deserialize(line);
        libros.insertar(l);
        hashAutor.insertar(l.autor, l.ISBN);
    }
    ifs.close();
    return true;
}

bool Biblioteca::cargarUsuarios() {
    ifstream ifs(archivoUsuarios);
    if (!ifs) return false;
    string line;
    usuarios.clear();
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Usuario u = Usuario::deserialize(line);
        usuarios.insertar(u);
    }
    ifs.close();
    return true;
}

Libro* Biblioteca::buscarPorISBN(const string& isbn) {
    return libros.buscar(isbn);
}

Usuario* Biblioteca::buscarUsuario(int id) {
    return usuarios.buscar(id);
}

void Biblioteca::agregarRelacionRecomendacion(const string& isbnA, const string& isbnB) {
    grafoRecomendaciones.agregarRelacion(isbnA, isbnB);
}

vector<string> Biblioteca::recomendarPara(const string& isbn) {
    return grafoRecomendaciones.recomendaciones(isbn);
}
