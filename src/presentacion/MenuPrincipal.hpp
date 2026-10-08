/**
 * @file MenuPrincipal.hpp
 * @brief Bucle del menú de consola.
 * @author Santiago Caicedo
 */

#pragma once

#include <memory>
#include <vector>

#include "presentacion/Consola.hpp"
#include "presentacion/comandos/Comando.hpp"

namespace almacen::presentacion {

/**
 * @brief Invocador del patrón Command: muestra las opciones y ejecuta la elegida.
 *
 * No conoce ningún caso de uso: solo recibe comandos. Los errores del
 * negocio y de entrada se muestran al usuario y el menú sigue
 * funcionando.
 */
class MenuPrincipal {
public:
    /** @param consola Consola para leer y escribir. */
    explicit MenuPrincipal(Consola& consola) : consola_(consola) {}

    /**
     * @brief Registra una opción. Se numera en el orden en que se agrega.
     * @param comando Comando a registrar.
     */
    void agregar(std::unique_ptr<Comando> comando);

    /** @brief Muestra el menú hasta que el usuario elige salir o se acaba la entrada. */
    void ejecutar();

private:
    void mostrarOpciones();

    Consola& consola_;
    std::vector<std::unique_ptr<Comando>> comandos_;
};

}  // namespace almacen::presentacion
