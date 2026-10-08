/**
 * @file Identificable.hpp
 * @brief Interfaz común para las entidades que tienen un identificador.
 * @author Santiago Caicedo
 */

#pragma once

namespace almacen::dominio {

/**
 * @brief Contrato mínimo de toda entidad del dominio: tener un identificador.
 *
 * Es una interfaz pequeña a propósito (principio de segregación de
 * interfaces): solo exige lo que todas las entidades comparten.
 */
class Identificable {
public:
    virtual ~Identificable() = default;

    /**
     * @brief Identificador único de la entidad.
     * @return Número entero positivo.
     */
    virtual int id() const noexcept = 0;
};

}  // namespace almacen::dominio
