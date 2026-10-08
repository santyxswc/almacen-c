/**
 * @file RepositorioAlmacenes.hpp
 * @brief Puerto de acceso al árbol de almacenes.
 * @author Santiago Caicedo
 */

#pragma once

#include "dominio/Almacen.hpp"

namespace almacen::aplicacion {

/**
 * @brief Contrato para acceder al almacén principal y su árbol de sub-almacenes.
 */
class RepositorioAlmacenes {
public:
    virtual ~RepositorioAlmacenes() = default;

    /** @return El almacén raíz, del que cuelgan todos los sub-almacenes. */
    virtual dominio::Almacen& principal() = 0;

    /** @copydoc principal() */
    virtual const dominio::Almacen& principal() const = 0;
};

}  // namespace almacen::aplicacion
