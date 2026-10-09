/**
 * @file Factura.cpp
 * @brief Implementación de Factura.
 * @author Santiago Caicedo
 */

#include "dominio/Factura.hpp"

#include <algorithm>

#include "dominio/Producto.hpp"
#include "dominio/Validaciones.hpp"

namespace almacen::dominio {

Factura::Factura(int id, int clienteId) : id_(validarId(id, "factura")), clienteId_(validarId(clienteId, "cliente")) {}

void Factura::agregarProducto(const Producto& producto, int cantidad) {
    auto existente = std::find_if(lineas_.begin(), lineas_.end(),
                                  [&](const LineaFactura& linea) { return linea.productoId() == producto.id(); });
    if (existente != lineas_.end()) {
        *existente = LineaFactura(producto, existente->cantidad() + cantidad);
        return;
    }
    lineas_.emplace_back(producto, cantidad);
}

void Factura::agregarLinea(const LineaFactura& linea) {
    auto existente = std::find_if(lineas_.begin(), lineas_.end(),
                                  [&](const LineaFactura& otra) { return otra.productoId() == linea.productoId(); });
    if (existente != lineas_.end()) {
        *existente = LineaFactura(linea.productoId(), linea.nombreProducto(), existente->cantidad() + linea.cantidad(),
                                  linea.precioBase(), linea.precioUnitario());
        return;
    }
    lineas_.push_back(linea);
}

Dinero Factura::total() const {
    Dinero suma;
    for (const auto& linea : lineas_) {
        suma += linea.subtotal();
    }
    return suma;
}

Dinero Factura::ahorroTotal() const {
    Dinero suma;
    for (const auto& linea : lineas_) {
        suma += linea.ahorro();
    }
    return suma;
}

}  // namespace almacen::dominio
