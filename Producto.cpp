#include "Producto.hpp"

// Constructor de Producto
Producto::Producto(int id, const std::string& nombre, float precio)
	: id_(id), nombre_(nombre), precio_(precio) {}

// Implementación de getId
int Producto::getId() const {
	return id_;
}

// Implementación de getNombre
const std::string& Producto::getNombre() const {
	return nombre_;
}


// Implementación de getPrecio
float Producto::getPrecio() const {
	return precio_;
}



