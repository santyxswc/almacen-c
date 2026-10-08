/**
 * @file LineaFactura.cpp
 * @brief Implementación de LineaFactura.
 * @author Santiago Caicedo
 */

#include "dominio/LineaFactura.hpp"

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

Dinero LineaFactura::subtotal() const {
    return precioUnitario_.multiplicar(cantidad_);
}

Dinero LineaFactura::ahorro() const {
    return precioBase_.multiplicar(cantidad_).restarHastaCero(subtotal());
}

}  // namespace almacen::dominio
