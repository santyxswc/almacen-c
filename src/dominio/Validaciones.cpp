/**
 * @file Validaciones.cpp
 * @brief Implementación de las reglas de validación compartidas.
 * @author Santiago Caicedo
 */

#include "dominio/Validaciones.hpp"

#include "dominio/Excepciones.hpp"

namespace almacen::dominio {

int validarId(int id, const std::string& entidad) {
    if (id <= 0) {
        throw ValorInvalido("El ID del " + entidad + " debe ser un numero positivo.");
    }
    return id;
}

std::string validarNombre(std::string nombre, const std::string& entidad) {
    const auto inicio = nombre.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        throw ValorInvalido("El nombre del " + entidad + " no puede estar vacio.");
    }
    const auto fin = nombre.find_last_not_of(" \t\r\n");
    return nombre.substr(inicio, fin - inicio + 1);
}

}  // namespace almacen::dominio
