/**
 * @file ComandosPedidos.cpp
 * @brief Implementación de las opciones del menú para pedidos y facturas.
 * @author Santiago Caicedo
 */

#include "presentacion/comandos/ComandosPedidos.hpp"

#include <ostream>

#include "presentacion/Consola.hpp"
#include "presentacion/Vistas.hpp"

namespace almacen::presentacion {

void ComandoCrearPedido::ejecutar(Consola& consola) {
    const int pedidoId = consola.leerEntero("ID del pedido: ");
    const int clienteId = consola.leerEntero("ID del cliente: ");
    const auto pedido = casoUso_.ejecutar(pedidoId, clienteId);
    consola.salida() << "Pedido " << pedido.id << " creado con exito.\n";
}

void ComandoCrearFactura::ejecutar(Consola& consola) {
    aplicacion::SolicitudFactura solicitud;
    solicitud.facturaId = consola.leerEntero("ID de la factura: ");
    solicitud.pedidoId = consola.leerEntero("ID del pedido: ");

    consola.salida() << "Productos disponibles:\n";
    mostrarProductos(consola.salida(), listarProductos_.ejecutar());
    consola.salida() << "Escriba el ID de cada producto y su cantidad. Escriba 0 para terminar.\n";

    while (true) {
        const int productoId = consola.leerEntero("ID del producto (0 para terminar): ");
        if (productoId == 0) {
            break;
        }
        const int cantidad = consola.leerEntero("Cantidad: ");
        solicitud.items.push_back({productoId, cantidad});
    }

    const auto factura = crearFactura_.ejecutar(solicitud);
    consola.salida() << "Factura creada con exito:\n";
    mostrarFactura(consola.salida(), factura);
}

void ComandoResumenCliente::ejecutar(Consola& consola) {
    const int clienteId = consola.leerEntero("ID del cliente: ");
    mostrarResumenCliente(consola.salida(), casoUso_.ejecutar(clienteId));
}

}  // namespace almacen::presentacion
