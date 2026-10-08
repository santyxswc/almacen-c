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

RepositorioAlmacenesEnMemoria::RepositorioAlmacenesEnMemoria(dominio::Almacen principal)
    : principal_(std::move(principal)) {}

}  // namespace almacen::infraestructura
