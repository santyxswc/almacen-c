#ifndef PRODUCTOCONCRETO_HPP
#define PRODUCTOCONCRETO_HPP

#include "Producto.hpp"

class ProductoConDescuentoFijo : public Producto {
public:
	ProductoConDescuentoFijo(int id, const std::string& nombre, float precio, float descuento);
	
	float calcularPrecioConDescuento() const override;
	
private:
	float descuento_;
};

class ProductoConDescuentoPorcentual : public Producto {
public:
	ProductoConDescuentoPorcentual(int id, const std::string& nombre, float precio, float descuento);
	
	float calcularPrecioConDescuento() const override;
	
private:
	float descuento_;
};

#endif 
