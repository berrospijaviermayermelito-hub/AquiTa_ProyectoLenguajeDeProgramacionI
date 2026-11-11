// CasosDeUso.h - Una función por caso de uso de la matriz (UC01 a UC14).
// Módulo 2 del sílabo: programación modular con funciones definidas por el usuario.
#pragma once
#include <string>

#include "Sistema.h"

// ---------------- Estudiante ----------------
// codigoEstudiante = código universitario con el que se identificó al entrar al menú.
void uc01BuscarReporte(Sistema& s, const std::string& codigoEstudiante);
void uc02RegistrarObjetoPerdido(Sistema& s, const std::string& codigoEstudiante);
void uc03GestionarReporte(Sistema& s, const std::string& codigoEstudiante);
void uc07SugerirCoincidencias(Sistema& s, const std::string& codigoEstudiante);
void uc09RegistrarSolicitud(Sistema& s, const std::string& codigoEstudiante);
void uc10CancelarSolicitud(Sistema& s, const std::string& codigoEstudiante);

// ---------------- Estudiante y Responsable ----------------
void uc04BuscarObjetoEncontrado(Sistema& s, bool esResponsable);

// ---------------- Responsable de Objetos ----------------
void uc05RegistrarObjetoEncontrado(Sistema& s);
void uc06GestionarObjetoEncontrado(Sistema& s);
void uc08BuscarSolicitud(Sistema& s);
void uc11EvaluarSolicitud(Sistema& s);
void uc12RegistrarEntrega(Sistema& s);
void uc13ListadoGeneral(Sistema& s);
void uc14GenerarReportes(Sistema& s);
