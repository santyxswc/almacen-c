/**
 * @file CasosUsoPedidos.hpp
 * @brief Casos de uso de pedidos y facturación.
 * @author Santiago Caicedo
 */

#pragma once

#include <vector>

#include "aplicacion/Dtos.hpp"
#include "aplicacion/puertos/RepositorioAlmacenes.hpp"
#include "aplicacion/puertos/RepositorioClientes.hpp"
#include "aplicacion/puertos/RepositorioPedidos.hpp"

namespace almacen::aplicacion {

/**
 * @brief Abre un pedido nuevo para un cliente existente.
 */
class CrearPedido {
public:
    /**
     * @param clientes Repositorio de clientes, para validar que el cliente exista.
     * @param pedidos Repositorio donde se guarda el pedido.
     */
    CrearPedido(const RepositorioClientes& clientes, RepositorioPedidos& pedidos)
        : clientes_(clientes), pedidos_(pedidos) {}

    /**
     * @param pedidoId Id del pedido nuevo.
     * @param clienteId Cliente que hace el pedido.
     * @return El pedido creado (sin facturas).
     * @throws dominio::EntidadNoEncontrada si el cliente no existe.
     * @throws dominio::EntidadDuplicada si ya existe un pedido con ese id.
     */
    PedidoDto ejecutar(int pedidoId, int clienteId);

private:
    const RepositorioClientes& clientes_;
    RepositorioPedidos& pedidos_;
};

/**
 * @brief Emite una factura con productos del almacén y la agrega a un pedido.
 *
 * Si la solicitud no trae pedido, abre uno nuevo para el cliente indicado.
 * La operación es atómica: si algún producto o cantidad no es válido, no se
 * crea ni el pedido ni la factura.
 */
class CrearFactura {
public:
    /**
     * @param pedidos Repositorio de pedidos.
     * @param almacenes Repositorio de almacenes, para buscar los productos.
     */
    CrearFactura(RepositorioPedidos& pedidos, const RepositorioAlmacenes& almacenes)
        : pedidos_(pedidos), almacenes_(almacenes) {}

    /**
     * @param pedidos Repositorio de pedidos.
     * @param almacenes Repositorio de almacenes, para buscar los productos.
     * @param clientes Repositorio de clientes, para validar el cliente de un pedido nuevo.
     */
    CrearFactura(RepositorioPedidos& pedidos, const RepositorioAlmacenes& almacenes,
                 const RepositorioClientes& clientes)
        : pedidos_(pedidos), almacenes_(almacenes), clientes_(&clientes) {}

    /**
     * @param solicitud Id de la factura, pedido y productos con sus cantidades.
     * @return La factura creada con sus totales.
     * @throws dominio::EntidadNoEncontrada si el pedido, el cliente o algún producto no existe.
     * @throws dominio::EntidadDuplicada si el id de factura ya se usó.
     * @throws dominio::ValorInvalido si no hay productos o alguna cantidad no es positiva.
     */
    FacturaDto ejecutar(const SolicitudFactura& solicitud);

private:
    RepositorioPedidos& pedidos_;
    const RepositorioAlmacenes& almacenes_;
    const RepositorioClientes* clientes_ = nullptr;
};

/**
 * @brief Reúne los pedidos y facturas de un cliente con sus totales.
 */
class ConsultarResumenCliente {
public:
    /**
     * @param clientes Repositorio de clientes.
     * @param pedidos Repositorio de pedidos.
     */
    ConsultarResumenCliente(const RepositorioClientes& clientes, const RepositorioPedidos& pedidos)
        : clientes_(clientes), pedidos_(pedidos) {}

    /**
     * @param clienteId Cliente a consultar.
     * @return Pedidos, facturas y total general del cliente.
     * @throws dominio::EntidadNoEncontrada si el cliente no existe.
     */
    ResumenClienteDto ejecutar(int clienteId) const;

private:
    const RepositorioClientes& clientes_;
    const RepositorioPedidos& pedidos_;
};

/**
 * @brief Lista todos los pedidos con sus facturas (historial completo).
 */
class ListarPedidos {
public:
    /** @param pedidos Repositorio de pedidos. */
    explicit ListarPedidos(const RepositorioPedidos& pedidos) : pedidos_(pedidos) {}

    /** @return Los pedidos ordenados por id. */
    std::vector<PedidoDto> ejecutar() const;

private:
    const RepositorioPedidos& pedidos_;
};

}  // namespace almacen::aplicacion
