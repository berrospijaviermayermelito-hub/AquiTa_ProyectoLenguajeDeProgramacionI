// Sistema.h - Agrupa los gestores y se encarga de cargar y guardar todo (SUPL01 y SUPL02).
#pragma once
#include "Contadores.h"
#include "GestorObjetos.h"
#include "GestorReportes.h"
#include "GestorSolicitudes.h"

class Sistema {
public:
    GestorReportes reportes;
    GestorObjetos objetos;
    GestorSolicitudes solicitudes;
    Contadores contadores;

    // SUPL02: se llama una vez al iniciar el programa.
    void cargarTodo();

    // SUPL01: se llama después de cada alta, edición o baja. Devuelve false si no pudo guardar.
    bool guardarTodo() const;
};
