/**
 * @file RepositorioPedidos.hpp
 * @brief Puerto de persistencia para los pedidos y sus facturas.
 * @author Santiago Caicedo
 */

#pragma once

#include <optional>
#include <vector>

#include "dominio/Pedido.hpp"

namespace almacen::aplicacion {

/**
 * @brief Contrato para guardar y consultar pedidos (patrón Repository).
 *
 * El pedido es la raíz de agregado: sus facturas se guardan con él.
 */
class RepositorioPedidos {
public:
    virtual ~RepositorioPedidos() = default;

    /**
     * @brief Guarda un pedido nuevo o reemplaza uno existente con el mismo id.
     * @param pedido Pedido a guardar, con todas sus facturas.
     */
    virtual void guardar(const dominio::Pedido& pedido) = 0;

    /**
     * @brief Busca un pedido por id.
     * @param id Id del pedido.
     * @return El pedido, o vacío si no existe.
     */
    virtual std::optional<dominio::Pedido> buscar(int id) const = 0;

    /**
     * @brief Pedidos de un cliente.
     * @param clienteId Id del cliente.
     * @return Los pedidos del cliente ordenados por id.
     */
    virtual std::vector<dominio::Pedido> listarPorCliente(int clienteId) const = 0;

    /**
     * @brief Indica si alguna factura de cualquier pedido tiene ese id.
     *
     * Los ids de factura son únicos en todo el sistema, no solo por pedido.
     */
    virtual bool existeFactura(int facturaId) const = 0;

    /** @return Todos los pedidos ordenados por id. */
    virtual std::vector<dominio::Pedido> listar() const = 0;

    /** @return Un id libre para un pedido nuevo. */
    virtual int siguienteId() const = 0;

    /** @return Un id libre para una factura nueva (único entre todos los pedidos). */
    virtual int siguienteFacturaId() const = 0;
};

}  // namespace almacen::aplicacion
