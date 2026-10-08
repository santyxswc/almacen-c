/**
 * @file Pedido.hpp
 * @brief Pedido de un cliente, agrupador de facturas.
 * @author Santiago Caicedo
 */

#pragma once

#include <vector>

#include "dominio/Dinero.hpp"
#include "dominio/Factura.hpp"
#include "dominio/Identificable.hpp"

namespace almacen::dominio {

/**
 * @brief Raíz de agregado: un pedido es dueño de sus facturas.
 *
 * Todas las facturas se agregan a través del pedido, que garantiza las
 * reglas: deben ser del mismo cliente, no estar vacías y no repetir id.
 */
class Pedido final : public Identificable {
public:
    /**
     * @param id Identificador positivo.
     * @param clienteId Cliente que hace el pedido.
     * @throws ValorInvalido si algún id no es válido.
     */
    Pedido(int id, int clienteId);

    int id() const noexcept override { return id_; }

    /** @return Id del cliente dueño del pedido. */
    int clienteId() const noexcept { return clienteId_; }

    /**
     * @brief Agrega una factura al pedido.
     * @param factura Factura del mismo cliente y con al menos un producto.
     * @throws ValorInvalido si la factura está vacía o es de otro cliente.
     * @throws EntidadDuplicada si el pedido ya tiene una factura con ese id.
     */
    void agregarFactura(Factura factura);

    /** @return Las facturas del pedido. */
    const std::vector<Factura>& facturas() const noexcept { return facturas_; }

    /** @return true si alguna factura del pedido tiene ese id. */
    bool tieneFactura(int facturaId) const noexcept;

    /** @return Suma de los totales de todas las facturas. */
    Dinero total() const;

private:
    int id_;
    int clienteId_;
    std::vector<Factura> facturas_;
};

}  // namespace almacen::dominio
