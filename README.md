# AquiTa — Sistema de objetos perdidos y encontrados (C++17, consola)

Esqueleto del proyecto basado en la matriz de requerimientos v2. Compila sin avisos y ya
guarda y carga datos. Cuatro casos de uso están completos como ejemplo del patrón; el resto
tiene un `TODO` con las reglas exactas de la matriz.

## Cómo compilar y ejecutar

**Linux / macOS / MinGW (g++):**

```
make          # compila y crea ./aquita
./aquita
```

Sin `make`: `g++ -std=c++17 src/*.cpp -o aquita`

**Visual Studio:** crea un proyecto de consola vacío, agrega todos los archivos de `src/`,
y en *Propiedades → C/C++ → Idioma* elige **C++17**; en *C/C++ → Línea de comandos* agrega
`/utf-8` (así se ven bien las tildes y la ñ).

El programa crea solas las carpetas `datos/` y `reportes/` junto al ejecutable. Ejecútalo
siempre desde la carpeta del proyecto para que encuentre `datos/`.

## Qué está hecho

| UC | Caso de uso | Estado |
|---|---|---|
| UC02 | Registrar objeto perdido | Listo |
| UC04 | Buscar objeto encontrado (ficha pública para el estudiante) | Listo |
| UC05 | Registrar objeto encontrado | Listo |
| UC13 | Listado general por estado | Listo |
| SUPL01 y SUPL02 | Guardar y cargar archivos | Listo |
| UC01, UC03, UC06, UC07 | Buscar reporte, gestionar reporte, gestionar objeto, coincidencias | TODO |
| UC08, UC09, UC10, UC11, UC12, UC14 | Solicitudes, evaluación, entrega, reportes | TODO |

Busca los pendientes con `grep -n "TODO" src/*.cpp src/*.h`. Cada uno dice qué estados
cambiar, qué validar y qué archivo generar.

## Estructura

| Archivo | Para qué sirve |
|---|---|
| `Objeto` (abstracta), `ObjetoPerdido`, `ObjetoEncontrado` | Herencia y polimorfismo (`mostrar`, `resumen`, `aLinea`) |
| `Solicitud` | Relaciona un reporte con un objeto encontrado |
| `GestorReportes`, `GestorObjetos` (`vector`), `GestorSolicitudes` (`list`) | CRUD de cada entidad, `cargar()` y `guardar()` |
| `Sistema` | Agrupa los gestores; `cargarTodo()` al iniciar y `guardarTodo()` tras cada cambio |
| `Contadores` | Genera REP-001, OBJ-001 y SOL-001 sin repetirlos |
| `Catalogos` | Categorías, lugares y estados (enums) |
| `Fecha` | Fechas DD/MM/AAAA: validar, comparar, restar |
| `Utilidades` | Lectura segura por consola, búsqueda sin tildes, archivos de texto |
| `CasosDeUso` | Una función por caso de uso |
| `Menu`, `main` | Menús por rol |

## Patrón para implementar un caso de uso

Mira `uc02`, `uc04` y `uc05` en `CasosDeUso.cpp`. Los pasos son siempre los mismos:

1. Pedir datos con `util::leerObligatorio`, `util::elegirDeLista` o `Fecha::leer`.
2. Buscar con `gestor.filtrar([&](const Tipo& x) { return ...; })`.
3. Cambiar estados con los setters de la clase.
4. Llamar a `s.guardarTodo()` después de **todo** cambio.

## Formato de los archivos (`datos/`)

Un registro por línea, campos separados por `|` (por eso el programa no deja escribir `|`).

- `reportes.txt`: `código|nombre|descripción|categoría|fecha|lugar|nombres|código universitario|teléfono|característica reservada|estado`
- `objetos.txt`: `código|nombre|descripción|categoría|fecha|lugar|lugar guardado|estado`
- `solicitudes.txt`: `código|reporte|objeto|fecha|estado|observación|fecha devolución|receptor|código receptor`
- `contadores.txt`: `último reporte|último objeto|última solicitud`

Si una línea está dañada, el programa avisa al iniciar, la omite y la elimina en el siguiente guardado.

## Antes de entregar

- Completar `cat::LUGARES` en `Catalogos.cpp` con los lugares reales del campus (hoy es provisional).
- Confirmar con el equipo qué muestra `ObjetoEncontrado::mostrarFicha()` (FEAT10).
- Si borras `datos/contadores.txt` pero conservas los otros archivos, los códigos se repetirán: bórralos todos juntos.
- Llevar una bitácora de decisiones (por qué `vector`, por qué `list`, por qué herencia): alimenta el informe.
