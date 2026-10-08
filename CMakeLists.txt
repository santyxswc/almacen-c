# @file CMakeLists.txt
# @brief Compilación del sistema de almacén, sus pruebas y su documentación.
# @author Santiago Caicedo

cmake_minimum_required(VERSION 3.16)
project(Almacen VERSION 2.0.0 DESCRIPTION "Sistema de almacen con arquitectura limpia" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(ALMACEN_WARNINGS_AS_ERRORS "Tratar las advertencias como errores" OFF)
option(ALMACEN_BUILD_TESTS "Compilar las pruebas" ON)

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
    src/aplicacion/casos_uso/CasosUsoInventario.cpp
    src/aplicacion/casos_uso/CasosUsoPedidos.cpp
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

if(MSVC)
    target_compile_options(almacen_nucleo PUBLIC /W4 /utf-8 /permissive-)
    if(ALMACEN_WARNINGS_AS_ERRORS)
        target_compile_options(almacen_nucleo PUBLIC /WX)
    endif()
else()
    target_compile_options(almacen_nucleo PUBLIC -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    if(ALMACEN_WARNINGS_AS_ERRORS)
        target_compile_options(almacen_nucleo PUBLIC -Werror)
    endif()
endif()

add_executable(almacen src/main.cpp)
target_link_libraries(almacen PRIVATE almacen_nucleo)

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
