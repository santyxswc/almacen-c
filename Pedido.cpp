#include "Pedido.hpp"
#include "Cliente.hpp"
#include "Factura.hpp"

Pedido::Pedido(int id, Cliente* cliente)
	: id_(id), cliente_(cliente) {}

int Pedido::getId() const {
	return id_;
}

Cliente* Pedido::getCliente() const {
	return cliente_;
}

void Pedido::addFactura(Factura* factura) {
	facturas_.push_back(factura);
}

float Pedido::getTotalFacturas() const {
	float total = 0.0;
	for (const auto& factura : facturas_) {
		total += factura->getTotal();
	}
	return total;
}

const std::vector<Factura*>& Pedido::getFacturas() const {
	return facturas_;
}
