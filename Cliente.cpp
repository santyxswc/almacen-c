#include "Cliente.hpp"
#include "Factura.hpp"
#include "Pedido.hpp"

Cliente::Cliente(int id, const std::string& nombre)
	: id_(id), nombre_(nombre) {}

int Cliente::getId() const {
	return id_;
}

const std::string& Cliente::getNombre() const {
	return nombre_;
}

void Cliente::addFactura(Factura* factura) {
	facturas_.push_back(factura);
}

const std::vector<Factura*>& Cliente::getFacturas() const {
	return facturas_;
}

ClienteConPedido::ClienteConPedido(int id, const std::string& nombre, Pedido* pedido)
	: Cliente(id, nombre), pedido_(pedido) {}

Pedido* ClienteConPedido::getPedido() const {
	return pedido_;
}
