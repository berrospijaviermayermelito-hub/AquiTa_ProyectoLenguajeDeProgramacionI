// Objeto.h - Clase base abstracta de ObjetoPerdido y ObjetoEncontrado.
// POO (criterio 1 de la rúbrica): encapsulamiento, herencia y polimorfismo.
//
// Los atributos comunes son los de la matriz: código, nombre, descripción,
// categoría, fecha y lugar.
//   - En un ObjetoPerdido, fecha/lugar = cuándo y dónde se perdió.
//   - En un ObjetoEncontrado, fecha/lugar = cuándo y dónde se halló.
#pragma once
#include <string>
#include <vector>

#include "Fecha.h"

class Objeto {
public:
    Objeto() = default;
    Objeto(const std::string& codigo, const std::string& nombre, const std::string& descripcion,
           const std::string& categoria, const Fecha& fecha, const std::string& lugar);
    virtual ~Objeto() = default;

    // --- getters ---
    const std::string& codigo() const { return codigo_; }
    const std::string& nombre() const { return nombre_; }
    const std::string& descripcion() const { return descripcion_; }
    const std::string& categoria() const { return categoria_; }
    const Fecha& fecha() const { return fecha_; }
    const std::string& lugar() const { return lugar_; }

    // --- setters (se usan al editar: FEAT03 y FEAT07) ---
    void setNombre(const std::string& v) { nombre_ = v; }
    void setDescripcion(const std::string& v) { descripcion_ = v; }
    void setCategoria(const std::string& v) { categoria_ = v; }
    void setFecha(const Fecha& v) { fecha_ = v; }
    void setLugar(const std::string& v) { lugar_ = v; }

    // --- polimorfismo: cada clase hija completa o cambia estos métodos ---
    virtual void mostrar() const;                 // ficha completa
    virtual std::string resumen() const;          // una línea para listados
    virtual std::string aLinea() const = 0;       // una línea para el archivo de texto

protected:
    // Campos comunes, en el orden en que se escriben en el archivo.
    std::vector<std::string> camposBase() const;
    // Lee los 6 primeros campos de una línea ya separada. Devuelve false si fallan.
    bool cargarBase(const std::vector<std::string>& campos);

private:
    std::string codigo_;
    std::string nombre_;
    std::string descripcion_;
    std::string categoria_;
    Fecha fecha_;
    std::string lugar_;
};
