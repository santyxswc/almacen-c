/**
 * @file MenuPrincipal.cpp
 * @brief Implementación del menú de consola.
 * @author Santiago Caicedo
 */

#include "presentacion/MenuPrincipal.hpp"

#include <ostream>
#include <utility>

#include "dominio/Excepciones.hpp"
#include "presentacion/Vistas.hpp"

namespace almacen::presentacion {

void MenuPrincipal::agregar(std::unique_ptr<Comando> comando) {
    comandos_.push_back(std::move(comando));
}

void MenuPrincipal::mostrarOpciones() {
    auto& salida = consola_.salida();
    salida << '\n';
    mostrarSeparador(salida);
    salida << "  MENU PRINCIPAL\n";
    mostrarSeparador(salida);
    for (std::size_t i = 0; i < comandos_.size(); ++i) {
        salida << "  " << (i + 1) << ". " << comandos_[i]->titulo() << '\n';
    }
    salida << "  0. Salir\n";
}

void MenuPrincipal::ejecutar() {
    auto& salida = consola_.salida();
    while (true) {
        try {
            mostrarOpciones();
            const int opcion = consola_.leerEntero("Seleccione una opcion: ");
            if (opcion == 0) {
                salida << "Hasta pronto.\n";
                return;
            }
            if (opcion < 0 || static_cast<std::size_t>(opcion) > comandos_.size()) {
                salida << "Opcion invalida.\n";
                continue;
            }
            mostrarSeparador(salida);
            comandos_[static_cast<std::size_t>(opcion) - 1]->ejecutar(consola_);
        } catch (const FinDeEntrada&) {
            salida << "\nHasta pronto.\n";
            return;
        } catch (const EntradaInvalida& error) {
            salida << "Entrada invalida: " << error.what() << '\n';
        } catch (const dominio::ErrorDominio& error) {
            salida << "Error: " << error.what() << '\n';
        }
    }
}

}  // namespace almacen::presentacion
