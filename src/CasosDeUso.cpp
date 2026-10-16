#include "CasosDeUso.h"

#include <iostream>
#include <vector>

#include "Catalogos.h"
#include "Coincidencias.h"
#include "Fecha.h"
#include "Utilidades.h"

// ============================================================ Ayudas internas

static bool mismoCodigo(const std::string& a, const std::string& b) {
    return util::normalizar(a) == util::normalizar(b);
}

static void pendiente(const std::string& uc, const std::string& nombre) {
    std::cout << "\n[" << uc << "] " << nombre
              << ": aún no implementado.\n"
                 "Mira el TODO de esta función en src/CasosDeUso.cpp: ahí están las reglas de la matriz.\n";
}

// ============================================================ UC02 (LISTO)
// FEAT02: registrar un objeto perdido, verificando antes que no esté ya registrado.
void uc02RegistrarObjetoPerdido(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC02] Registrar objeto perdido ===\n";
    std::string nombre = util::leerObligatorio("Nombre del objeto: ");

    // Paso 1 de FEAT02: buscar si este estudiante ya reportó algo parecido.
    // filtrar() recibe una función lambda que decide qué reportes cumplen.
    std::vector<ObjetoPerdido*> parecidos = s.reportes.filtrar([&](const ObjetoPerdido& r) {
        return mismoCodigo(r.contactoCodigo(), codigoEstudiante) &&
               r.estado() == cat::EstadoReporte::Pendiente && util::contiene(r.nombre(), nombre);
    });
    if (!parecidos.empty()) {
        std::cout << "\nYa tienes reportes pendientes parecidos:\n";
        for (const ObjetoPerdido* r : parecidos) std::cout << "  " << r->resumen() << "\n";
        if (!util::confirmar("¿Registrar un reporte nuevo de todos modos?")) {
            std::cout << "Registro cancelado.\n";
            return;
        }
    }

    // Paso 2: pedir el resto de datos. Categoría y lugar se eligen de una lista.
    std::string descripcion = util::leerObligatorio("Descripción: ");
    std::string categoria = util::elegirDeLista("Categoría:", cat::CATEGORIAS);
    Fecha fecha = Fecha::leer("Fecha aproximada de la pérdida");
    std::string lugar = util::elegirDeLista("Lugar donde se perdió:", cat::LUGARES);
    std::string nombres = util::leerObligatorio("Tus nombres: ");
    std::string telefono = util::leerObligatorio("Teléfono de contacto: ");
    std::string reservada = util::leerObligatorio(
        "Característica reservada (un detalle que solo tú conoces; no se mostrará a otros): ");

    // Paso 3: el sistema asigna el código; el estado inicial es «pendiente».
    ObjetoPerdido reporte(s.contadores.nuevoCodigoReporte(), nombre, descripcion, categoria, fecha,
                          lugar, nombres, codigoEstudiante, telefono, reservada);
    s.reportes.agregar(reporte);
    s.guardarTodo();  // SUPL01
    std::cout << "\nReporte registrado con el código " << reporte.codigo() << ".\n";
}

// ============================================================ UC05 (LISTO)
// FEAT06: registrar un objeto encontrado, verificando antes que no esté ya registrado.
void uc05RegistrarObjetoEncontrado(Sistema& s) {
    std::cout << "\n=== [UC05] Registrar objeto encontrado ===\n";
    std::string nombre = util::leerObligatorio("Nombre del objeto: ");

    std::vector<ObjetoEncontrado*> parecidos = s.objetos.filtrar([&](const ObjetoEncontrado& o) {
        return o.estado() != cat::EstadoObjeto::Desactivado && util::contiene(o.nombre(), nombre);
    });
    if (!parecidos.empty()) {
        std::cout << "\nYa hay objetos registrados con un nombre parecido:\n";
        for (const ObjetoEncontrado* o : parecidos) std::cout << "  " << o->resumen() << "\n";
        if (!util::confirmar("¿Es otro objeto distinto y quieres registrarlo?")) {
            std::cout << "Registro cancelado.\n";
            return;
        }
    }

    std::string descripcion = util::leerObligatorio("Descripción: ");
    std::string categoria = util::elegirDeLista("Categoría:", cat::CATEGORIAS);
    Fecha fecha = Fecha::leer("Fecha del hallazgo");
    std::string lugar = util::elegirDeLista("Lugar del hallazgo:", cat::LUGARES);
    std::string guardado = util::leerObligatorio("Lugar donde queda guardado: ");

    ObjetoEncontrado objeto(s.contadores.nuevoCodigoObjeto(), nombre, descripcion, categoria, fecha,
                            lugar, guardado);  // estado inicial: disponible
    s.objetos.agregar(objeto);
    s.guardarTodo();  // SUPL01
    std::cout << "\nObjeto registrado con el código " << objeto.codigo() << ".\n";
}

// ============================================================ UC04 (LISTO)
// FEAT05 (Responsable), FEAT09 (Estudiante) y FEAT10 (ficha pública).
void uc04BuscarObjetoEncontrado(Sistema& s, bool esResponsable) {
    std::cout << "\n=== [UC04] Buscar objeto encontrado ===\n";
    std::cout << "Buscar por:\n"
                 "  1. Nombre\n"
                 "  2. Categoría\n"
                 "  3. Lugar del hallazgo\n";
    if (esResponsable) std::cout << "  4. Código\n";
    // TODO (FEAT05): agregar la búsqueda por fecha del hallazgo para el Responsable.
    int opcion = util::leerEntero("Elige una opción: ", 1, esResponsable ? 4 : 3);

    std::string valor;
    if (opcion == 2) {
        valor = util::elegirDeLista("Categoría:", cat::CATEGORIAS);
    } else if (opcion == 3) {
        valor = util::elegirDeLista("Lugar del hallazgo:", cat::LUGARES);
    } else {
        valor = util::leerObligatorio(opcion == 1 ? "Nombre (o parte del nombre): " : "Código: ");
    }

    std::vector<ObjetoEncontrado*> resultados = s.objetos.filtrar([&](const ObjetoEncontrado& o) {
        // El estudiante solo ve objetos disponibles (FEAT09); el Responsable ve todos (FEAT05).
        if (!esResponsable && o.estado() != cat::EstadoObjeto::Disponible) return false;
        switch (opcion) {
            case 1: return util::contiene(o.nombre(), valor);
            case 2: return util::normalizar(o.categoria()) == util::normalizar(valor);
            case 3: return util::normalizar(o.lugar()) == util::normalizar(valor);
            default: return util::contiene(o.codigo(), valor);
        }
    });

    if (resultados.empty()) {
        std::cout << "\nNo se encontraron objetos.\n";
        return;
    }
    std::cout << "\nSe encontraron " << resultados.size() << " objeto(s):\n";
    for (const ObjetoEncontrado* o : resultados) std::cout << "  " << o->resumen() << "\n";

    std::string codigo = util::leerLinea("\nEscribe un código para ver su ficha (Enter para volver): ");
    if (codigo.empty()) return;
    for (const ObjetoEncontrado* o : resultados) {
        if (mismoCodigo(o->codigo(), codigo)) {
            std::cout << "\n--- Ficha " << o->codigo() << " ---\n";
            if (esResponsable) {
                o->mostrar();       // el Responsable ve todos los datos
            } else {
                o->mostrarFicha();  // FEAT10: el estudiante ve solo la ficha pública
            }
            return;
        }
    }
    std::cout << "Ese código no está en los resultados.\n";
}

// ============================================================ UC13 (LISTO)
// FEAT17: listado general de reportes o de objetos encontrados, filtrado por estado.
void uc13ListadoGeneral(Sistema& s) {
    std::cout << "\n=== [UC13] Listado general ===\n"
                 "  1. Reportes de objetos perdidos\n"
                 "  2. Objetos encontrados\n";
    int que = util::leerEntero("Elige una opción: ", 1, 2);

    // Se arma la lista de estados a partir de los enums (0 = todos).
    std::vector<std::string> estados;
    if (que == 1) {
        for (int i = 0; i < 3; ++i) estados.push_back(cat::aTexto(static_cast<cat::EstadoReporte>(i)));
    } else {
        for (int i = 0; i < 4; ++i) estados.push_back(cat::aTexto(static_cast<cat::EstadoObjeto>(i)));
    }
    std::cout << "Filtrar por estado:\n  0. Todos\n";
    for (size_t i = 0; i < estados.size(); ++i) std::cout << "  " << (i + 1) << ". " << estados[i] << "\n";
    int e = util::leerEntero("Elige una opción: ", 0, static_cast<int>(estados.size()));

    int mostrados = 0;
    std::cout << "\n";
    if (que == 1) {
        for (const ObjetoPerdido& r : s.reportes.todos()) {
            if (e == 0 || static_cast<int>(r.estado()) == e - 1) {
                std::cout << "  " << r.resumen() << "\n";
                ++mostrados;
            }
        }
    } else {
        for (const ObjetoEncontrado& o : s.objetos.todos()) {
            if (e == 0 || static_cast<int>(o.estado()) == e - 1) {
                std::cout << "  " << o.resumen() << "\n";
                ++mostrados;
            }
        }
    }
    if (mostrados == 0) std::cout << "  (no hay registros con ese estado)\n";
    std::cout << "\nTotal mostrado: " << mostrados << "\n";
}

// ============================================================ PENDIENTES (TODO)

// UC01 - FEAT01: buscar un reporte por código, nombre, categoría o estado.
// TODO: usar s.reportes.filtrar(...) igual que uc04. REGLA de privacidad: el estudiante
//       solo ve SUS reportes (mismoCodigo(r.contactoCodigo(), codigoEstudiante)).
void uc01BuscarReporte(Sistema& /*s*/, const std::string& /*codigoEstudiante*/) {
    pendiente("UC01", "Buscar reporte de objeto perdido");
}

// UC03 - FEAT03 y FEAT04: editar o cancelar un reporte PENDIENTE propio.
// TODO: buscar el reporte (UC01); editar con los setters (setNombre, setCategoria, ...)
//       o cancelar con setEstado(cat::EstadoReporte::Cancelado); luego s.guardarTodo().
//       Decide y documenta qué pasa si el reporte tiene una solicitud pendiente.
void uc03GestionarReporte(Sistema& /*s*/, const std::string& /*codigoEstudiante*/) {
    pendiente("UC03", "Gestionar reporte de objeto perdido");
}

// UC07 - FEAT11: sugerir los objetos disponibles que más se parecen a un reporte pendiente.
// TODO: implementar calcularPuntaje() en Coincidencias.cpp; recorrer los objetos
//       «disponible», calcular el puntaje, quedarse con los de 30 o más, ordenar de mayor
//       a menor con std::sort y mostrar de 3 a 5.
void uc07SugerirCoincidencias(Sistema& /*s*/, const std::string& /*codigoEstudiante*/) {
    pendiente("UC07", "Sugerir coincidencias de objeto");
}

// UC09 - FEAT13: registrar una solicitud de recuperación.
// TODO: elegir un reporte pendiente PROPIO y un objeto «disponible»; crear
//       Solicitud(s.contadores.nuevoCodigoSolicitud(), codReporte, codObjeto, Fecha::hoy());
//       s.solicitudes.agregar(...); objeto -> EnProceso; s.guardarTodo().
//       Validar (RNF06): el objeto debe estar disponible.
void uc09RegistrarSolicitud(Sistema& /*s*/, const std::string& /*codigoEstudiante*/) {
    pendiente("UC09", "Registrar solicitud de recuperación");
}

// UC10 - FEAT14: cancelar una solicitud pendiente propia.
// TODO: solicitud -> Cancelada; el objeto vuelve a Disponible; s.guardarTodo().
void uc10CancelarSolicitud(Sistema& /*s*/, const std::string& /*codigoEstudiante*/) {
    pendiente("UC10", "Cancelar solicitud de recuperación");
}

// UC06 - FEAT07 y FEAT08: editar, desactivar y depurar objetos encontrados.
// TODO: buscar el objeto (como uc04); editar con setters; «desactivar» = setEstado(Desactivado);
//       depurar = s.objetos.depurar(codigo) (solo funciona con objetos ya desactivados).
//       Limita la edición manual del estado para no contradecir las solicitudes (RNF06).
void uc06GestionarObjetoEncontrado(Sistema& /*s*/) {
    pendiente("UC06", "Gestionar objeto encontrado");
}

// UC08 - FEAT12: buscar una solicitud por código, nombre o código universitario del estudiante,
//       objeto o estado. TODO: s.solicitudes.filtrar(...); el nombre y el código del estudiante
//       se obtienen del reporte con s.reportes.buscarPorCodigo(sol.codigoReporte()).
void uc08BuscarSolicitud(Sistema& /*s*/) {
    pendiente("UC08", "Buscar solicitud de recuperación");
}

// UC11 - FEAT15: evaluar una solicitud pendiente.
// TODO: mostrar al Responsable la característica reservada del reporte para que la compare;
//       registrar Aprobada o Rechazada con una observación; si se rechaza, el objeto vuelve
//       a Disponible; s.guardarTodo().
void uc11EvaluarSolicitud(Sistema& /*s*/) {
    pendiente("UC11", "Evaluar solicitud de recuperación");
}

// UC12 - FEAT16: registrar la entrega de un objeto con solicitud aprobada.
// TODO: pedir fecha de devolución (Fecha::leer) y nombres y código de quien recibe;
//       solicitud.setEntrega(...) y estado Entregada; objeto -> Devuelto; reporte -> Recuperado.
void uc12RegistrarEntrega(Sistema& /*s*/) {
    pendiente("UC12", "Registrar entrega de objeto");
}

// UC14 - FEAT19, FEAT20 y FEAT21: reportes guardados en archivo de texto (carpeta reportes/).
// TODO: pedir el rango de fechas con Fecha::leer y filtrar con los operadores <= y >= de Fecha.
//       FEAT19 -> reportes/reporte_perdidos_encontrados.txt
//       FEAT20 -> reportes/reporte_devueltos.txt
//       FEAT21 -> reportes/reporte_sin_reclamar.txt (días guardados = Fecha::diasEntre(o.fecha(), Fecha::hoy()))
//       Escribir con std::ofstream (o util::escribirLineas) y avisar la ruta del archivo.
void uc14GenerarReportes(Sistema& /*s*/) {
    pendiente("UC14", "Generar reportes básicos");
}
