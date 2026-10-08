/**
 * @file RepositorioClientes.hpp
 * @brief Puerto de persistencia para los clientes.
 * @author Santiago Caicedo
 */

#pragma once

#include <optional>
#include <vector>

#include "dominio/Cliente.hpp"

namespace almacen::aplicacion {

/**
 * @brief Contrato para guardar y consultar clientes (patrón Repository).
 *
 * La capa de aplicación depende de esta abstracción, no de una base de
 * datos concreta (principio de inversión de dependencias). La
 * implementación vive en la capa de infraestructura.
 */
class RepositorioClientes {
public:
    virtual ~RepositorioClientes() = default;

    /**
     * @brief Guarda un cliente nuevo o reemplaza uno existente con el mismo id.
     * @param cliente Cliente a guardar.
     */
    virtual void guardar(const dominio::Cliente& cliente) = 0;

    /**
     * @brief Busca un cliente por id.
     * @param id Id del cliente.
     * @return El cliente, o vacío si no existe.
     */
    virtual std::optional<dominio::Cliente> buscar(int id) const = 0;

    /** @return Todos los clientes ordenados por id. */
    virtual std::vector<dominio::Cliente> listar() const = 0;
};

}  // namespace almacen::aplicacion
