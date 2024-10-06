#include "ProductoConcreto.hpp"

// Implementación de ProductoConDescuentoFijo

ProductoConDescuentoFijo::ProductoConDescuentoFijo(int id, const std::string& nombre, float precio, float descuento)
	: Producto(id, nombre, precio), descuento_(descuento) {}

float ProductoConDescuentoFijo::calcularPrecioConDescuento() const {
	return precio_ - descuento_;
}

// Implementación de ProductoConDescuentoPorcentual

ProductoConDescuentoPorcentual::ProductoConDescuentoPorcentual(int id, const std::string& nombre, float precio, float descuento)
	: Producto(id, nombre, precio), descuento_(descuento) {}

float ProductoConDescuentoPorcentual::calcularPrecioConDescuento() const {
	return precio_ * (1.0f - descuento_ / 100.0f);
}
