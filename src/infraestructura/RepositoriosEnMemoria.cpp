/**
 * @file RepositoriosEnMemoria.cpp
 * @brief Implementación de los repositorios en memoria.
 * @author Santiago Caicedo
 */

#include "infraestructura/RepositoriosEnMemoria.hpp"

#include <algorithm>
#include <utility>

namespace almacen::infraestructura {

void RepositorioClientesEnMemoria::guardar(const dominio::Cliente& cliente) {
    clientes_.insert_or_assign(cliente.id(), cliente);
}

std::optional<dominio::Cliente> RepositorioClientesEnMemoria::buscar(int id) const {
    const auto it = clientes_.find(id);
    if (it == clientes_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<dominio::Cliente> RepositorioClientesEnMemoria::listar() const {
    std::vector<dominio::Cliente> resultado;
    resultado.reserve(clientes_.size());
    for (const auto& [id, cliente] : clientes_) {
        resultado.push_back(cliente);
    }
    return resultado;
}

int RepositorioClientesEnMemoria::siguienteId() const {
    return clientes_.empty() ? 1 : clientes_.rbegin()->first + 1;
}

void RepositorioPedidosEnMemoria::guardar(const dominio::Pedido& pedido) {
    pedidos_.insert_or_assign(pedido.id(), pedido);
}

std::optional<dominio::Pedido> RepositorioPedidosEnMemoria::buscar(int id) const {
    const auto it = pedidos_.find(id);
    if (it == pedidos_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<dominio::Pedido> RepositorioPedidosEnMemoria::listarPorCliente(int clienteId) const {
    std::vector<dominio::Pedido> resultado;
    for (const auto& [id, pedido] : pedidos_) {
        if (pedido.clienteId() == clienteId) {
            resultado.push_back(pedido);
        }
    }
    return resultado;
}

bool RepositorioPedidosEnMemoria::existeFactura(int facturaId) const {
    return std::any_of(pedidos_.begin(), pedidos_.end(),
                       [facturaId](const auto& par) { return par.second.tieneFactura(facturaId); });
}

std::vector<dominio::Pedido> RepositorioPedidosEnMemoria::listar() const {
    std::vector<dominio::Pedido> resultado;
    resultado.reserve(pedidos_.size());
    for (const auto& [id, pedido] : pedidos_) {
        resultado.push_back(pedido);
    }
    return resultado;
}

int RepositorioPedidosEnMemoria::siguienteId() const {
    return pedidos_.empty() ? 1 : pedidos_.rbegin()->first + 1;
}

int RepositorioPedidosEnMemoria::siguienteFacturaId() const {
    int mayor = 0;
    for (const auto& [id, pedido] : pedidos_) {
        for (const auto& factura : pedido.facturas()) {
            mayor = std::max(mayor, factura.id());
        }
    }
    return mayor + 1;
}

RepositorioAlmacenesEnMemoria::RepositorioAlmacenesEnMemoria(dominio::Almacen principal)
    : principal_(std::move(principal)) {}

}  // namespace almacen::infraestructura
