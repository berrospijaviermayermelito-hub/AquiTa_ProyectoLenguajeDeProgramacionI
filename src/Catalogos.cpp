#include "Catalogos.h"

namespace cat {

const std::vector<std::string> CATEGORIAS = {
    "electrónicos", "documentos", "ropa", "útiles", "accesorios", "llaves", "otros"};

// PROVISIONAL: el equipo debe entregar la lista real de lugares del campus.
// Al cambiarla, los archivos ya guardados siguen funcionando (se guarda el texto).
const std::vector<std::string> ZONAS = {
    "ingreso y control de acceso", "patio de ingreso", "biblioteca", "cafetería", 
    "auditorio", "aulas", "laboratorios", "pasillos y zonas de estudio", 
    "oficinas y atención al cliente (admisión, ventas y SAED)", "sala de profesores", 
    "zona de objetos perdidos", "estacionamiento", "escaleras y graderías", 
    "azotea", "terrazas y balcones", "servicios higiénicos", "otros"};

// Pisos de los edificios y opción de no aplica
const std::vector<std::string> PISOS = {
    "edificio B: piso 1-B", "edificio B: piso 2-B", "edificio B: piso 3-B", "edificio B: piso 4-B", 
    "edificio B: piso 5-B", "edificio B: piso 6-B", "edificio B: piso 7-B", "edificio B: piso 8-B", 
    "edificio B: piso 9-B", "edificio B: piso 10-B",
    "edificio C: piso 1-C", "edificio C: piso 2-C", "edificio C: piso 3-C", "edificio C: piso 4-C", 
    "edificio C: piso 5-C", "edificio C: piso 6-C", "edificio C: piso 7-C", "edificio C: piso 8-C", 
    "edificio C: piso 9-C", "no aplica"};

// Lugar donde queda guardado (elegido de la lista de zonas)
const std::vector<std::string> LUGARES_GUARDADO = ZONAS;

// Mantenido por compatibilidad
const std::vector<std::string> LUGARES = ZONAS;
// ---- Nombres de los estados (el índice coincide con el valor del enum) ----
static const char* NOMBRES_REPORTE[] = {"pendiente", "recuperado", "cancelado"};
static const char* NOMBRES_OBJETO[] = {"disponible", "en proceso", "devuelto", "desactivado"};
static const char* NOMBRES_SOLICITUD[] = {"pendiente", "aprobada", "rechazada", "cancelada",
                                          "entregada"};

std::string aTexto(EstadoReporte e) { return NOMBRES_REPORTE[static_cast<int>(e)]; }
std::string aTexto(EstadoObjeto e) { return NOMBRES_OBJETO[static_cast<int>(e)]; }
std::string aTexto(EstadoSolicitud e) { return NOMBRES_SOLICITUD[static_cast<int>(e)]; }

bool desdeTexto(const std::string& t, EstadoReporte& e) {
    for (int i = 0; i < 3; ++i) {
        if (t == NOMBRES_REPORTE[i]) {
            e = static_cast<EstadoReporte>(i);
            return true;
        }
    }
    return false;
}

bool desdeTexto(const std::string& t, EstadoObjeto& e) {
    for (int i = 0; i < 4; ++i) {
        if (t == NOMBRES_OBJETO[i]) {
            e = static_cast<EstadoObjeto>(i);
            return true;
        }
    }
    return false;
}

bool desdeTexto(const std::string& t, EstadoSolicitud& e) {
    for (int i = 0; i < 5; ++i) {
        if (t == NOMBRES_SOLICITUD[i]) {
            e = static_cast<EstadoSolicitud>(i);
            return true;
        }
    }
    return false;
}

}  // namespace cat
