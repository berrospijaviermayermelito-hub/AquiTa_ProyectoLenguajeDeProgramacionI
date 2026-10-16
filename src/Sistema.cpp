#include "Sistema.h"

#include <filesystem>
#include <iostream>

static const std::string RUTA_REPORTES = "datos/reportes.txt";
static const std::string RUTA_OBJETOS = "datos/objetos.txt";
static const std::string RUTA_SOLICITUDES = "datos/solicitudes.txt";
static const std::string RUTA_CONTADORES = "datos/contadores.txt";

void Sistema::cargarTodo() {
    // Crea las carpetas si no existen (primera ejecución).
    std::filesystem::create_directories("datos");
    std::filesystem::create_directories("reportes");

    int omitidas = reportes.cargar(RUTA_REPORTES) + objetos.cargar(RUTA_OBJETOS) +
                   solicitudes.cargar(RUTA_SOLICITUDES);
    if (omitidas > 0) {
        std::cout << "Aviso: se omitieron " << omitidas
                  << " línea(s) inválida(s) al cargar los archivos de datos.\n";
    }
    if (!contadores.cargar(RUTA_CONTADORES)) {
        std::cout << "Aviso: no se pudo leer datos/contadores.txt; los códigos empiezan de nuevo.\n";
    }
    std::cout << "Datos cargados: " << reportes.cantidad() << " reporte(s), " << objetos.cantidad()
              << " objeto(s) encontrado(s), " << solicitudes.cantidad() << " solicitud(es).\n";
}

bool Sistema::guardarTodo() const {
    bool ok = reportes.guardar(RUTA_REPORTES) && objetos.guardar(RUTA_OBJETOS) &&
              solicitudes.guardar(RUTA_SOLICITUDES) && contadores.guardar(RUTA_CONTADORES);
    if (!ok) std::cout << "ERROR: no se pudieron guardar los datos en la carpeta 'datos'.\n";
    return ok;
}
