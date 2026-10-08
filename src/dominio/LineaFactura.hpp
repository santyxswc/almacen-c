/**
 * @file LineaFactura.hpp
 * @brief Renglón de una factura.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>

#include "dominio/Dinero.hpp"

namespace almacen::dominio {

class Producto;

/**
 * @brief Objeto de valor con un producto facturado y su cantidad.
 *
 * Guarda una "foto" del nombre y los precios del producto en el momento
 * de facturar: si después cambia el precio, la factura no se altera.
 */
class LineaFactura {
public:
    /**
     * @param producto Producto que se factura.
     * @param cantidad Unidades, mayor que cero.
     * @throws ValorInvalido si la cantidad no es positiva.
     */
    LineaFactura(const Producto& producto, int cantidad);

    /** @return Id del producto facturado. */
    int productoId() const noexcept { return productoId_; }

    /** @return Nombre del producto al momento de facturar. */
    const std::string& nombreProducto() const noexcept { return nombreProducto_; }

    /** @return Unidades facturadas. */
    int cantidad() const noexcept { return cantidad_; }

    /** @return Precio de lista de una unidad. */
    Dinero precioBase() const noexcept { return precioBase_; }

    /** @return Precio de una unidad con descuento. */
    Dinero precioUnitario() const noexcept { return precioUnitario_; }

    /** @return Precio unitario por cantidad. */
    Dinero subtotal() const;

    /** @return Dinero ahorrado por el descuento en esta línea. */
    Dinero ahorro() const;

private:
    int productoId_;
    std::string nombreProducto_;
    int cantidad_;
    Dinero precioBase_;
    Dinero precioUnitario_;
};

}  // namespace almacen::dominio
