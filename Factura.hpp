#ifndef FACTURA_HPP
#define FACTURA_HPP

#include <vector>
#include <utility>
#include "Identificable.hpp"
#include "Producto.hpp"
#include "Cliente.hpp" 

class Cliente; 

class Factura : public Identificable {
public:
	Factura(int id, Cliente* cliente);
	int getId() const override;
	Cliente* getCliente() const;
	void addProducto(Producto* producto, int cantidad);
	float getTotal() const;
	const std::vector<std::pair<Producto*, int>>& getProductos() const;
	
private:
	void calcularTotal();
	
	int id_;
	Cliente* cliente_;
	float total_;
	std::vector<std::pair<Producto*, int>> productos_;
};

#endif 
