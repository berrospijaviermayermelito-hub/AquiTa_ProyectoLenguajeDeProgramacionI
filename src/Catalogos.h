// Catalogos.h - Listas predefinidas y estados (hoja "Estados y listas" de la matriz).
// Módulo 2 del sílabo: estructuras de datos estáticas (listas fijas).
#pragma once
#include <string>
#include <vector>

namespace cat {

// Listas entre las que el usuario elige (en vez de escribir texto libre).
extern const std::vector<std::string> CATEGORIAS;
extern const std::vector<std::string> LUGARES;  // TODO: completar con los lugares reales del campus

// Estados de cada entidad (misma tabla que la matriz).
enum class EstadoReporte { Pendiente, Recuperado, Cancelado };
enum class EstadoObjeto { Disponible, EnProceso, Devuelto, Desactivado };
enum class EstadoSolicitud { Pendiente, Aprobada, Rechazada, Cancelada, Entregada };

// Estado -> texto (se usa al mostrar y al guardar en archivo).
std::string aTexto(EstadoReporte e);
std::string aTexto(EstadoObjeto e);
std::string aTexto(EstadoSolicitud e);

// Texto -> estado (se usa al cargar desde archivo). Devuelve false si no existe.
bool desdeTexto(const std::string& texto, EstadoReporte& e);
bool desdeTexto(const std::string& texto, EstadoObjeto& e);
bool desdeTexto(const std::string& texto, EstadoSolicitud& e);

}  // namespace cat
