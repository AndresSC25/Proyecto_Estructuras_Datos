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
    cout << "12. Modificar libro\n";
    cout << "13. Eliminar libro\n";
    cout << "14. Modificar usuario\n";
    cout << "15. Eliminar usuario\n";
    cout << "16. Marcar prestamo como devuelto\n";
    cout << "17. Eliminar prestamo (registro)\n";
    cout << "18. Eliminar relacion de recomendacion\n";
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
    bib.cargarPrestamos();

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
            if (bib.guardarPrestamos()) cout << "Prestamos guardados en prestamos.txt\n";
            else cout << "Error guardando prestamos\n";
            break;
        }
        case 9: {
            if (bib.cargarLibros()) cout << "Libros cargados desde libros.txt\n";
            else cout << "No se pudo cargar libros (archivo faltante o formato invalido)\n";
            if (bib.cargarUsuarios()) cout << "Usuarios cargados desde usuarios.txt\n";
            else cout << "No se pudo cargar usuarios\n";
            if (bib.cargarPrestamos()) cout << "Prestamos cargados desde prestamos.txt\n";
            else cout << "No se pudo cargar prestamos\n";
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
        case 12: { // modificar libro
            string isbn = leerLinea("ISBN a modificar: ");
            string titulo = leerLinea("Nuevo titulo: ");
            string autor = leerLinea("Nuevo autor: ");
            string categoria = leerLinea("Nueva categoria: ");
            string disp = leerLinea("Disponible? (1=si/0=no): ");
            bool disponible = (disp == "1");
            Libro nuevo(isbn, titulo, autor, categoria);
            nuevo.disponible = disponible;
            bib.modificarLibro(isbn, nuevo);
            break;
        }
        case 13: {
            string isbn = leerLinea("ISBN a eliminar: ");
            bib.eliminarLibro(isbn);
            break;
        }
        case 14: { // modificar usuario
            cout << "ID usuario a modificar: ";
            int id = leerEntero();
            string nombre = leerLinea("Nuevo nombre: ");
            string email = leerLinea("Nuevo email: ");
            Usuario u(id, nombre, email);
            bib.modificarUsuario(id, u);
            break;
        }
        case 15: {
            cout << "ID usuario a eliminar: ";
            int id = leerEntero();
            bib.eliminarUsuario(id);
            break;
        }
        case 16: {
            string idp = leerLinea("ID prestamo a marcar como devuelto: ");
            bib.marcarComoDevuelto(idp);
            break;
        }
        case 17: {
            string idp = leerLinea("ID prestamo a eliminar: ");
            bib.eliminarPrestamo(idp);
            break;
        }
        case 18: {
            string a = leerLinea("ISBN origen (A): ");
            string b = leerLinea("ISBN a quitar de recomendaciones (B): ");
            bib.eliminarRelacionRecomendacion(a, b);
            cout << "Relacion eliminada (si existia)\n";
            break;
        }
        default:
            cout << "Opcion invalida\n";
        }
    }

    // Al salir, guardar datos
    bib.guardarLibros();
    bib.guardarUsuarios();
    bib.guardarPrestamos();
    cout << "Saliendo... datos guardados.\n";
    return 0;
}
