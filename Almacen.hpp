#ifndef ALMACEN_HPP
#define ALMACEN_HPP

#include <vector>
#include "Producto.hpp"

class Almacen {
public:
	Almacen(int id);
	int getId() const;
	void addProducto(Producto* producto);
	void addSubAlmacen(Almacen* subAlmacen);  // Nueva función para agregar sub-almacenes
	int getTotalProductos() const;
	const std::vector<Almacen*>& getSubAlmacenes() const; // Nueva función para obtener sub-almacenes
	Producto* findProducto(int idProducto) const; 
	const std::vector<Producto*>& getProductos() const; 
	
private:
	int id_;
	std::vector<Producto*> productos_;
	std::vector<Almacen*> subAlmacenes_;  
};

#endif 
