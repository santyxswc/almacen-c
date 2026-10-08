/**
 * @file Comando.hpp
 * @brief Interfaz de una opción del menú.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>

namespace almacen::presentacion {

class Consola;

/**
 * @brief Patrón Command: cada opción del menú es un objeto.
 *
 * El `switch` gigante de la versión anterior se reemplaza por una lista
 * de comandos. Agregar una opción nueva es escribir una clase y
 * registrarla en `main.cpp`, sin modificar el menú (principio
 * abierto/cerrado).
 */
class Comando {
public:
    virtual ~Comando() = default;

    /** @return Texto de la opción en el menú. */
    virtual std::string titulo() const = 0;

    /**
     * @brief Pide los datos al usuario, ejecuta el caso de uso y muestra el resultado.
     * @param consola Consola para leer y escribir.
     */
    virtual void ejecutar(Consola& consola) = 0;
};

}  // namespace almacen::presentacion
