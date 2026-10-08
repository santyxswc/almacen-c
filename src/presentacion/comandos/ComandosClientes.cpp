/**
 * @file ComandosClientes.cpp
 * @brief Implementación de las opciones del menú para clientes.
 * @author Santiago Caicedo
 */

#include "presentacion/comandos/ComandosClientes.hpp"

#include <ostream>

#include "presentacion/Consola.hpp"
#include "presentacion/Vistas.hpp"

namespace almacen::presentacion {

void ComandoCrearCliente::ejecutar(Consola& consola) {
    const int id = consola.leerEntero("ID del cliente: ");
    const std::string nombre = consola.leerTexto("Nombre del cliente: ");
    const auto cliente = casoUso_.ejecutar(id, nombre);
    consola.salida() << "Cliente " << cliente.id << " (" << cliente.nombre << ") creado con exito.\n";
}

void ComandoListarClientes::ejecutar(Consola& consola) {
    mostrarClientes(consola.salida(), casoUso_.ejecutar());
}

}  // namespace almacen::presentacion
