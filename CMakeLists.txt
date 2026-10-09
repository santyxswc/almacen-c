# @file CMakeLists.txt
# @brief Compilación del sistema de almacén, sus pruebas y su documentación.
# @author Santiago Caicedo

cmake_minimum_required(VERSION 3.16)
project(Almacen VERSION 2.1.0 DESCRIPTION "Sistema de almacen con arquitectura limpia" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(ALMACEN_WARNINGS_AS_ERRORS "Tratar las advertencias como errores" OFF)
option(ALMACEN_BUILD_TESTS "Compilar las pruebas" ON)
option(ALMACEN_BUILD_GUI "Compilar la aplicacion de escritorio (requiere Qt 6)" ON)

# Núcleo: dominio, aplicación, infraestructura y presentación.
# El ejecutable y las pruebas enlazan la misma biblioteca.
add_library(almacen_nucleo STATIC
    src/dominio/Almacen.cpp
    src/dominio/Cliente.cpp
    src/dominio/Dinero.cpp
    src/dominio/Factura.cpp
    src/dominio/LineaFactura.cpp
    src/dominio/Pedido.cpp
    src/dominio/PoliticaDescuento.cpp
    src/dominio/Producto.cpp
    src/dominio/Validaciones.cpp
    src/aplicacion/Mapeo.cpp
    src/aplicacion/casos_uso/CasosUsoClientes.cpp
    src/aplicacion/casos_uso/CasosUsoGenerales.cpp
    src/aplicacion/casos_uso/CasosUsoInventario.cpp
    src/aplicacion/casos_uso/CasosUsoPedidos.cpp
    src/infraestructura/ArchivoDatos.cpp
    src/infraestructura/CatalogoInicial.cpp
    src/infraestructura/RepositoriosEnMemoria.cpp
    src/presentacion/Consola.cpp
    src/presentacion/MenuPrincipal.cpp
    src/presentacion/Vistas.cpp
    src/presentacion/comandos/ComandosClientes.cpp
    src/presentacion/comandos/ComandosInventario.cpp
    src/presentacion/comandos/ComandosPedidos.cpp
    src/Aplicacion.cpp
)
target_include_directories(almacen_nucleo PUBLIC src)

# Advertencias estrictas para todo el código del proyecto (no para las bibliotecas externas).
function(almacen_advertencias objetivo)
    if(MSVC)
        target_compile_options(${objetivo} PRIVATE /W4 /utf-8 /permissive-)
        if(ALMACEN_WARNINGS_AS_ERRORS)
            target_compile_options(${objetivo} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${objetivo} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
        if(ALMACEN_WARNINGS_AS_ERRORS)
            target_compile_options(${objetivo} PRIVATE -Werror)
        endif()
    endif()
endfunction()
almacen_advertencias(almacen_nucleo)
if(MSVC)
    # El código fuente está en UTF-8 (tildes y eñes en los textos).
    target_compile_options(almacen_nucleo PUBLIC /utf-8)
endif()

add_executable(almacen src/main.cpp)
target_link_libraries(almacen PRIVATE almacen_nucleo)
almacen_advertencias(almacen)


# Aplicación de escritorio (Qt 6). Si Qt no está instalado, se omite y el resto compila igual.
if(ALMACEN_BUILD_GUI)
    find_package(Qt6 6.4 QUIET COMPONENTS Widgets Test)
    if(Qt6_FOUND)
        set(CMAKE_AUTOMOC ON)
        add_library(almacen_escritorio STATIC
            src/presentacion/escritorio/Comun.cpp
            src/presentacion/escritorio/Pagina.hpp
            src/presentacion/escritorio/PaginaInicio.cpp
            src/presentacion/escritorio/PaginaVenta.cpp
            src/presentacion/escritorio/SelectorCantidad.cpp
            src/presentacion/escritorio/PaginaHistorial.cpp
            src/presentacion/escritorio/PaginaClientes.cpp
            src/presentacion/escritorio/PaginaProductos.cpp
            src/presentacion/escritorio/DialogoProducto.cpp
            src/presentacion/escritorio/PaginaAlmacenes.cpp
            src/presentacion/escritorio/VentanaPrincipal.cpp
            src/escritorio/AplicacionEscritorio.cpp
        )
        target_link_libraries(almacen_escritorio PUBLIC almacen_nucleo Qt6::Widgets)
        almacen_advertencias(almacen_escritorio)

        add_executable(sistema_almacen WIN32 MACOSX_BUNDLE src/escritorio/main_escritorio.cpp)
        target_link_libraries(sistema_almacen PRIVATE almacen_escritorio)
        set_target_properties(sistema_almacen PROPERTIES OUTPUT_NAME "SistemaAlmacen")
        almacen_advertencias(sistema_almacen)

        add_executable(capturas_escritorio EXCLUDE_FROM_ALL herramientas/capturas_escritorio.cpp)
        target_link_libraries(capturas_escritorio PRIVATE almacen_escritorio Qt6::Test)
        message(STATUS "Qt ${Qt6_VERSION} encontrado: se compila la aplicacion de escritorio")
    else()
        message(STATUS "Qt 6 no encontrado: se omite la aplicacion de escritorio")
    endif()
endif()

if(ALMACEN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentación con Doxygen (opcional): cmake --build build --target docs
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generando la documentacion en docs/api"
        VERBATIM)
endif()
