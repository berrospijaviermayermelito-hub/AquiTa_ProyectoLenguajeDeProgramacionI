// Coincidencias.h - Puntaje de coincidencia entre un reporte y un objeto encontrado (FEAT11, UC07).
// Es el aporte innovador del proyecto: hay que poder explicar la fórmula con un ejemplo.
#pragma once
#include "ObjetoEncontrado.h"
#include "ObjetoPerdido.h"

// TODO (UC07 / FEAT11): implementar según la matriz.
//   puntaje = 50 * factorNombre + 30 * factorCategoria + 20 * factorLugar
//   - factorNombre   = proporción de palabras en común entre los nombres (0 a 1).
//   - factorCategoria = 1 si las categorías coinciden, 0 si no.
//   - factorLugar     = 1 si el lugar de la pérdida y del hallazgo coinciden, 0 si no.
// Pistas: separar el nombre en palabras con std::istringstream; comparar con
// util::normalizar() para ignorar mayúsculas y tildes.
// Luego, en CasosDeUso (uc07), recorrer los objetos «disponible», calcular el puntaje,
// quedarse con los de 30 o más, ordenarlos de mayor a menor con std::sort y mostrar de 3 a 5.
double calcularPuntaje(const ObjetoPerdido& reporte, const ObjetoEncontrado& objeto);
