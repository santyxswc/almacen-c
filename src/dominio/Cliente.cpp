/**
 * @file Cliente.cpp
 * @brief Implementación de Cliente.
 * @author Santiago Caicedo
 */

#include "dominio/Cliente.hpp"

#include <utility>

#include "dominio/Validaciones.hpp"

namespace almacen::dominio {

Cliente::Cliente(int id, std::string nombre)
    : id_(validarId(id, "cliente")), nombre_(validarNombre(std::move(nombre), "cliente")) {}

}  // namespace almacen::dominio
