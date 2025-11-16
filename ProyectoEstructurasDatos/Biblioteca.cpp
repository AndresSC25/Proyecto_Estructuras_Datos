#include "Biblioteca.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

// ====== Libros ======
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

Libro* Biblioteca::buscarPorISBN(const string& isbn) {
    return libros.buscar(isbn);
}

vector<Libro*> Biblioteca::buscarPorTituloPrefijo(const string& prefijo) {
    return libros.buscarPorTituloPrefijo(prefijo);
}

vector<Libro*> Biblioteca::listarLibros() {
    return libros.listarTodos();
}

bool Biblioteca::modificarLibro(const string& isbn, const Libro& nuevo) {
    Libro* orig = libros.buscar(isbn);
    if (!orig) {
        cout << "No se encontro libro con ISBN " << isbn << "\n";
        return false;
    }
    // si cambia ISBN no permitimos (clave primaria). Debe eliminar y reinsertar manualmente.
    if (nuevo.ISBN != isbn) {
        cout << "Cambio de ISBN no permitido via modificar. Elimina e inserta con nuevo ISBN.\n";
        return false;
    }

    // si cambia autor -> actualizar hashAutor
    if (orig->autor != nuevo.autor) {
        hashAutor.removeValue(orig->autor, isbn);
        hashAutor.insertar(nuevo.autor, isbn);
    }

    orig->titulo = nuevo.titulo;
    orig->autor = nuevo.autor;
    orig->categoria = nuevo.categoria;
    orig->disponible = nuevo.disponible;
    cout << "Libro modificado: " << isbn << "\n";
    return true;
}

bool Biblioteca::eliminarLibro(const string& isbn) {
    Libro* l = libros.buscar(isbn);
    if (!l) {
        cout << "No existe libro (ISBN " << isbn << ")\n";
        return false;
    }

    // eliminar del hashAutor
    hashAutor.removeValue(l->autor, isbn);

    // eliminar del grafo
    grafoRecomendaciones.eliminarNodo(isbn);

    // eliminar
    bool ok = libros.erase(isbn);
    if (ok) cout << "Libro eliminado: " << isbn << "\n";
    else cout << "Error eliminando libro: " << isbn << "\n";
    return ok;
}

// ====== Usuarios ======
bool Biblioteca::registrarUsuario(const Usuario& usuario) {
    if (buscarUsuario(usuario.ID) != nullptr) {
        cout << "Usuario con ID ya existe: " << usuario.ID << "\n";
        return false;
    }
    usuarios.insertar(usuario);
    cout << "Usuario registrado: " << usuario.nombre << " (ID: " << usuario.ID << ")\n";
    return true;
}

Usuario* Biblioteca::buscarUsuario(int id) {
    return usuarios.buscar(id);
}

vector<Usuario> Biblioteca::listarUsuarios() {
    return usuarios.listarTodos();
}

bool Biblioteca::modificarUsuario(int id, const Usuario& nuevo) {
    Usuario* u = usuarios.buscar(id);
    if (!u) {
        cout << "Usuario no encontrado: " << id << "\n";
        return false;
    }
    // ID no puede cambiar
    if (nuevo.ID != id) {
        cout << "No se permite cambiar el ID del usuario.\n";
        return false;
    }
    u->nombre = nuevo.nombre;
    u->email = nuevo.email;
    cout << "Usuario modificado: " << id << "\n";
    return true;
}

bool Biblioteca::eliminarUsuario(int id) {
    // opcional: comprobar prestamos pendientes (si existe alguno no procesado)
    for (auto& kv : prestamosMap) {
        const Prestamo& p = kv.second;
        if (p.usuarioID == id && !p.procesado) {
            cout << "Usuario tiene prestamos pendientes -> no se puede eliminar\n";
            return false;
        }
    }
    bool ok = usuarios.erase(id);
    if (ok) cout << "Usuario eliminado: " << id << "\n";
    else cout << "No se encontro usuario: " << id << "\n";
    return ok;
}

// ====== Hash Autor ======
vector<string> Biblioteca::buscarISBNsPorAutor(const string& autor) {
    return hashAutor.buscar(autor);
}

// ====== Grafo recomendaciones ======
void Biblioteca::agregarRelacionRecomendacion(const string& isbnA, const string& isbnB) {
    grafoRecomendaciones.agregarRelacion(isbnA, isbnB);
}

vector<string> Biblioteca::recomendarPara(const string& isbn) {
    return grafoRecomendaciones.recomendaciones(isbn);
}

void Biblioteca::eliminarRelacionRecomendacion(const string& isbnA, const string& isbnB) {
    grafoRecomendaciones.eliminarRelacion(isbnA, isbnB);
}

// ====== Prestamos ======
static string nowDate() {
    std::time_t t = std::time(nullptr);
    std::tm tmstruct;
#if defined(_MSC_VER)
    localtime_s(&tmstruct, &t);
#else
    tmstruct = *std::localtime(&t);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tmstruct, "%Y-%m-%d");
    return ss.str();
}

void Biblioteca::solicitarPrestamo(const string& isbn, int usuarioID) {
    Libro* l = buscarPorISBN(isbn);
    Usuario* u = buscarUsuario(usuarioID);
    if (!l) { cout << "Libro no encontrado (ISBN " << isbn << ")\n"; return; }
    if (!u) { cout << "Usuario no encontrado (ID " << usuarioID << ")\n"; return; }
    if (!l->disponible) { cout << "Libro no disponible: " << l->titulo << "\n"; return; }

    auto t = std::time(nullptr);
    string idp = isbn + "-" + to_string(usuarioID) + "-" + to_string(t);
    string fecha = nowDate();
    Prestamo p(idp, isbn, usuarioID, fecha);
    prestamosMap[idp] = p;
    colaPrestamos.push(idp);
    l->disponible = false;
    cout << "Prestamo creado y encolado: " << idp << " -> " << l->titulo << "\n";
    guardarPrestamos(); // persistir cambios
    guardarLibros();
}

void Biblioteca::procesarColaPrestamos() {
    cout << "Procesando cola de prestamos...\n";
    while (!colaPrestamos.empty()) {
        string id = colaPrestamos.front();
        colaPrestamos.pop();
        if (!prestamosMap.count(id)) {
            cout << "Prestamo no encontrado en mapa: " << id << "\n";
            continue;
        }
        Prestamo& p = prestamosMap[id];
        // marcar procesado y asignar fechaFin vacia (se procesa como 'registrado' salida)
        p.procesado = true;
        cout << "Procesado prestamo: " << p.idPrestamo
            << " Libro: " << p.ISBN_libro
            << " UsuarioID: " << p.usuarioID << "\n";
    }
    cout << "Cola procesada.\n";
    guardarPrestamos();
}

bool Biblioteca::marcarComoDevuelto(const string& idPrestamo) {
    if (!prestamosMap.count(idPrestamo)) {
        cout << "Prestamo no encontrado: " << idPrestamo << "\n";
        return false;
    }
    Prestamo& p = prestamosMap[idPrestamo];
    if (!p.procesado) {
        cout << "Prestamo aun no procesado (no fue entregado formalmente)\n";
        return false;
    }
    if (!p.fechaFin.empty()) {
        cout << "Prestamo ya fue devuelto previamente: " << idPrestamo << "\n";
        return false;
    }
    p.fechaFin = nowDate();
    // volver a habilitar libro
    Libro* l = buscarPorISBN(p.ISBN_libro);
    if (l) l->disponible = true;
    cout << "Prestamo marcado como devuelto: " << idPrestamo << "\n";
    guardarPrestamos();
    guardarLibros();
    return true;
}

bool Biblioteca::eliminarPrestamo(const string& idPrestamo) {
    // si existe en cola y no procesado -> eliminar de cola: cola simple no permite eliminar por id (simplificamos)
    // solo permitimos borrar si ya procesado o si existe en mapa (borrar registro)
    if (!prestamosMap.count(idPrestamo)) {
        cout << "No existe prestamo: " << idPrestamo << "\n";
        return false;
    }
    Prestamo p = prestamosMap[idPrestamo];
    // si fue procesado y no hay devolución -> marcar libro disponible (consistencia)
    if (p.procesado && p.fechaFin.empty()) {
        Libro* l = buscarPorISBN(p.ISBN_libro);
        if (l) l->disponible = true;
    }
    prestamosMap.erase(idPrestamo);
    cout << "Prestamo eliminado: " << idPrestamo << "\n";
    guardarPrestamos();
    guardarLibros();
    return true;
}

// ====== Persistencia ======
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

bool Biblioteca::guardarPrestamos() {
    ofstream ofs(archivoPrestamos);
    if (!ofs) return false;
    for (auto& kv : prestamosMap) {
        ofs << kv.second.serialize() << "\n";
    }
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

bool Biblioteca::cargarPrestamos() {
    ifstream ifs(archivoPrestamos);
    if (!ifs) return false;
    string line;
    prestamosMap.clear();
    // cola no se restaura; solo cargamos registros. Los no procesados se encolan.
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        Prestamo p = Prestamo::deserialize(line);
        prestamosMap[p.idPrestamo] = p;
        if (!p.procesado) {
            // reenqueamos
            colaPrestamos.push(p.idPrestamo);
        }
    }
    ifs.close();
    return true;
}

void Biblioteca::clearAll() {
    libros.clear();
    usuarios.clear();
    hashAutor.clear();
    grafoRecomendaciones.clear();
    prestamosMap.clear();
    colaPrestamos.clear();
    // no borra archivos
}
