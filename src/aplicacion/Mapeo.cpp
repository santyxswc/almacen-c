/**
 * @file Mapeo.cpp
 * @brief Implementación de la conversión de entidades a DTOs.
 * @author Santiago Caicedo
 */

#include "aplicacion/Mapeo.hpp"

namespace almacen::aplicacion {

ClienteDto aDto(const dominio::Cliente& cliente) {
    return {cliente.id(), cliente.nombre()};
}

ProductoDto aDto(const dominio::Producto& producto) {
    return {producto.id(), producto.nombre(), producto.precioBase(), producto.precioFinal(),
            producto.descuento().describir()};
}

FacturaDto aDto(const dominio::Factura& factura, int pedidoId) {
    FacturaDto dto;
    dto.id = factura.id();
    dto.pedidoId = pedidoId;
    for (const auto& linea : factura.lineas()) {
        dto.lineas.push_back({linea.nombreProducto(), linea.cantidad(), linea.precioUnitario(), linea.subtotal()});
    }
    dto.ahorro = factura.ahorroTotal();
    dto.total = factura.total();
    return dto;
}

PedidoDto aDto(const dominio::Pedido& pedido) {
    PedidoDto dto;
    dto.id = pedido.id();
    dto.clienteId = pedido.clienteId();
    for (const auto& factura : pedido.facturas()) {
        dto.facturas.push_back(aDto(factura, pedido.id()));
    }
    dto.total = pedido.total();
    return dto;
}

AlmacenDto aDto(const dominio::Almacen& almacen) {
    AlmacenDto dto;
    dto.id = almacen.id();
    dto.nombre = almacen.nombre();
    dto.totalProductos = almacen.totalProductos();
    for (const auto& producto : almacen.productos()) {
        dto.productos.push_back(aDto(*producto));
    }
    for (const auto* sub : almacen.subAlmacenes()) {
        dto.subAlmacenes.push_back(sub->id());
    }
    return dto;
}

}  // namespace almacen::aplicacion
