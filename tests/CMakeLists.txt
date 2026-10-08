# @file tests/CMakeLists.txt
# @brief Pruebas del sistema de almacén.
# @author Santiago Caicedo

add_executable(almacen_pruebas
    main_pruebas.cpp
    PruebasDominio.cpp
    PruebasAplicacion.cpp
    PruebasConsola.cpp
)
target_link_libraries(almacen_pruebas PRIVATE almacen_nucleo)
target_include_directories(almacen_pruebas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME almacen_pruebas COMMAND almacen_pruebas)
