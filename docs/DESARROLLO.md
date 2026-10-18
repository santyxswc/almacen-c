# Guía de desarrollo

Todo lo necesario para hacer cambios sin romper nada: cómo preparar el entorno, las reglas del
proyecto, dónde va cada tipo de cambio y cómo publicar una versión nueva.

## Preparar el entorno

| Herramienta | Versión | Para qué |
| --- | --- | --- |
| Compilador C++17 | GCC 9+, Clang 10+ o Visual Studio 2019+ | Compilar |
| CMake | 3.16+ | Configurar el proyecto |
| Qt | 6.4+ (módulos Widgets y Test) | Aplicación de escritorio (opcional) |
| clang-format | 18 | Formato del código |
| Doxygen y Graphviz | 1.9+ | Documentación de la API |

En Ubuntu:

```bash
sudo apt install build-essential cmake qt6-base-dev clang-format doxygen graphviz
```

En Windows, instale Visual Studio (con "Desarrollo para el escritorio con C++"), CMake y Qt 6 desde el
instalador oficial de Qt, y configure con `-DCMAKE_PREFIX_PATH=C:/Qt/6.8.0/msvc2022_64`.

## Ciclo de trabajo

```bash
cmake -S . -B build -DALMACEN_WARNINGS_AS_ERRORS=ON   # una sola vez
cmake --build build                                   # compilar
ctest --test-dir build --output-on-failure            # 40 pruebas (consola y ventana)
./build/SistemaAlmacen                                 # probar la ventana a mano
find src tests herramientas -name '*.?pp' | xargs clang-format -i   # formato
doxygen Doxyfile                                       # documentación: no debe mostrar avisos
```

Antes de cada commit deben pasar las tres revisiones que hace la CI: **compila sin advertencias**,
**pasan las pruebas** y **el formato es correcto**. Doxygen tampoco debe mostrar avisos.

Opciones de CMake:

| Opción | Por defecto | Efecto |
| --- | --- | --- |
| `ALMACEN_BUILD_GUI` | `ON` | Compila la aplicación de escritorio si encuentra Qt 6 |
| `ALMACEN_BUILD_TESTS` | `ON` | Compila las pruebas |
| `ALMACEN_WARNINGS_AS_ERRORS` | `OFF` | Trata las advertencias como errores (la CI lo activa) |

## Reglas del proyecto

1. **Las dependencias apuntan hacia adentro.** `dominio/` no incluye nada de las otras capas;
   `aplicacion/` solo incluye `dominio/`; Qt solo aparece en `presentacion/escritorio/` y `escritorio/`.
   Si un cambio necesita romper esta regla, el diseño está mal: hace falta un puerto nuevo.
2. **Las reglas del negocio van en el dominio**, no en las pantallas. La ventana y la consola solo leen
   datos, llaman casos de uso y muestran el resultado.
3. **El dinero es siempre `dominio::Dinero`**, nunca `float` ni `double`.
4. **Un caso de uso por clase**, con un único método `ejecutar` y sus dependencias por el constructor.
5. **Cada pantalla recibe solo los casos de uso que usa.**
6. **Nada de `new`/`delete` fuera de la interfaz gráfica.** En Qt, todo widget creado con `new` debe
   tener un padre que lo libere.
7. **Los errores del negocio son excepciones de `ErrorDominio`** con mensajes listos para el usuario.
8. **Todo cambio de comportamiento lleva su prueba.**

## Documentación con Doxygen

Cada archivo empieza con este encabezado (también los `.cpp` y las pruebas):

```cpp
/**
 * @file NombreDelArchivo.hpp
 * @brief Qué contiene el archivo, en una línea.
 * @author Santiago Caicedo
 */
```

Toda clase, función y parámetro público se documenta en el `.hpp`:

```cpp
/**
 * @brief Qué hace, en una línea.
 *
 * Detalles opcionales: por qué existe, qué patrón aplica, qué no hace.
 *
 * @param productoId Producto a buscar.
 * @return El producto, o nullptr si no existe.
 * @throws ValorInvalido si el id no es positivo.
 */
std::shared_ptr<const Producto> buscarProducto(int productoId) const;
```

`Doxyfile` está configurado para avisar de cualquier cosa sin documentar y la CI falla si aparece un
aviso. La documentación generada queda en `docs/api/html/index.html`, con diagramas de clases.

## Dónde va cada cambio

| Quiero... | Archivos | Pasos |
| --- | --- | --- |
| **Un tipo de descuento nuevo** (por ejemplo "2x1") | `dominio/PoliticaDescuento.*` | Clase nueva que herede de `PoliticaDescuento`. Para crearla desde la ventana: un valor nuevo en `TipoDescuento` (`aplicacion/Dtos.hpp`), su caso en `crearDescuento` (`CasosUsoInventario.cpp`) y una opción en `DialogoProducto`. Para guardarla: ver "un dato nuevo en el archivo". |
| **Una regla del negocio nueva** (por ejemplo, un máximo de unidades por factura) | `dominio/` | Validarla en la entidad dueña del dato (`Factura`, `Pedido`...) y lanzar `ValorInvalido`. Prueba en `tests/PruebasDominio.cpp`. La ventana mostrará el mensaje sola. |
| **Un caso de uso nuevo** | `aplicacion/casos_uso/` | Clase con `ejecutar`, que reciba los repositorios por el constructor. Si necesita datos nuevos, agregarlos a `Dtos.hpp` y `Mapeo.*`. Prueba en `tests/PruebasAplicacion.cpp`. Crearlo en `AplicacionEscritorio.hpp` (y en `Aplicacion.cpp` si lo usa la consola). |
| **Una pantalla nueva en la ventana** | `presentacion/escritorio/` | Clase que herede de `Pagina` e implemente `refrescar()`; emitir `datosCambiados()` cuando cambie algo. Registrarla con `agregarPagina(...)` en `AplicacionEscritorio.cpp` y agregar su valor a `IndicePagina` (`Pagina.hpp`). Agregar el `.cpp` a `CMakeLists.txt`. |
| **Cambiar colores, letras o espaciado** | `presentacion/escritorio/Comun.cpp` | Todo el estilo está en `hojaDeEstilos()`. Los widgets se identifican por `setObjectName` (`principal`, `tarjeta`, `titulo`...). |
| **Una opción nueva en el menú de consola** | `presentacion/comandos/` | Clase que herede de `Comando` y una línea `menu.agregar(...)` en `Aplicacion.cpp`. |
| **Un dato nuevo en el archivo** | `infraestructura/ArchivoDatos.cpp` | Escribirlo en `guardar` y leerlo en `cargar`. Si cambia el formato de una línea existente, **subir `kVersion`** y seguir leyendo la versión anterior, para que los archivos de los usuarios no queden inservibles. Agregar el caso a `tests/PruebasPersistencia.cpp`. |
| **Guardar en SQLite u otra base de datos** | `infraestructura/` | Implementar `RepositorioClientes`, `RepositorioPedidos` y `RepositorioAlmacenes` y usarlos en `AplicacionEscritorio.cpp`. Los casos de uso no cambian. |
| **Actualizar las capturas del README** | `herramientas/capturas_escritorio.cpp` | `cmake --build build --target capturas_escritorio` y luego `QT_QPA_PLATFORM=offscreen ./build/capturas_escritorio docs/capturas`. Los datos de ejemplo están en `cargarEjemplo()`. |

## Pruebas

| Archivo | Qué prueba | Cómo |
| --- | --- | --- |
| `tests/PruebasDominio.cpp` | Reglas puras: dinero, descuentos, facturas, árbol de almacenes | Marco propio `MiniTest.hpp` (`PRUEBA`, `VERIFICAR`, `VERIFICAR_IGUAL`, `VERIFICAR_LANZA`) |
| `tests/PruebasAplicacion.cpp` | Casos de uso con repositorios en memoria | MiniTest |
| `tests/PruebasPersistencia.cpp` | Guardar, cargar y archivos dañados | MiniTest, en la carpeta temporal |
| `tests/PruebasConsola.cpp` | El menú completo con entrada simulada | MiniTest con `std::istringstream` |
| `tests/PruebasEscritorio.cpp` | La ventana con clics y teclas simulados | Qt Test, sin pantalla (`QT_QPA_PLATFORM=offscreen`) |

Para una prueba nueva basta con escribir `PRUEBA(nombre_descriptivo) { ... }` en el archivo que
corresponda: se registra sola.

## Integración continua

`.github/workflows/ci.yml` corre en cada push:

- compila y prueba en **Linux, Windows y macOS** con la ventana incluida;
- ejecuta las pruebas con **AddressSanitizer y UBSan** (fugas de memoria y comportamiento indefinido);
- revisa el **formato** con clang-format;
- genera la **documentación** con Doxygen y falla si hay avisos.

## Publicar una versión

1. Actualizar el número en `CMakeLists.txt` (`project(... VERSION x.y.z)`) y en `Doxyfile`
   (`PROJECT_NUMBER`).
2. Anotar los cambios en `CHANGELOG.md`.
3. Hacer commit y crear la etiqueta: `git tag vx.y.z && git push origin vx.y.z`.
4. El flujo **Versión para Windows** compila, prueba, empaqueta `SistemaAlmacen-windows.zip` con
   las bibliotecas de Qt y publica el *Release* en GitHub.

Sin etiqueta, el mismo flujo se puede ejecutar a mano (pestaña **Actions → Versión para Windows → Run
workflow**) y deja el `.zip` como archivo descargable de esa ejecución.

## Convenciones

- Código, comentarios, mensajes y commits en **español**.
- Clases en `PascalCase`, funciones y variables en `camelCase`, miembros privados terminan en `_`.
- Un archivo `.hpp`/`.cpp` por clase importante; las clases pequeñas relacionadas pueden compartir archivo.
- Mensajes de commit en presente y que digan qué cambia para el usuario: "Agrega...", "Corrige...".
