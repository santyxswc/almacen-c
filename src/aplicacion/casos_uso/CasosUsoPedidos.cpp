/**
 * @file CasosUsoPedidos.cpp
 * @brief Implementación de los casos de uso de pedidos y facturación.
 * @author Santiago Caicedo
 */

#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"

#include <optional>
#include <utility>

#include "aplicacion/Mapeo.hpp"
#include "dominio/Excepciones.hpp"

namespace almacen::aplicacion {

PedidoDto CrearPedido::ejecutar(int pedidoId, int clienteId) {
    if (!clientes_.buscar(clienteId)) {
        throw dominio::EntidadNoEncontrada("Cliente", clienteId);
    }
    if (pedidos_.buscar(pedidoId)) {
        throw dominio::EntidadDuplicada("pedido", pedidoId);
    }
    const dominio::Pedido pedido(pedidoId, clienteId);
    pedidos_.guardar(pedido);
    return aDto(pedido);
}

FacturaDto CrearFactura::ejecutar(const SolicitudFactura& solicitud) {
    std::optional<dominio::Pedido> pedido;
    if (solicitud.pedidoId != 0) {
        pedido = pedidos_.buscar(solicitud.pedidoId);
        if (!pedido) {
            throw dominio::EntidadNoEncontrada("Pedido", solicitud.pedidoId);
        }
    } else {
        if (clientes_ == nullptr || !clientes_->buscar(solicitud.clienteId)) {
            throw dominio::EntidadNoEncontrada("Cliente", solicitud.clienteId);
        }
        pedido.emplace(pedidos_.siguienteId(), solicitud.clienteId);
    }

    const int facturaId = solicitud.facturaId != 0 ? solicitud.facturaId : pedidos_.siguienteFacturaId();
    if (pedidos_.existeFactura(facturaId)) {
        throw dominio::EntidadDuplicada("factura", facturaId);
    }

    dominio::Factura factura(facturaId, pedido->clienteId());
    for (const auto& item : solicitud.items) {
        const auto producto = almacenes_.principal().buscarProducto(item.productoId);
        if (!producto) {
            throw dominio::EntidadNoEncontrada("Producto", item.productoId);
        }
        factura.agregarProducto(*producto, item.cantidad);
    }

    const FacturaDto dto = aDto(factura, pedido->id());
    pedido->agregarFactura(std::move(factura));
    pedidos_.guardar(*pedido);
    return dto;
}

ResumenClienteDto ConsultarResumenCliente::ejecutar(int clienteId) const {
    const auto cliente = clientes_.buscar(clienteId);
    if (!cliente) {
        throw dominio::EntidadNoEncontrada("Cliente", clienteId);
    }
    ResumenClienteDto resumen;
    resumen.cliente = aDto(*cliente);
    for (const auto& pedido : pedidos_.listarPorCliente(clienteId)) {
        resumen.pedidos.push_back(aDto(pedido));
        resumen.totalGeneral += pedido.total();
    }
    return resumen;
}

std::vector<PedidoDto> ListarPedidos::ejecutar() const {
    std::vector<PedidoDto> resultado;
    for (const auto& pedido : pedidos_.listar()) {
        resultado.push_back(aDto(pedido));
    }
    return resultado;
}

}  // namespace almacen::aplicacion
