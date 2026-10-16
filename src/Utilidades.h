// Utilidades.h - Funciones de apoyo: texto, entrada por consola y archivos.
// Módulo 1 del sílabo: estructuras de control, cadenas y funciones.
#pragma once
#include <string>
#include <vector>

namespace util {

// Separador de campos en los archivos de texto. Se prohíbe en los datos
// que escribe el usuario (ver leerObligatorio).
const char DELIM = '|';

// ---------- Texto ----------
std::string recortar(const std::string& s);      // quita espacios al inicio y al final
std::string normalizar(const std::string& s);    // minúsculas y sin tildes (para buscar)
bool contiene(const std::string& texto, const std::string& patron);  // búsqueda parcial
std::vector<std::string> separar(const std::string& linea, char delim = DELIM);
std::string unir(const std::vector<std::string>& campos, char delim = DELIM);

// ---------- Entrada por consola ----------
// Todas leen con getline (así se aceptan espacios) y vuelven a pedir el dato
// si no es válido (RNF02).
std::string leerLinea(const std::string& mensaje);
std::string leerObligatorio(const std::string& mensaje);   // no vacío y sin '|'
int leerEntero(const std::string& mensaje, int minimo, int maximo);
bool confirmar(const std::string& mensaje);                // pregunta s/n
std::string elegirDeLista(const std::string& titulo,
                          const std::vector<std::string>& opciones);
void pausa();

// ---------- Archivos de texto ----------
// Devuelve las líneas no vacías. Si el archivo no existe devuelve un vector vacío.
std::vector<std::string> leerLineas(const std::string& ruta);
bool escribirLineas(const std::string& ruta, const std::vector<std::string>& lineas);

}  // namespace util
