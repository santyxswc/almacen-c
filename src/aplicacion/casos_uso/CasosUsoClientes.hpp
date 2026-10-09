/**
 * @file CasosUsoClientes.hpp
 * @brief Casos de uso del módulo de clientes.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>
#include <vector>

#include "aplicacion/Dtos.hpp"
#include "aplicacion/puertos/RepositorioClientes.hpp"

namespace almacen::aplicacion {

/**
 * @brief Registra un cliente nuevo.
 *
 * Cada caso de uso es una clase con un único método `ejecutar`
 * (principio de responsabilidad única) y recibe sus dependencias por el
 * constructor (inyección de dependencias).
 */
class CrearCliente {
public:
    /** @param clientes Repositorio donde se guardan los clientes. */
    explicit CrearCliente(RepositorioClientes& clientes) : clientes_(clientes) {}

    /**
     * @param id Id del cliente nuevo.
     * @param nombre Nombre del cliente.
     * @return El cliente creado.
     * @throws dominio::EntidadDuplicada si ya existe un cliente con ese id.
     * @throws dominio::ValorInvalido si algún dato no es válido.
     */
    ClienteDto ejecutar(int id, const std::string& nombre);

    /**
     * @brief Registra un cliente con el siguiente id libre.
     * @param nombre Nombre del cliente.
     * @return El cliente creado, con su id asignado.
     * @throws dominio::ValorInvalido si el nombre está vacío.
     */
    ClienteDto ejecutar(const std::string& nombre);

private:
    RepositorioClientes& clientes_;
};

/**
 * @brief Lista todos los clientes registrados.
 */
class ListarClientes {
public:
    /** @param clientes Repositorio de clientes. */
    explicit ListarClientes(const RepositorioClientes& clientes) : clientes_(clientes) {}

    /** @return Los clientes ordenados por id. */
    std::vector<ClienteDto> ejecutar() const;

private:
    const RepositorioClientes& clientes_;
};

}  // namespace almacen::aplicacion
