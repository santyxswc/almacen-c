#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include <vector>
#include "Identificable.hpp"
#include "Factura.hpp" 
#include "Pedido.hpp" 

class Factura; 
class Pedido; 

class Cliente : public Identificable {
public:
	Cliente(int id, const std::string& nombre);
	int getId() const override;
	const std::string& getNombre() const;
	void addFactura(Factura* factura);
	const std::vector<Factura*>& getFacturas() const;
	
protected:
	int id_;
	std::string nombre_;
	std::vector<Factura*> facturas_;
};

class ClienteConPedido : public Cliente {
public:
	ClienteConPedido(int id, const std::string& nombre, Pedido* pedido);
	Pedido* getPedido() const;
	
private:
	Pedido* pedido_;
};

#endif 
