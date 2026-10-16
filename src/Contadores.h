// Contadores.h - Genera los códigos REP-001, OBJ-001 y SOL-001 (los asigna el sistema).
// El último número usado se guarda en datos/contadores.txt para que un código nunca
// se repita, ni siquiera después de depurar registros.
#pragma once
#include <string>

class Contadores {
public:
    bool cargar(const std::string& ruta);        // si el archivo no existe, empieza en 0
    bool guardar(const std::string& ruta) const;

    std::string nuevoCodigoReporte();    // "REP-001", "REP-002", ...
    std::string nuevoCodigoObjeto();     // "OBJ-001", ...
    std::string nuevoCodigoSolicitud();  // "SOL-001", ...

private:
    static std::string formatear(const std::string& prefijo, int numero);

    int reporte_ = 0;
    int objeto_ = 0;
    int solicitud_ = 0;
};
