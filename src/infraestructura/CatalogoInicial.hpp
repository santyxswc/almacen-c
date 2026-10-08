/**
 * @file CatalogoInicial.hpp
 * @brief Datos de ejemplo con los que arranca el programa.
 * @author Santiago Caicedo
 */

#pragma once

#include "dominio/Almacen.hpp"

namespace almacen::infraestructura {

/**
 * @brief Crea el almacén principal con los productos de ejemplo.
 *
 * Patrón Factory: concentra en un solo lugar la construcción de los
 * datos iniciales, que antes estaba mezclada con el menú en `main.cpp`.
 *
 * @return Almacén principal (id 1) con cuatro productos y sus descuentos.
 */
dominio::Almacen crearAlmacenConCatalogoInicial();

}  // namespace almacen::infraestructura
