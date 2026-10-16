// CasosDeUso.h - Una función por caso de uso de la matriz (UC01 a UC14).
// Módulo 2 del sílabo: programación modular con funciones definidas por el usuario.
//
// Estado actual (ver README.md):
//   Implementados como ejemplo del patrón: UC02, UC04, UC05, UC13.
//   Pendientes (TODO con las reglas de la matriz en CasosDeUso.cpp): el resto.
#pragma once
#include <string>

#include "Sistema.h"

// ---------------- Estudiante ----------------
// codigoEstudiante = código universitario con el que se identificó al entrar al menú.
void uc01BuscarReporte(Sistema& s, const std::string& codigoEstudiante);              // TODO
void uc02RegistrarObjetoPerdido(Sistema& s, const std::string& codigoEstudiante);     // listo
void uc03GestionarReporte(Sistema& s, const std::string& codigoEstudiante);           // TODO
void uc07SugerirCoincidencias(Sistema& s, const std::string& codigoEstudiante);       // TODO
void uc09RegistrarSolicitud(Sistema& s, const std::string& codigoEstudiante);         // TODO
void uc10CancelarSolicitud(Sistema& s, const std::string& codigoEstudiante);          // TODO

// ---------------- Estudiante y Responsable ----------------
void uc04BuscarObjetoEncontrado(Sistema& s, bool esResponsable);                      // listo

// ---------------- Responsable de Objetos ----------------
void uc05RegistrarObjetoEncontrado(Sistema& s);                                       // listo
void uc06GestionarObjetoEncontrado(Sistema& s);                                       // TODO
void uc08BuscarSolicitud(Sistema& s);                                                 // TODO
void uc11EvaluarSolicitud(Sistema& s);                                                // TODO
void uc12RegistrarEntrega(Sistema& s);                                                // TODO
void uc13ListadoGeneral(Sistema& s);                                                  // listo
void uc14GenerarReportes(Sistema& s);                                                 // TODO
