#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include "Identificable.hpp"
#include <string>

class Producto : public Identificable {
public:
	Producto(int id, const std::string& nombre, float precio);
	virtual ~Producto() = default;
	
	int getId() const override;
	const std::string& getNombre() const;
	float getPrecio() const;
	
	virtual float calcularPrecioConDescuento() const = 0; // mwtodo virtual puro
	
protected:
	int id_;
	std::string nombre_;
	float precio_;
};

#endif 
