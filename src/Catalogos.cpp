#include "Catalogos.h"

namespace cat {

const std::vector<std::string> CATEGORIAS = {
    "electrónicos", "documentos", "ropa", "útiles", "accesorios", "llaves", "otros"};

// Zonas del campus (hoja «Estados y listas» de la matriz).
const std::vector<std::string> ZONAS = {
    "ingreso y control de acceso", "patio de ingreso", "biblioteca", "cafetería", 
    "auditorio", "aulas", "laboratorios", "pasillos y zonas de estudio", 
    "oficinas y atención al cliente (admisión, ventas y SAED)", "sala de profesores", 
    "zona de objetos perdidos", "estacionamiento", "escaleras y graderías", 
    "azotea", "terrazas y balcones", "servicios higiénicos", "otros"};

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
