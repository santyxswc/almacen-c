/**
 * @file LineaFactura.cpp
 * @brief Implementación de LineaFactura.
 * @author Santiago Caicedo
 */

#include "dominio/LineaFactura.hpp"

#include <utility>

#include "dominio/Excepciones.hpp"
#include "dominio/Producto.hpp"

namespace almacen::dominio {

LineaFactura::LineaFactura(const Producto& producto, int cantidad)
    : productoId_(producto.id()),
      nombreProducto_(producto.nombre()),
      cantidad_(cantidad),
      precioBase_(producto.precioBase()),
      precioUnitario_(producto.precioFinal()) {
    if (cantidad <= 0) {
        throw ValorInvalido("La cantidad de " + producto.nombre() + " debe ser mayor que cero.");
    }
}

LineaFactura::LineaFactura(int productoId, std::string nombreProducto, int cantidad, Dinero precioBase,
                           Dinero precioUnitario)
    : productoId_(productoId),
      nombreProducto_(std::move(nombreProducto)),
      cantidad_(cantidad),
      precioBase_(precioBase),
      precioUnitario_(precioUnitario) {
    if (cantidad <= 0) {
        throw ValorInvalido("La cantidad de " + nombreProducto_ + " debe ser mayor que cero.");
    }
    if (precioUnitario > precioBase) {
        throw ValorInvalido("El precio con descuento no puede ser mayor que el precio de lista.");
    }
}

Dinero LineaFactura::subtotal() const {
    return precioUnitario_.multiplicar(cantidad_);
}

Dinero LineaFactura::ahorro() const {
    return precioBase_.multiplicar(cantidad_).restarHastaCero(subtotal());
}

}  // namespace almacen::dominio
