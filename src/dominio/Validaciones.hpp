/**
 * @file Validaciones.hpp
 * @brief Reglas de validación compartidas por las entidades.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>

namespace almacen::dominio {

/**
 * @brief Verifica que un identificador sea positivo.
 * @param id Identificador a validar.
 * @param entidad Nombre de la entidad, para el mensaje de error.
 * @return El mismo id, para usarlo en listas de inicialización.
 * @throws ValorInvalido si el id es cero o negativo.
 */
int validarId(int id, const std::string& entidad);

/**
 * @brief Quita los espacios de los extremos y verifica que el nombre no quede vacío.
 * @param nombre Nombre a validar.
 * @param entidad Nombre de la entidad, para el mensaje de error.
 * @return El nombre sin espacios al inicio ni al final.
 * @throws ValorInvalido si el nombre queda vacío.
 */
std::string validarNombre(std::string nombre, const std::string& entidad);

}  // namespace almacen::dominio
