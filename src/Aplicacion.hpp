/**
 * @file Aplicacion.hpp
 * @brief Raíz de composición: crea y conecta todas las piezas.
 * @author Santiago Caicedo
 */

#pragma once

#include <iosfwd>

namespace almacen {

/**
 * @brief Arma la aplicación completa y ejecuta el menú.
 *
 * Es el único lugar que conoce las clases concretas de todas las capas
 * (repositorios en memoria, casos de uso y comandos) y las une por
 * inyección de dependencias. Recibe los flujos para poder probar el
 * programa completo con texto simulado.
 *
 * @param entrada Flujo de entrada (normalmente `std::cin`).
 * @param salida Flujo de salida (normalmente `std::cout`).
 */
void ejecutarAplicacion(std::istream& entrada, std::ostream& salida);

}  // namespace almacen
