/**
 * @file main.cpp
 * @brief Punto de entrada del sistema de almacén por consola.
 * @author Santiago Caicedo
 */

#include <exception>
#include <iostream>

#include "Aplicacion.hpp"

/**
 * @brief Inicia la aplicación con la entrada y salida estándar.
 * @return 0 si terminó normalmente, 1 si ocurrió un error inesperado.
 */
int main() {
    try {
        almacen::ejecutarAplicacion(std::cin, std::cout);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error inesperado: " << error.what() << '\n';
        return 1;
    }
}
