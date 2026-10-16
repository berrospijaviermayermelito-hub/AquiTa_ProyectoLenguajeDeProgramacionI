#include "Menu.h"

#include <iostream>

#include "CasosDeUso.h"
#include "Utilidades.h"

// Cada opción del menú muestra entre corchetes el caso de uso de la matriz que ejecuta.

static void menuEstudiante(Sistema& s) {
    std::cout << "\n=== Menú de Estudiante ===\n";
    // Sin inicio de sesión (mejora futura): el estudiante se identifica con su código
    // universitario y solo puede ver y editar sus propios reportes.
    std::string codigo = util::leerObligatorio("Ingresa tu código universitario: ");

    while (true) {
        std::cout << "\n--- Estudiante (" << codigo << ") ---\n"
                     "  1. Buscar mis reportes                  [UC01]\n"
                     "  2. Registrar un objeto perdido          [UC02]\n"
                     "  3. Editar o cancelar un reporte         [UC03]\n"
                     "  4. Buscar objetos encontrados           [UC04]\n"
                     "  5. Ver coincidencias de mi reporte      [UC07]\n"
                     "  6. Solicitar la recuperación de un objeto [UC09]\n"
                     "  7. Cancelar una solicitud               [UC10]\n"
                     "  0. Volver\n";
        int op = util::leerEntero("Elige una opción: ", 0, 7);
        if (op == 0) return;
        switch (op) {
            case 1: uc01BuscarReporte(s, codigo); break;
            case 2: uc02RegistrarObjetoPerdido(s, codigo); break;
            case 3: uc03GestionarReporte(s, codigo); break;
            case 4: uc04BuscarObjetoEncontrado(s, false); break;
            case 5: uc07SugerirCoincidencias(s, codigo); break;
            case 6: uc09RegistrarSolicitud(s, codigo); break;
            case 7: uc10CancelarSolicitud(s, codigo); break;
            default: break;
        }
        util::pausa();
    }
}

static void menuResponsable(Sistema& s) {
    while (true) {
        std::cout << "\n--- Responsable de Objetos ---\n"
                     "  1. Registrar un objeto encontrado       [UC05]\n"
                     "  2. Buscar objetos encontrados           [UC04]\n"
                     "  3. Editar, desactivar o depurar objetos [UC06]\n"
                     "  4. Buscar solicitudes                   [UC08]\n"
                     "  5. Evaluar una solicitud                [UC11]\n"
                     "  6. Registrar la entrega de un objeto    [UC12]\n"
                     "  7. Listado general                      [UC13]\n"
                     "  8. Generar reportes                     [UC14]\n"
                     "  0. Volver\n";
        int op = util::leerEntero("Elige una opción: ", 0, 8);
        if (op == 0) return;
        switch (op) {
            case 1: uc05RegistrarObjetoEncontrado(s); break;
            case 2: uc04BuscarObjetoEncontrado(s, true); break;
            case 3: uc06GestionarObjetoEncontrado(s); break;
            case 4: uc08BuscarSolicitud(s); break;
            case 5: uc11EvaluarSolicitud(s); break;
            case 6: uc12RegistrarEntrega(s); break;
            case 7: uc13ListadoGeneral(s); break;
            case 8: uc14GenerarReportes(s); break;
            default: break;
        }
        util::pausa();
    }
}

void menuPrincipal(Sistema& s) {
    while (true) {
        std::cout << "\n==============================================\n"
                     "  AquiTa - Objetos perdidos y encontrados\n"
                     "  Universidad Científica del Sur\n"
                     "==============================================\n"
                     "  1. Soy estudiante\n"
                     "  2. Soy responsable de objetos\n"
                     "  0. Salir\n";
        int op = util::leerEntero("Elige una opción: ", 0, 2);
        if (op == 0) {
            std::cout << "\nHasta luego.\n";
            return;
        }
        if (op == 1) {
            menuEstudiante(s);
        } else {
            menuResponsable(s);
        }
    }
}
