/**
 * @file Pedido.cpp
 * @brief Implementación de Pedido.
 * @author Santiago Caicedo
 */

#include "dominio/Pedido.hpp"

#include <algorithm>
#include <utility>

#include "dominio/Excepciones.hpp"
#include "dominio/Validaciones.hpp"

namespace almacen::dominio {

Pedido::Pedido(int id, int clienteId) : id_(validarId(id, "pedido")), clienteId_(validarId(clienteId, "cliente")) {}

void Pedido::agregarFactura(Factura factura) {
    if (factura.clienteId() != clienteId_) {
        throw ValorInvalido("La factura debe ser del mismo cliente del pedido.");
    }
    if (factura.estaVacia()) {
        throw ValorInvalido("La factura debe tener al menos un producto.");
    }
    if (tieneFactura(factura.id())) {
        throw EntidadDuplicada("factura", factura.id());
    }
    facturas_.push_back(std::move(factura));
}

bool Pedido::tieneFactura(int facturaId) const noexcept {
    return std::any_of(facturas_.begin(), facturas_.end(),
                       [facturaId](const Factura& factura) { return factura.id() == facturaId; });
}

Dinero Pedido::total() const {
    Dinero suma;
    for (const auto& factura : facturas_) {
        suma += factura.total();
    }
    return suma;
}

}  // namespace almacen::dominio
