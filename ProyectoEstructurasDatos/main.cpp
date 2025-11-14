#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Biblioteca.h"

using namespace std;

void mostrarMenu() {
    cout << "\n--- Sistema de Gestion de Biblioteca ---\n";
    cout << "1. Agregar libro\n";
    cout << "2. Registrar usuario\n";
    cout << "3. Buscar libro por ISBN\n";
    cout << "4. Buscar libros por titulo (prefijo)\n";
    cout << "5. Buscar por autor\n";
    cout << "6. Solicitar prestamo\n";
    cout << "7. Procesar cola de prestamos\n";
    cout << "8. Guardar datos en archivos (txt)\n";
    cout << "9. Cargar datos desde archivos (txt)\n";
    cout << "10. Agregar relacion de recomendacion\n";
    cout << "11. Ver recomendaciones para ISBN\n";
    cout << "0. Salir\n";
    cout << "Selecciona una opcion: ";
}

int leerEntero() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intenta de nuevo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string leerLinea(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int main() {
    Biblioteca bib;

    // Intentar cargar datos al iniciar (si existen)
    bib.cargarLibros();
    bib.cargarUsuarios();

    while (true) {
        mostrarMenu();
        int op = leerEntero();
        if (op == 0) break;

        switch (op) {
        case 1: {
            string isbn = leerLinea("ISBN: ");
            string titulo = leerLinea("Titulo: ");
            string autor = leerLinea("Autor: ");
            string categoria = leerLinea("Categoria: ");
            Libro l(isbn, titulo, autor, categoria);
            bib.agregarLibro(l);
            break;
        }
        case 2: {
            int id;
            cout << "ID (numero): ";
            id = leerEntero();
            string nombre = leerLinea("Nombre: ");
            string email = leerLinea("Email: ");
            Usuario u(id, nombre, email);
            bib.registrarUsuario(u);
            break;
        }
        case 3: {
            string isbn = leerLinea("ISBN a buscar: ");
            Libro* l = bib.buscarPorISBN(isbn);
            if (!l) cout << "No encontrado\n";
            else cout << "Encontrado: " << l->titulo << " Autor: " << l->autor << " Disponible: " << (l->disponible ? "Si" : "No") << "\n";
            break;
        }
        case 4: {
            string pref = leerLinea("Prefijo de titulo: ");
            auto res = bib.buscarPorTituloPrefijo(pref);
            if (res.empty()) cout << "No hay coincidencias\n";
            else {
                cout << "Coincidencias:\n";
                for (auto p : res) cout << " - " << p->titulo << " (ISBN: " << p->ISBN << ")\n";
            }
            break;
        }
        case 5: {
            string autor = leerLinea("Autor: ");
            auto isbns = bib.buscarISBNsPorAutor(autor);
            if (isbns.empty()) cout << "No se encontraron libros del autor\n";
            else {
                cout << "Libros del autor:\n";
                for (auto& i : isbns) {
                    auto l = bib.buscarPorISBN(i);
                    if (l) cout << " - " << l->titulo << " (ISBN: " << l->ISBN << ")\n";
                }
            }
            break;
        }
        case 6: {
            string isbn = leerLinea("ISBN a prestar: ");
            cout << "ID usuario: ";
            int uid = leerEntero();
            bib.solicitarPrestamo(isbn, uid);
            break;
        }
        case 7: {
            bib.procesarColaPrestamos();
            break;
        }
        case 8: {
            if (bib.guardarLibros()) cout << "Libros guardados en libros.txt\n";
            else cout << "Error guardando libros\n";
            if (bib.guardarUsuarios()) cout << "Usuarios guardados en usuarios.txt\n";
            else cout << "Error guardando usuarios\n";
            break;
        }
        case 9: {
            if (bib.cargarLibros()) cout << "Libros cargados desde libros.txt\n";
            else cout << "No se pudo cargar libros (archivo faltante o formato invalido)\n";
            if (bib.cargarUsuarios()) cout << "Usuarios cargados desde usuarios.txt\n";
            else cout << "No se pudo cargar usuarios\n";
            break;
        }
        case 10: {
            string a = leerLinea("ISBN origen (A): ");
            string b = leerLinea("ISBN recomendado (B): ");
            bib.agregarRelacionRecomendacion(a, b);
            cout << "Relacion agregada\n";
            break;
        }
        case 11: {
            string isbn = leerLinea("ISBN: ");
            auto recs = bib.recomendarPara(isbn);
            if (recs.empty()) cout << "Sin recomendaciones\n";
            else {
                cout << "Recomendaciones:\n";
                for (auto& r : recs) {
                    if (auto l = bib.buscarPorISBN(r))
                        cout << " - " << l->titulo << " (ISBN: " << l->ISBN << ")\n";
                    else cout << " - ISBN: " << r << " (no encontrado en inventario)\n";
                }
            }
            break;
        }
        default:
            cout << "Opcion invalida\n";
        }
    }

    // Al salir, guardar datos
    bib.guardarLibros();
    bib.guardarUsuarios();
    cout << "Saliendo... datos guardados.\n";
    return 0;
}
