// Coincidencias.h - Puntaje de coincidencia entre un reporte y un objeto encontrado (FEAT11, UC07).
// Es el aporte innovador del proyecto: hay que poder explicar la fórmula con un ejemplo.
#pragma once
#include "ObjetoEncontrado.h"
#include "ObjetoPerdido.h"

// Fórmula de la matriz (UC07 / FEAT11):
//   puntaje = 50 * factorNombre + 30 * factorCategoria + 20 * factorLugar
//   - factorNombre   = proporción de palabras del reporte presentes en el objeto (0 a 1).
//   - factorCategoria = 1 si las categorías coinciden, 0 si no.
//   - factorLugar     = 1 si el lugar de la pérdida y del hallazgo coinciden, 0 si no.
// En CasosDeUso (uc07) se recorren los objetos «disponible», se calcula el puntaje,
// se conservan los de 30 o más, se ordenan de mayor a menor con std::sort y se muestran hasta 5.
double calcularPuntaje(const ObjetoPerdido& reporte, const ObjetoEncontrado& objeto);
