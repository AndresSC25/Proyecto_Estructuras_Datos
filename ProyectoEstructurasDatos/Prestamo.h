#pragma once
#include "Libro.h"
#include "Usuario.h"

struct Prestamo {
    string idPrestamo;
    string ISBN_libro;
    int usuarioID;
    string fechaInicio;
    string fechaFin;

    Prestamo() = default;
    Prestamo(const string& idp, const string& isbn, int uid, const string& fi, const string& ff = "")
        : idPrestamo(idp), ISBN_libro(isbn), usuarioID(uid), fechaInicio(fi), fechaFin(ff) {
    }
};
