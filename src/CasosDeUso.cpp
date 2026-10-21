#include "CasosDeUso.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

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

// ============================================================ UC01 (LISTO)
// FEAT01: permitir buscar un reporte de objeto perdido por: código, nombre, categoría o estado.
// REGLA: El estudiante se identifica con su código universitario y solo ve sus propios reportes.
void uc01BuscarReporte(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC01] Buscar mis reportes de objetos perdidos ===\n";
    
    // Primero filtramos exclusivamente los reportes que pertenecen a este estudiante
    std::vector<ObjetoPerdido*> misReportes = s.reportes.filtrar([&](const ObjetoPerdido& r) {
        return mismoCodigo(r.contactoCodigo(), codigoEstudiante);
    });

    if (misReportes.empty()) {
        std::cout << "\nNo tienes ningún reporte registrado en el sistema.\n";
        return;
    }

    std::cout << "Tienes " << misReportes.size() << " reporte(s) en total.\n";
    std::cout << "Buscar por:\n"
                 "  1. Todos mis reportes\n"
                 "  2. Código específico\n"
                 "  3. Nombre del objeto\n"
                 "  4. Categoría\n"
                 "  5. Estado del reporte\n";
    int opcion = util::leerEntero("Elige una opción: ", 1, 5);

    std::string filtroValor;
    cat::EstadoReporte estadoBuscado = cat::EstadoReporte::Pendiente;

    if (opcion == 2) {
        filtroValor = util::leerObligatorio("Escribe el código (ej. REP-001): ");
    } else if (opcion == 3) {
        filtroValor = util::leerObligatorio("Escribe el nombre (o parte del nombre): ");
    } else if (opcion == 4) {
        filtroValor = util::elegirDeLista("Selecciona la categoría:", cat::CATEGORIAS);
    } else if (opcion == 5) {
        std::cout << "Estados disponibles:\n"
                     "  1. pendiente\n"
                     "  2. recuperado\n"
                     "  3. cancelado\n";
        int estOp = util::leerEntero("Elige el estado: ", 1, 3);
        estadoBuscado = static_cast<cat::EstadoReporte>(estOp - 1);
    }

    // Filtramos sobre los reportes del estudiante según el criterio elegido
    std::vector<ObjetoPerdido*> resultados;
    for (ObjetoPerdido* r : misReportes) {
        bool cumple = false;
        switch (opcion) {
            case 1: cumple = true; break;
            case 2: cumple = mismoCodigo(r->codigo(), filtroValor); break;
            case 3: cumple = util::contiene(r->nombre(), filtroValor); break;
            case 4: cumple = util::normalizar(r->categoria()) == util::normalizar(filtroValor); break;
            case 5: cumple = (r->estado() == estadoBuscado); break;
        }
        if (cumple) resultados.push_back(r);
    }

    if (resultados.empty()) {
        std::cout << "\nNo se encontraron reportes con los criterios indicados.\n";
        return;
    }

    std::cout << "\nSe encontraron " << resultados.size() << " reporte(s):\n";
    for (const ObjetoPerdido* r : resultados) {
        std::cout << "  " << r->resumen() << "\n";
    }

    // Permitir ver la ficha completa detallada de un reporte si el usuario lo desea
    std::string codigoDetalle = util::leerLinea("\nEscribe un código para ver la ficha completa (Enter para volver): ");
    if (codigoDetalle.empty()) return;

    for (const ObjetoPerdido* r : resultados) {
        if (mismoCodigo(r->codigo(), codigoDetalle)) {
            std::cout << "\n--- Ficha Detallada del Reporte " << r->codigo() << " ---\n";
            r->mostrar();
            return;
        }
    }
    std::cout << "Ese código no está en la lista de resultados.\n";
}

// ============================================================ UC03 (IMPLEMENTACIÓN)
// FEAT03 y FEAT04: editar o cancelar un reporte PENDIENTE propio.
void uc03GestionarReporte(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC03] Gestionar reporte de objeto perdido ===\n";

    // Paso 1: Buscar reportes pendientes de este estudiante
    std::vector<ObjetoPerdido*> misPendientes = s.reportes.filtrar([&](const ObjetoPerdido& r) {
        return mismoCodigo(r.contactoCodigo(), codigoEstudiante) &&
               r.estado() == cat::EstadoReporte::Pendiente;
    });

    if (misPendientes.empty()) {
        std::cout << "\nNo tienes reportes pendientes para editar o cancelar.\n";
        return;
    }

    std::cout << "\nTus reportes pendientes:\n";
    for (const ObjetoPerdido* r : misPendientes) {
        std::cout << "  " << r->resumen() << "\n";
    }

    std::string codigo = util::leerObligatorio("\nEscribe el código del reporte que deseas gestionar: ");
    ObjetoPerdido* reporteEncontrado = nullptr;

    for (ObjetoPerdido* r : misPendientes) {
        if (mismoCodigo(r->codigo(), codigo)) {
            reporteEncontrado = r;
            break;
        }
    }

    if (!reporteEncontrado) {
        std::cout << "No se encontró un reporte pendiente con ese código que te pertenezca.\n";
        return;
    }

    std::cout << "\n¿Qué deseas hacer con el reporte " << reporteEncontrado->codigo() << "?\n"
                 "  1. Editar datos del reporte [FEAT03]\n"
                 "  2. Cancelar reporte [FEAT04]\n"
                 "  0. Volver\n";
    int opcion = util::leerEntero("Elige una opción: ", 0, 2);

    if (opcion == 0) {
        return;
    } else if (opcion == 1) {
        // FEAT03: Editar campos del reporte
        std::cout << "\n--- Editando reporte " << reporteEncontrado->codigo() << " ---\n";
        std::string nuevoNombre = util::leerObligatorio("Nuevo nombre (actual: " + reporteEncontrado->nombre() + "): ");
        std::string nuevaDesc = util::leerObligatorio("Nueva descripción: ");
        std::string nuevaCat = util::elegirDeLista("Nueva categoría:", cat::CATEGORIAS);
        Fecha nuevaFecha = Fecha::leer("Nueva fecha aproximada");
        std::string nuevoLugar = util::elegirDeLista("Nuevo lugar donde se perdió:", cat::LUGARES);
        std::string nuevoTel = util::leerObligatorio("Nuevo teléfono de contacto: ");
        std::string nuevaRes = util::leerObligatorio("Nueva característica reservada: ");

        reporteEncontrado->setNombre(nuevoNombre);
        reporteEncontrado->setDescripcion(nuevaDesc);
        reporteEncontrado->setCategoria(nuevaCat);
        reporteEncontrado->setFecha(nuevaFecha);
        reporteEncontrado->setLugar(nuevoLugar);
        reporteEncontrado->setContactoTelefono(nuevoTel);
        reporteEncontrado->setCaracteristicaReservada(nuevaRes);

        s.guardarTodo(); // Persistir cambios inmediatamente (SUPL01)
        std::cout << "\n¡Reporte actualizado y guardado con éxito!\n";

    } else if (opcion == 2) {
        // FEAT04: Cancelar reporte
        if (util::confirmar("¿Estás seguro de cancelar este reporte?")) {
            reporteEncontrado->setEstado(cat::EstadoReporte::Cancelado);
            s.guardarTodo(); // Persistir cambios (SUPL01)
            std::cout << "\nEl reporte ha sido marcado como «cancelado».\n";
        } else {
            std::cout << "Operación cancelada.\n";
        }
    }
}

// ============================================================ UC07 (IMPLEMENTACIÓN)
// FEAT11: sugerir los objetos disponibles que más se parezcan a un reporte pendiente.
void uc07SugerirCoincidencias(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC07] Sugerencias de objetos para tus reportes ===\n";

    // 1. Filtrar reportes pendientes propios
    std::vector<ObjetoPerdido*> misPendientes = s.reportes.filtrar([&](const ObjetoPerdido& r) {
        return mismoCodigo(r.contactoCodigo(), codigoEstudiante) &&
               r.estado() == cat::EstadoReporte::Pendiente;
    });

    if (misPendientes.empty()) {
        std::cout << "\nNo tienes reportes pendientes para buscarles coincidencias.\n";
        return;
    }

    std::cout << "\nTus reportes pendientes:\n";
    for (const ObjetoPerdido* r : misPendientes) {
        std::cout << "  " << r->resumen() << "\n";
    }

    std::string codigoRep = util::leerObligatorio("\nEscribe el código del reporte para buscar coincidencias: ");
    ObjetoPerdido* reporteSeleccionado = nullptr;
    for (ObjetoPerdido* r : misPendientes) {
        if (mismoCodigo(r->codigo(), codigoRep)) {
            reporteSeleccionado = r;
            break;
        }
    }

    if (!reporteSeleccionado) {
        std::cout << "No se encontró un reporte pendiente tuyo con ese código.\n";
        return;
    }

    // 2. Recorrer objetos encontrados que estén «disponibles» y calcular puntaje
    struct CoincidenciaItem {
        ObjetoEncontrado* objeto;
        double puntaje;
    };

    std::vector<CoincidenciaItem> listaCoincidencias;
    for (const ObjetoEncontrado& obj : s.objetos.todos()) {
    if (obj.estado() == cat::EstadoObjeto::Disponible) {
        double p = calcularPuntaje(*reporteSeleccionado, obj);
        if (p >= 30.0) {
            listaCoincidencias.push_back({const_cast<ObjetoEncontrado*>(&obj), p});
        }
    }
}

    if (listaCoincidencias.empty()) {
        std::cout << "\nNo se encontraron objetos disponibles con un puntaje de coincidencia suficiente (>= 30).\n";
        return;
    }

    // 3. Ordenar de mayor a menor puntaje
    std::sort(listaCoincidencias.begin(), listaCoincidencias.end(), [](const CoincidenciaItem& a, const CoincidenciaItem& b) {
        return a.puntaje > b.puntaje;
    });

    // 4. Mostrar de 3 a 5 objetos sugeridos
    std::cout << "\n¡Objetos encontrados que podrían ser el tuyo (ordenados por coincidencia)!\n";
    size_t mostrarMax = std::min(size_t(5), listaCoincidencias.size());
    
    for (size_t i = 0; i < mostrarMax; ++i) {
        std::cout << "  [" << (i + 1) << "] Puntaje: " << listaCoincidencias[i].puntaje << " pts\n"
                  << "      Resumen: " << listaCoincidencias[i].objeto->resumen() << "\n"
                  << "      Guardado en: " << listaCoincidencias[i].objeto->lugarGuardado() << "\n\n";
    }

    std::cout << "Nota: Si alguno de estos objetos coincide con lo que buscas, puedes proceder a solicitar su recuperación [UC09].\n";
}

// ============================================================ UC09 (IMPLEMENTACIÓN)
// FEAT13: registrar una solicitud de recuperación a partir de un reporte pendiente y un objeto disponible.
void uc09RegistrarSolicitud(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC09] Registrar solicitud de recuperación ===\n";

    // 1. Elegir un reporte pendiente PROPIO del estudiante
    std::vector<ObjetoPerdido*> misPendientes = s.reportes.filtrar([&](const ObjetoPerdido& r) {
        return mismoCodigo(r.contactoCodigo(), codigoEstudiante) &&
               r.estado() == cat::EstadoReporte::Pendiente;
    });

    if (misPendientes.empty()) {
        std::cout << "\nNo tienes reportes pendientes para asociar a una solicitud.\n";
        return;
    }

    std::cout << "\nTus reportes pendientes:\n";
    for (const ObjetoPerdido* r : misPendientes) {
        std::cout << "  " << r->resumen() << "\n";
    }

    std::string codigoRep = util::leerObligatorio("\nEscribe el código de tu reporte: ");
    ObjetoPerdido* reporteElegido = nullptr;
    for (ObjetoPerdido* r : misPendientes) {
        if (mismoCodigo(r->codigo(), codigoRep)) {
            reporteElegido = r;
            break;
        }
    }

    if (!reporteElegido) {
        std::cout << "No se encontró un reporte pendiente tuyo con ese código.\n";
        return;
    }

    // 2. Elegir un objeto encontrado que esté «disponible»
    std::vector<ObjetoEncontrado*> disponibles = s.objetos.filtrar([&](const ObjetoEncontrado& obj) {
        return obj.estado() == cat::EstadoObjeto::Disponible;
    });

    if (disponibles.empty()) {
        std::cout << "\nNo hay objetos disponibles en este momento en el sistema.\n";
        return;
    }

    std::cout << "\nObjetos encontrados disponibles:\n";
    for (const ObjetoEncontrado* obj : disponibles) {
        std::cout << "  " << obj->resumen() << " | Guardado en: " << obj->lugarGuardado() << "\n";
    }

    std::string codigoObj = util::leerObligatorio("\nEscribe el código del objeto encontrado que reclamas (ej. OBJ-001): ");
    ObjetoEncontrado* objetoElegido = nullptr;
    for (ObjetoEncontrado* obj : disponibles) {
        if (mismoCodigo(obj->codigo(), codigoObj)) {
            objetoElegido = obj;
            break;
        }
    }

    if (!objetoElegido) {
        std::cout << "El código ingresado no corresponde a ningún objeto disponible.\n";
        return;
    }

    // 3. Confirmar la solicitud
    std::cout << "\nVas a solicitar la recuperación del objeto [" << objetoElegido->codigo() 
              << " - " << objetoElegido->nombre() << "] para tu reporte [" << reporteElegido->codigo() << "].\n";
    
    if (!util::confirmar("¿Confirmar el envío de esta solicitud?")) {
        std::cout << "Operación cancelada.\n";
        return;
    }

    // 4. Crear la solicitud, cambiar el estado del objeto a «en proceso» y guardar
    std::string codigoSol = s.contadores.nuevoCodigoSolicitud();
    Solicitud nuevaSolicitud(codigoSol, reporteElegido->codigo(), objetoElegido->codigo(), Fecha::hoy());
    
    s.solicitudes.agregar(nuevaSolicitud);
    objetoElegido->setEstado(cat::EstadoObjeto::EnProceso);  // RNF06: Bloquea el objeto mientras se procesa

    s.guardarTodo(); // Persistir cambios en archivos (SUPL01)

    std::cout << "\n¡Solicitud registrada con éxito!\n"
              << "  Código de solicitud: " << codigoSol << "\n"
              << "  Estado: pendiente\n"
              << "  El objeto seleccionado ha pasado a estado «en proceso» a la espera de evaluación por el responsable.\n";
}

// ============================================================ UC10 (IMPLEMENTACIÓN)
// FEAT14: cancelar una solicitud pendiente propia, dejándola en «cancelada» y el objeto otra vez en «disponible».
void uc10CancelarSolicitud(Sistema& s, const std::string& codigoEstudiante) {
    std::cout << "\n=== [UC10] Cancelar solicitud de recuperación ===\n";

    // 1. Filtrar solicitudes pendientes del estudiante usando el método filtrar de la clase
    std::vector<Solicitud*> misSolicitudesPendientes = s.solicitudes.filtrar([&](const Solicitud& sol) {
        if (sol.estado() != cat::EstadoSolicitud::Pendiente) return false;
        ObjetoPerdido* rep = s.reportes.buscarPorCodigo(sol.codigoReporte());
        return rep && mismoCodigo(rep->contactoCodigo(), codigoEstudiante);
    });

    if (misSolicitudesPendientes.empty()) {
        std::cout << "\nNo tienes solicitudes pendientes para cancelar.\n";
        return;
    }

    std::cout << "\nTus solicitudes pendientes:\n";
    for (const Solicitud* sol : misSolicitudesPendientes) {
        std::cout << "  " << sol->resumen() << "\n";
    }

    std::string codigoSol = util::leerObligatorio("\nEscribe el código de la solicitud que deseas cancelar (ej. SOL-001): ");
    Solicitud* solicitudElegida = nullptr;
    for (Solicitud* sol : misSolicitudesPendientes) {
        if (mismoCodigo(sol->codigo(), codigoSol)) {
            solicitudElegida = sol;
            break;
        }
    }

    if (!solicitudElegida) {
        std::cout << "No se encontró una solicitud pendiente tuya con ese código.\n";
        return;
    }

    // 2. Confirmar cancelación
    if (!util::confirmar("¿Estás seguro de cancelar esta solicitud de recuperación?")) {
        std::cout << "Operación cancelada.\n";
        return;
    }

    // 3. Cambiar estado de la solicitud a «cancelada»
    solicitudElegida->setEstado(cat::EstadoSolicitud::Cancelada);

    // 4. Liberar el objeto asociado: vuelve a estado «disponible»
    ObjetoEncontrado* objetoAsociado = s.objetos.buscarPorCodigo(solicitudElegida->codigoObjeto());
    if (objetoAsociado) {
        objetoAsociado->setEstado(cat::EstadoObjeto::Disponible);
    }

    s.guardarTodo(); // Persistir cambios en archivos (SUPL01)

    std::cout << "\n¡Solicitud cancelada con éxito!\n"
              << "  - La solicitud [" << solicitudElegida->codigo() << "] ahora está «cancelada».\n"
              << "  - El objeto [" << solicitudElegida->codigoObjeto() << "] ha vuelto a estar «disponible».\n";
}

// ============================================================ UC06 (IMPLEMENTACIÓN)
// FEAT07 y FEAT08: editar, desactivar y depurar objetos encontrados.
void uc06GestionarObjetoEncontrado(Sistema& s) {
    std::cout << "\n=== [UC06] Gestionar objeto encontrado ===\n";

    // 1. Mostrar todos los objetos activos/no depurados para que el responsable elija
    const auto& todosLosObjetos = s.objetos.todos();
    if (todosLosObjetos.empty()) {
        std::cout << "\nNo hay objetos registrados en el sistema.\n";
        return;
    }

    std::cout << "\nListado de objetos registrados:\n";
    for (const ObjetoEncontrado& obj : todosLosObjetos) {
        std::cout << "  " << obj.resumen() << " | Guardado en: " << obj.lugarGuardado() << "\n";
    }

    std::string codigo = util::leerObligatorio("\nEscribe el código del objeto que deseas gestionar (ej. OBJ-001): ");
    ObjetoEncontrado* objetoEncontrado = s.objetos.buscarPorCodigo(codigo);

    if (!objetoEncontrado) {
        std::cout << "No se encontró ningún objeto con ese código.\n";
        return;
    }

    std::cout << "\nObjeto seleccionado:\n";
    objetoEncontrado->mostrar();

    std::cout << "\n¿Qué deseas hacer con este objeto?\n"
                 "  1. Editar sus datos [FEAT07]\n"
                 "  2. Desactivar objeto (registro por error) [FEAT08]\n"
                 "  3. Depurar (eliminar definitivamente un objeto ya desactivado) [FEAT08]\n"
                 "  0. Volver\n";
    int opcion = util::leerEntero("Elige una opción: ", 0, 3);

    if (opcion == 0) {
        return;
    } else if (opcion == 1) {
        // FEAT07: Editar datos del objeto encontrado
        std::cout << "\n--- Editando objeto " << objetoEncontrado->codigo() << " ---\n";
        std::string nuevoNombre = util::leerObligatorio("Nuevo nombre (actual: " + objetoEncontrado->nombre() + "): ");
        std::string nuevaDesc = util::leerObligatorio("Nueva descripción: ");
        std::string nuevaCat = util::elegirDeLista("Nueva categoría:", cat::CATEGORIAS);
        Fecha nuevaFecha = Fecha::leer("Nueva fecha del hallazgo");
        std::string nuevoLugar = util::elegirDeLista("Nuevo lugar del hallazgo:", cat::LUGARES);
        std::string nuevoGuardado = util::leerObligatorio("Nuevo lugar donde queda guardado: ");

        objetoEncontrado->setNombre(nuevoNombre);
        objetoEncontrado->setDescripcion(nuevaDesc);
        objetoEncontrado->setCategoria(nuevaCat);
        objetoEncontrado->setFecha(nuevaFecha);
        objetoEncontrado->setLugar(nuevoLugar);
        objetoEncontrado->setLugarGuardado(nuevoGuardado);

        s.guardarTodo(); // Persistir cambios inmediatamente (SUPL01)
        std::cout << "\n¡Objeto actualizado y guardado con éxito!\n";

    } else if (opcion == 2) {
        // FEAT08: Desactivar objeto
        if (objetoEncontrado->estado() == cat::EstadoObjeto::Desactivado) {
            std::cout << "El objeto ya se encuentra desactivado.\n";
            return;
        }
        if (util::confirmar("¿Estás seguro de desactivar este objeto por registro erróneo?")) {
            objetoEncontrado->setEstado(cat::EstadoObjeto::Desactivado);
            s.guardarTodo();
            std::cout << "\nEl objeto ha sido marcado como «desactivado».\n";
        } else {
            std::cout << "Operación cancelada.\n";
        }

    } else if (opcion == 3) {
        // FEAT08: Depuración definitiva (eliminar de la lista)
        if (objetoEncontrado->estado() != cat::EstadoObjeto::Desactivado) {
            std::cout << "\nError: Solo se pueden depurar (eliminar definitivamente) objetos que estén en estado «desactivado».\n";
            return;
        }
        if (util::confirmar("¿Estás seguro de ELIMINAR DEFINITIVAMENTE este registro de la base de datos?")) {
            if (s.objetos.depurar(codigo)) {
                s.guardarTodo();
                std::cout << "\nEl objeto ha sido depurado y eliminado definitivamente del sistema.\n";
            } else {
                std::cout << "\nError al intentar depurar el objeto.\n";
            }
        } else {
            std::cout << "Operación cancelada.\n";
        }
    }
}


// ============================================================ UC08 (IMPLEMENTACIÓN)
// FEAT12: buscar solicitudes de recuperación por código, código de estudiante/reporte o estado.
void uc08BuscarSolicitud(Sistema& s) {
    std::cout << "\n=== [UC08] Buscar solicitudes de recuperación ===\n";

    const auto& todasLasSolicitudes = s.solicitudes.todas();
    if (todasLasSolicitudes.empty()) {
        std::cout << "\nNo hay solicitudes registradas en el sistema.\n";
        return;
    }

    std::cout << "Hay " << todasLasSolicitudes.size() << " solicitud(es) en total.\n"
              << "Buscar por:\n"
              << "  1. Todas las solicitudes\n"
              << "  2. Código de solicitud específico (ej. SOL-001)\n"
              << "  3. Código de estudiante (contacto del reporte)\n"
              << "  4. Estado de la solicitud\n";
    int opcion = util::leerEntero("Elige una opción: ", 1, 4);

    std::string filtroValor;
    cat::EstadoSolicitud estadoBuscado = cat::EstadoSolicitud::Pendiente;

    if (opcion == 2) {
        filtroValor = util::leerObligatorio("Escribe el código de la solicitud: ");
    } else if (opcion == 3) {
        filtroValor = util::leerObligatorio("Escribe el código del estudiante: ");
    } else if (opcion == 4) {
        std::cout << "Estados disponibles:\n"
                     "  1. pendiente\n"
                     "  2. aprobada\n"
                     "  3. rechazada\n"
                     "  4. cancelada\n";
        int estOp = util::leerEntero("Elige el estado: ", 1, 4);
        estadoBuscado = static_cast<cat::EstadoSolicitud>(estOp - 1);
    }

    // Filtrar solicitudes según el criterio elegido
    std::vector<const Solicitud*> resultados;
    for (const Solicitud& sol : todasLasSolicitudes) {
        bool cumple = false;
        switch (opcion) {
            case 1: 
                cumple = true; 
                break;
            case 2: 
                cumple = mismoCodigo(sol.codigo(), filtroValor); 
                break;
            case 3: {
                ObjetoPerdido* rep = s.reportes.buscarPorCodigo(sol.codigoReporte());
                if (rep && mismoCodigo(rep->contactoCodigo(), filtroValor)) {
                    cumple = true;
                }
                break;
            }
            case 4: 
                cumple = (sol.estado() == estadoBuscado); 
                break;
        }
        if (cumple) {
            resultados.push_back(&sol);
        }
    }

    if (resultados.empty()) {
        std::cout << "\nNo se encontraron solicitudes con los criterios indicados.\n";
        return;
    }

    std::cout << "\nSe encontraron " << resultados.size() << " solicitud(es):\n";
    for (const Solicitud* sol : resultados) {
        std::cout << "  - " << sol->resumen() << "\n";
    }

    // Permitir ver detalles de una solicitud específica si se desea
    std::string codigoDetalle = util::leerLinea("\nEscribe el código de una solicitud para ver su detalle (Enter para volver): ");
    if (codigoDetalle.empty()) return;

    for (const Solicitud* sol : resultados) {
        if (mismoCodigo(sol->codigo(), codigoDetalle)) {
            std::cout << "\n--- Ficha Detallada de la Solicitud " << sol->codigo() << " ---\n";
            sol->mostrar();
            
            // Mostrar información complementaria del reporte y objeto asociados
            ObjetoPerdido* rep = s.reportes.buscarPorCodigo(sol->codigoReporte());
            ObjetoEncontrado* obj = s.objetos.buscarPorCodigo(sol->codigoObjeto());
            if (rep) {
                std::cout << "Reporte Asociado: " << rep->codigo() << " (" << rep->nombre() 
                          << " - Estudiante: " << rep->contactoCodigo() << ")\n";
            }
            if (obj) {
                std::cout << "Objeto Asociado: " << obj->codigo() << " (" << obj->nombre() 
                          << " - Guardado en: " << obj->lugarGuardado() << ")\n";
            }
            return;
        }
    }
    std::cout << "Ese código no está en la lista de resultados.\n";
}

// ============================================================ UC11 (IMPLEMENTACIÓN)
// FEAT15: evaluar (aprobar o rechazar) una solicitud pendiente de recuperación comparando la característica reservada.
void uc11EvaluarSolicitud(Sistema& s) {
    std::cout << "\n=== [UC11] Evaluar solicitud de recuperación ===\n";

    // 1. Filtrar solicitudes que estén en estado Pendiente
    std::vector<Solicitud*> solicitudesPendientes = s.solicitudes.filtrar([](const Solicitud& sol) {
        return sol.estado() == cat::EstadoSolicitud::Pendiente;
    });

    if (solicitudesPendientes.empty()) {
        std::cout << "\nNo hay solicitudes pendientes de evaluación en este momento.\n";
        return;
    }

    std::cout << "\nSolicitudes pendientes de revisión:\n";
    for (const Solicitud* sol : solicitudesPendientes) {
        ObjetoPerdido* rep = s.reportes.buscarPorCodigo(sol->codigoReporte());
        std::cout << "  - Solicitud: " << sol->codigo() 
                  << " | Reporte: " << sol->codigoReporte() 
                  << " | Objeto: " << sol->codigoObjeto();
        if (rep) {
            std::cout << " | Estudiante (Código): " << rep->contactoCodigo();
        }
        std::cout << "\n";
    }

    std::string codigoSol = util::leerObligatorio("\nEscribe el código de la solicitud que deseas evaluar (ej. SOL-001): ");
    Solicitud* solicitudElegida = nullptr;
    for (Solicitud* sol : solicitudesPendientes) {
        if (mismoCodigo(sol->codigo(), codigoSol)) {
            solicitudElegida = sol;
            break;
        }
    }

    if (!solicitudElegida) {
        std::cout << "No se encontró ninguna solicitud pendiente con ese código.\n";
        return;
    }

    // 2. Obtener el reporte y el objeto asociados para mostrar la característica reservada
    ObjetoPerdido* reporte = s.reportes.buscarPorCodigo(solicitudElegida->codigoReporte());
    ObjetoEncontrado* objeto = s.objetos.buscarPorCodigo(solicitudElegida->codigoObjeto());

    if (!reporte || !objeto) {
        std::cout << "\nError crítico: No se encontró el reporte o el objeto asociado a esta solicitud.\n";
        return;
    }

    std::cout << "\n--- Datos de Validación ---\n"
              << "Objeto reclamado: " << objeto->codigo() << " - " << objeto->nombre() << "\n"
              << "Reporte del estudiante: " << reporte->codigo() << " - " << reporte->nombre() << "\n"
              << "Estudiante (Código): " << reporte->contactoCodigo() << "\n"
              << "--------------------------------------------------\n"
              << "Característica reservada declarada por el estudiante:\n"
              << " > \"" << reporte->caracteristicaReservada() << "\"\n"
              << "--------------------------------------------------\n";

    std::cout << "¿Deseas aprobar o rechazar esta solicitud?\n"
                 "  1. Aprobar solicitud [FEAT15]\n"
                 "  2. Rechazar solicitud [FEAT15]\n"
                 "  0. Volver (dejar pendiente)\n";
    int decision = util::leerEntero("Elige una opción: ", 0, 2);

    if (decision == 0) {
        std::cout << "Operación pospuesta.\n";
        return;
    } else if (decision == 1) {
        // APROBAR
        solicitudElegida->setEstado(cat::EstadoSolicitud::Aprobada);
        objeto->setEstado(cat::EstadoObjeto::Devuelto); // El objeto pasa a devuelto/entregado con éxito
        s.guardarTodo();
        std::cout << "\n¡Solicitud APROBADA con éxito!\n"
                  << "  - La solicitud [" << solicitudElegida->codigo() << "] pasa a estado «aprobada».\n"
                  << "  - El objeto [" << objeto->codigo() << "] ha sido marcado como «devuelto» / entregado.\n";
    } else if (decision == 2) {
        // RECHAZAR
        solicitudElegida->setEstado(cat::EstadoSolicitud::Rechazada);
        objeto->setEstado(cat::EstadoObjeto::Disponible); // El objeto se libera de nuevo
        s.guardarTodo();
        std::cout << "\nLa solicitud ha sido RECHAZADA.\n"
                  << "  - La solicitud [" << solicitudElegida->codigo() << "] pasa a estado «rechazada».\n"
                  << "  - El objeto [" << objeto->codigo() << "] vuelve a estar «disponible» en el sistema.\n";
    }
}

// ============================================================ UC12 (IMPLEMENTACIÓN)
// FEAT16: registrar la entrega de un objeto con solicitud aprobada.
void uc12RegistrarEntrega(Sistema& s) {
    std::cout << "\n=== [UC12] Registrar entrega de objeto ===\n";

    // 1. Filtrar solicitudes que estén aprobadas
    std::vector<Solicitud*> aprobadas = s.solicitudes.filtrar([](const Solicitud& sol) {
        return sol.estado() == cat::EstadoSolicitud::Aprobada;
    });

    if (aprobadas.empty()) {
        std::cout << "\nNo hay solicitudes con estado «aprobada» listas para entrega.\n";
        return;
    }

    std::cout << "\nSolicitudes aprobadas disponibles:\n";
    for (const Solicitud* sol : aprobadas) {
        std::cout << "  - Solicitud: " << sol->codigo() 
                  << " | Objeto: " << sol->codigoObjeto() 
                  << " | Reporte: " << sol->codigoReporte() << "\n";
    }

    std::string codigoSol = util::leerObligatorio("\nEscribe el código de la solicitud a entregar (ej. SOL-001): ");
    Solicitud* solicitudElegida = nullptr;
    for (Solicitud* sol : aprobadas) {
        if (mismoCodigo(sol->codigo(), codigoSol)) {
            solicitudElegida = sol;
            break;
        }
    }

    if (!solicitudElegida) {
        std::cout << "No se encontró una solicitud aprobada con ese código.\n";
        return;
    }

    // 2. Pedir fecha de devolución y datos de quien recibe
    Fecha fechaDevolucion = Fecha::leer("Fecha de devolución/entrega");
    std::string nombreRecibe = util::leerObligatorio("Nombre de la persona que recibe el objeto: ");
    std::string codigoRecibe = util::leerObligatorio("Código universitario de quien recibe: ");

    // 3. Actualizar estados según las reglas del negocio
    solicitudElegida->setEstado(cat::EstadoSolicitud::Entregada);

    ObjetoEncontrado* objeto = s.objetos.buscarPorCodigo(solicitudElegida->codigoObjeto());
    if (objeto) {
        objeto->setEstado(cat::EstadoObjeto::Devuelto);
    }

    ObjetoPerdido* reporte = s.reportes.buscarPorCodigo(solicitudElegida->codigoReporte());
    if (reporte) {
        reporte->setEstado(cat::EstadoReporte::Recuperado);
    }

    s.guardarTodo(); // Persistir cambios inmediatamente (SUPL01)

    std::cout << "\n¡Entrega registrada con éxito!\n"
              << "  - Solicitud [" << solicitudElegida->codigo() << "] -> Entregada\n"
              << "  - Objeto [" << solicitudElegida->codigoObjeto() << "] -> Devuelto\n"
              << "  - Reporte [" << solicitudElegida->codigoReporte() << "] -> Recuperado\n"
              << "  - Recibió: " << nombreRecibe << " (" << codigoRecibe << ")\n";
}

// ============================================================ UC14 (IMPLEMENTACIÓN)
// FEAT19, FEAT20 y FEAT21: generar reportes básicos en archivos de texto en la carpeta reportes/.
void uc14GenerarReportes(Sistema& s) {
    std::cout << "\n=== [UC14] Generar reportes básicos del sistema ===\n";

    std::cout << "Define el rango de fechas para los reportes:\n";
    Fecha fechaInicio = Fecha::leer("Fecha inicial");
    Fecha fechaFin = Fecha::leer("Fecha final");

    // -------------------------------------------------------------
    // FEAT19: Reporte de objetos perdidos y encontrados en el rango
    // -------------------------------------------------------------
    std::string rutaFeat19 = "reportes_perdidos_encontrados.txt";
    std::ofstream archivo19(rutaFeat19);
    if (archivo19.is_open()) {
        archivo19 << "=== REPORTE DE OBJETOS PERDIDOS Y ENCONTRADOS ===\n\n";
        
        archivo19 << "[OBJETOS ENCONTRADOS]\n";
        for (const auto& obj : s.objetos.todos()) {
            archivo19 << " - " << obj.resumen() << " | Lugar: " << obj.lugarGuardado() << "\n";
        }

        archivo19 << "\n[REPORTES PERDIDOS]\n";
        for (const auto& rep : s.reportes.todos()) {
            archivo19 << " - " << rep.resumen() << " | Estudiante: " << rep.contactoCodigo() << "\n";
        }
        archivo19.close();
    }

    // -------------------------------------------------------------
    // FEAT20: Reporte de objetos devueltos en el rango
    // -------------------------------------------------------------
    std::string rutaFeat20 = "reportes_devueltos.txt";
    std::ofstream archivo20(rutaFeat20);
    if (archivo20.is_open()) {
        archivo20 << "=== REPORTE DE OBJETOS DEVUELTOS / ENTREGADOS ===\n\n";
        
        for (const auto& obj : s.objetos.todos()) {
            if (obj.estado() == cat::EstadoObjeto::Devuelto) {
                archivo20 << " - " << obj.resumen() << "\n";
            }
        }
        archivo20.close();
    }

    // -------------------------------------------------------------
    // FEAT21: Reporte de objetos sin reclamar (con días acumulados)
    // -------------------------------------------------------------
    std::string rutaFeat21 = "reportes_sin_reclamar.txt";
    std::ofstream archivo21(rutaFeat21);
    if (archivo21.is_open()) {
        archivo21 << "=== REPORTE DE OBJETOS SIN RECLAMAR ===\n\n";
        
        for (const auto& obj : s.objetos.todos()) {
            if (obj.estado() == cat::EstadoObjeto::Disponible) {
                int diasAcumulados = Fecha::diasEntre(obj.fecha(), Fecha::hoy());
                archivo21 << " - " << obj.resumen() 
                          << " | Días en custodia sin reclamar: " << diasAcumulados << " días\n";
            }
        }
        archivo21.close();
    }

    std::cout << "\n¡Reportes generados con éxito en la raíz del proyecto!\n"
              << "  1. " << rutaFeat19 << " [FEAT19]\n"
              << "  2. " << rutaFeat20 << " [FEAT20]\n"
              << "  3. " << rutaFeat21 << " [FEAT21]\n";
}
