/**
 * @file Vistas.cpp
 * @brief Implementación del formato de consola.
 * @author Santiago Caicedo
 */

#include "presentacion/Vistas.hpp"

#include <iomanip>
#include <ostream>

namespace almacen::presentacion {

void mostrarSeparador(std::ostream& salida) {
    salida << std::string(64, '-') << '\n';
}

void mostrarProductos(std::ostream& salida, const std::vector<aplicacion::ProductoDto>& productos) {
    if (productos.empty()) {
        salida << "  (sin productos)\n";
        return;
    }
    salida << std::left << std::setw(5) << "  ID" << std::setw(14) << "Producto" << std::setw(12) << "Precio"
           << std::setw(22) << "Descuento" << "Precio final\n";
    for (const auto& producto : productos) {
        salida << "  " << std::left << std::setw(3) << producto.id << std::setw(14) << producto.nombre << std::setw(12)
               << producto.precioBase.formatear() << std::setw(22) << producto.descuento
               << producto.precioFinal.formatear() << '\n';
    }
}

void mostrarClientes(std::ostream& salida, const std::vector<aplicacion::ClienteDto>& clientes) {
    if (clientes.empty()) {
        salida << "No hay clientes registrados.\n";
        return;
    }
    for (const auto& cliente : clientes) {
        salida << "  Cliente " << cliente.id << ": " << cliente.nombre << '\n';
    }
}

void mostrarFactura(std::ostream& salida, const aplicacion::FacturaDto& factura) {
    salida << "  Factura " << factura.id << " (pedido " << factura.pedidoId << ")\n";
    for (const auto& linea : factura.lineas) {
        salida << "    " << std::left << std::setw(14) << linea.producto << " x" << std::setw(4) << linea.cantidad
               << " a " << std::setw(10) << linea.precioUnitario.formatear() << " = " << linea.subtotal.formatear()
               << '\n';
    }
    if (!factura.ahorro.esCero()) {
        salida << "    Ahorro por descuentos: " << factura.ahorro.formatear() << '\n';
    }
    salida << "    Total factura: " << factura.total.formatear() << '\n';
}

void mostrarResumenCliente(std::ostream& salida, const aplicacion::ResumenClienteDto& resumen) {
    salida << "Cliente " << resumen.cliente.id << ": " << resumen.cliente.nombre << '\n';
    if (resumen.pedidos.empty()) {
        salida << "  No tiene pedidos.\n";
        return;
    }
    for (const auto& pedido : resumen.pedidos) {
        salida << "Pedido " << pedido.id << '\n';
        if (pedido.facturas.empty()) {
            salida << "  (sin facturas)\n";
        }
        for (const auto& factura : pedido.facturas) {
            mostrarFactura(salida, factura);
        }
        salida << "  Total del pedido: " << pedido.total.formatear() << '\n';
    }
    mostrarSeparador(salida);
    salida << "Total a pagar: " << resumen.totalGeneral.formatear() << '\n';
}

void mostrarAlmacen(std::ostream& salida, const aplicacion::AlmacenDto& almacen) {
    salida << "Almacen " << almacen.id << ": " << almacen.nombre << '\n';
    salida << "Productos distintos (incluye sub-almacenes): " << almacen.totalProductos << '\n';
    salida << "Productos guardados aqui:\n";
    mostrarProductos(salida, almacen.productos);
    salida << "Sub-almacenes:";
    if (almacen.subAlmacenes.empty()) {
        salida << " ninguno";
    }
    for (int id : almacen.subAlmacenes) {
        salida << ' ' << id;
    }
    salida << '\n';
}

}  // namespace almacen::presentacion
