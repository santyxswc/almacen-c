/**
 * @file Excepciones.hpp
 * @brief Errores que lanzan las reglas del negocio.
 * @author Santiago Caicedo
 */

#pragma once

#include <stdexcept>
#include <string>

namespace almacen::dominio {

/**
 * @brief Base de todos los errores del dominio.
 *
 * Las capas externas pueden atrapar este tipo para distinguir un error
 * del negocio (mostrable al usuario) de un fallo técnico.
 */
class ErrorDominio : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * @brief Un dato no cumple las reglas (precio negativo, nombre vacío...).
 */
class ValorInvalido : public ErrorDominio {
public:
    using ErrorDominio::ErrorDominio;
};

/**
 * @brief Se buscó una entidad que no existe.
 */
class EntidadNoEncontrada : public ErrorDominio {
public:
    /**
     * @brief Construye el mensaje a partir del tipo de entidad y su id.
     * @param entidad Nombre de la entidad (por ejemplo, "Cliente").
     * @param id Identificador que no se encontró.
     */
    EntidadNoEncontrada(const std::string& entidad, int id)
        : ErrorDominio(entidad + " con ID " + std::to_string(id) + " no existe.") {}
};

/**
 * @brief Se intentó registrar una entidad con un id que ya está en uso.
 */
class EntidadDuplicada : public ErrorDominio {
public:
    /**
     * @brief Construye el mensaje a partir del tipo de entidad y su id.
     * @param entidad Nombre de la entidad (por ejemplo, "Pedido").
     * @param id Identificador repetido.
     */
    EntidadDuplicada(const std::string& entidad, int id)
        : ErrorDominio("Ya existe un(a) " + entidad + " con ID " + std::to_string(id) + ".") {}
};

}  // namespace almacen::dominio
