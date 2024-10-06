#include "Almacen.hpp"

Almacen::Almacen(int id) : id_(id) {}

int Almacen::getId() const {
	return id_;
}

void Almacen::addProducto(Producto* producto) {
	productos_.push_back(producto);
}

void Almacen::addSubAlmacen(Almacen* subAlmacen) {  // Implementación para agregar sub-almacenes
	subAlmacenes_.push_back(subAlmacen);
}

int Almacen::getTotalProductos() const {
	int total = productos_.size();
	for (const auto& subAlmacen : subAlmacenes_) {  // Incluir productos de sub-almacenes
		total += subAlmacen->getTotalProductos();
	}
	return total;
}

const std::vector<Almacen*>& Almacen::getSubAlmacenes() const {  // Implementación para obtener sub-almacenes
	return subAlmacenes_;
}

Producto* Almacen::findProducto(int idProducto) const {  // Implementación para buscar producto
	for (const auto& producto : productos_) {
		if (producto->getId() == idProducto) {
			return producto;
		}
	}
	for (const auto& subAlmacen : subAlmacenes_) {
		Producto* producto = subAlmacen->findProducto(idProducto);
		if (producto) {
			return producto;
		}
	}
	return nullptr;
}
const std::vector<Producto*>& Almacen::getProductos() const {  // Implementación para obtener productos
	return productos_;
}
