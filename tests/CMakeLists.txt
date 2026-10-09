# @file tests/CMakeLists.txt
# @brief Pruebas del sistema de almacén.
# @author Santiago Caicedo

add_executable(almacen_pruebas
    main_pruebas.cpp
    PruebasDominio.cpp
    PruebasAplicacion.cpp
    PruebasConsola.cpp
    PruebasPersistencia.cpp
)
target_link_libraries(almacen_pruebas PRIVATE almacen_nucleo)
almacen_advertencias(almacen_pruebas)
target_include_directories(almacen_pruebas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME almacen_pruebas COMMAND almacen_pruebas)

# Pruebas de la ventana (solo si se compila la aplicación de escritorio).
if(TARGET almacen_escritorio)
    add_executable(almacen_pruebas_escritorio PruebasEscritorio.cpp)
    set_target_properties(almacen_pruebas_escritorio PROPERTIES AUTOMOC ON)
    target_link_libraries(almacen_pruebas_escritorio PRIVATE almacen_escritorio Qt6::Test)
    almacen_advertencias(almacen_pruebas_escritorio)
    add_test(NAME almacen_pruebas_escritorio COMMAND almacen_pruebas_escritorio)
    set_tests_properties(almacen_pruebas_escritorio PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
