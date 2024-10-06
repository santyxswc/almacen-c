#ifndef PEDIDO_HPP
#define PEDIDO_HPP

#include <vector>
#include "Identificable.hpp"
#include "Factura.hpp"
#include "Cliente.hpp" 

class Cliente;
class Factura; 

class Pedido : public Identificable {
public:
	Pedido(int id, Cliente* cliente);
	int getId() const override;
	Cliente* getCliente() const;
	void addFactura(Factura* factura);
	float getTotalFacturas() const;
	const std::vector<Factura*>& getFacturas() const;
	
private:
	int id_;
	Cliente* cliente_;
	std::vector<Factura*> facturas_;
};

#endif 
