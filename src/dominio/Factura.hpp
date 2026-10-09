/**
 * @file Factura.hpp
 * @brief Factura emitida dentro de un pedido.
 * @author Santiago Caicedo
 */

#pragma once

#include <vector>

#include "dominio/Dinero.hpp"
#include "dominio/Identificable.hpp"
#include "dominio/LineaFactura.hpp"

namespace almacen::dominio {

class Producto;

/**
 * @brief Documento de cobro con varias líneas de productos.
 *
 * El total se calcula siempre a partir de las líneas (no se guarda un
 * acumulado que pueda quedar desactualizado) y usa el precio con
 * descuento de cada producto.
 */
class Factura final : public Identificable {
public:
    /**
     * @param id Identificador positivo.
     * @param clienteId Cliente al que se le factura.
     * @throws ValorInvalido si algún id no es válido.
     */
    Factura(int id, int clienteId);

    int id() const noexcept override { return id_; }

    /** @return Id del cliente facturado. */
    int clienteId() const noexcept { return clienteId_; }

    /**
     * @brief Agrega un producto. Si ya estaba en la factura, suma la cantidad.
     * @param producto Producto a facturar.
     * @param cantidad Unidades, mayor que cero.
     * @throws ValorInvalido si la cantidad no es positiva.
     */
    void agregarProducto(const Producto& producto, int cantidad);

    /**
     * @brief Agrega una línea ya armada (se usa al reconstruir una factura guardada).
     * @param linea Línea a agregar; si su producto ya estaba, se suman las cantidades.
     */
    void agregarLinea(const LineaFactura& linea);

    /** @return Las líneas de la factura en el orden en que se agregaron. */
    const std::vector<LineaFactura>& lineas() const noexcept { return lineas_; }

    /** @return true si la factura no tiene productos. */
    bool estaVacia() const noexcept { return lineas_.empty(); }

    /** @return Suma de los subtotales de todas las líneas. */
    Dinero total() const;

    /** @return Dinero total ahorrado por descuentos. */
    Dinero ahorroTotal() const;

private:
    int id_;
    int clienteId_;
    std::vector<LineaFactura> lineas_;
};

}  // namespace almacen::dominio
