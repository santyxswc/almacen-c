#include "Factura.hpp"
#include "Cliente.hpp"
#include "Producto.hpp"

Factura::Factura(int id, Cliente* cliente)
	: id_(id), cliente_(cliente), total_(0.0) {}

int Factura::getId() const {
	return id_;
}

Cliente* Factura::getCliente() const {
	return cliente_;
}

void Factura::addProducto(Producto* producto, int cantidad) {
	productos_.emplace_back(producto, cantidad);
	calcularTotal();
}

float Factura::getTotal() const {
	return total_;
}

const std::vector<std::pair<Producto*, int>>& Factura::getProductos() const {
	return productos_;
}

void Factura::calcularTotal() {
	total_ = 0.0;
	for (const auto& item : productos_) {
		total_ += item.first->getPrecio() * item.second;
	}
}
