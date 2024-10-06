#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include "ProductoConcreto.hpp"
#include "Cliente.hpp"
#include "Factura.hpp"
#include "Pedido.hpp"
#include "Almacen.hpp"
#include "Excepciones.hpp"

using namespace std;

int leerEntrada() {
	int valor;
	cin >> valor;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		throw EntradaInvalidaException("Entrada no valida. Por favor ingrese un numero entero.");
	}
	return valor;
}

void showMenu() {
	std::cout << "\n_________________________________________\n";
	std::cout << "Menu:                             \t| \n";
	std::cout << "1. Crear Cliente                  \t| \n";
	std::cout << "2. Crear Pedido                   \t| \n";
	std::cout << "3. Crear Factura                  \t| \n";
	std::cout << "4. Mostrar Resultados             \t| \n";
	std::cout << "5. Crear Sub-Almacen              \t| \n";
	std::cout << "6. Mostrar Detalles de Sub-Almacen\t| \n";  
	std::cout << "7. Agregar Producto a Sub-Almacen \t| \n";  
	std::cout << "8. Salir                          \t| \n";
	std::cout << "Seleccione una opcion: ";
}

void mostrarDetallesSubAlmacen(const Almacen& subAlmacen) {
	std::cout << "Sub-Almacen ID: " << subAlmacen.getId() << "\n";
	std::cout << "Total Productos: " << subAlmacen.getTotalProductos() << "\n";
	std::cout << "Productos en el sub-almacen: \n";
	for (const auto& producto : subAlmacen.getProductos()) {
		std::cout << "Producto ID: " << producto->getId()
			<< ", Nombre: " << producto->getNombre()
			<< ", Precio: $" << producto->getPrecio() << "\n";
	}
	std::cout << "____________________________________________\n";
}


void agregarProductoASubAlmacen(Almacen& almacen, int idProducto, int idSubAlmacen) {
	Producto* producto = almacen.findProducto(idProducto);
	if (!producto) {
		std::cout << "Producto no encontrado en el almacén principal.\n";
		return;
	}
	
	auto& subAlmacenes = almacen.getSubAlmacenes();
	auto it = std::find_if(subAlmacenes.begin(), subAlmacenes.end(),
						   [idSubAlmacen](const Almacen* subAlmacen) { return subAlmacen->getId() == idSubAlmacen; });
	if (it != subAlmacenes.end()) {
		(*it)->addProducto(producto);
		std::cout << "Producto agregado al sub-almacen con éxito.\n";
	} else {
		std::cout << "Sub-almacen no encontrado.\n";
	}
}

int main() {
	Almacen almacen(1);
	
	ProductoConDescuentoFijo producto1(1, "Camisa", 100.0, 10.0);
	ProductoConDescuentoPorcentual producto2(2, "Pantalon", 200.0, 20.0);
	ProductoConDescuentoFijo producto3(3, "Zapatillas", 300.0, 30.0);
	ProductoConDescuentoPorcentual producto4(4, "Tacones", 400.0, 40.0);
	
	almacen.addProducto(&producto1);
	almacen.addProducto(&producto2);
	almacen.addProducto(&producto3);
	almacen.addProducto(&producto4);
	
	Cliente* cliente = nullptr;
	Pedido* pedido = nullptr;
	
	std::cout << "Total Productos del almacen: " << almacen.getTotalProductos() << "\n";
	bool running = true;
	
	while (running) {
		try {
			showMenu();
			int choice = leerEntrada();
			std::cout << "\n_________________________________________\n";
			
			switch (choice) {
			case 1: {
				int idCliente;
				std::string nombreCliente;
				
				std::cout << "Ingrese ID del cliente: ";
				idCliente = leerEntrada();
				std::cout << "Ingrese nombre del cliente: ";
				std::cin >> nombreCliente;
				
				cliente = new ClienteConPedido(idCliente, nombreCliente, nullptr);
				std::cout << "Cliente creado con exito.\n";
				break;
			}
			case 2: {
				if (!cliente) {
					throw ClienteNoCreadoException();
				}
				
				int idPedido;
				std::cout << "Ingrese ID del pedido: ";
				idPedido = leerEntrada();
				
				pedido = new Pedido(idPedido, cliente);
				std::cout << "Pedido creado con exito.\n";
				break;
			}
			case 3: {
				if (!cliente || !pedido) {
					throw PedidoNoCreadoException();
				}
				
				int idFactura;
				std::cout << "Ingrese ID de la factura: ";
				idFactura = leerEntrada();
				
				Factura* factura = new Factura(idFactura, cliente);
				
				std::cout << "Los productos son:";
				std::cout << "\n///////////////////////////////////////////////////////////////////\n";
				std::cout << "producto 1: (Camisa) \n";
				std::cout << "producto 2: (Pantalon) \n";
				std::cout << "producto 3: (Zapatillas) \n";
				std::cout << "producto 4: (Tacones) \n";
				std::cout << "\n///////////////////////////////////////////////////////////////////\n";
				
				int cantidadProducto1, cantidadProducto2, cantidadProducto3, cantidadProducto4;
				std::cout << "Ingrese cantidad del primer producto en la factura: ";
				cantidadProducto1 = leerEntrada();
				factura->addProducto(&producto1, cantidadProducto1);
				
				std::cout << "Ingrese cantidad del segundo producto en la factura: ";
				cantidadProducto2 = leerEntrada();
				factura->addProducto(&producto2, cantidadProducto2);
				
				std::cout << "Ingrese cantidad del tercer producto en la factura: ";
				cantidadProducto3 = leerEntrada();
				factura->addProducto(&producto3, cantidadProducto3);
				
				std::cout << "Ingrese cantidad del cuarto producto en la factura: ";
				cantidadProducto4 = leerEntrada();
				factura->addProducto(&producto4, cantidadProducto4);
				
				pedido->addFactura(factura);
				cliente->addFactura(factura);
				std::cout << "Factura creada y agregada al pedido con éxito.\n";
				break;
			}
			case 4: {
				if (!pedido) {
					std::cout << "No hay pedidos para mostrar.\n";
					break;
				}
				
				std::cout << "Cliente ID: " << cliente->getId() << ", Nombre: " << cliente->getNombre() << " \n";
				for (auto factura : cliente->getFacturas()) {
					std::cout << "Factura ID: " << factura->getId() << ", Total: $" << factura->getTotal() <<" \n";
					std::cout << "Productos en la factura:  \n";
					for (const auto& producto : factura->getProductos()) {
						std::cout << "Producto: " << producto.first->getNombre()
							<< ", Cantidad: " << producto.second
							<< ", Precio Unitario: $" << producto.first->getPrecio() << "\n";
					}
				}
				
				std::cout << "Precio total a pagar en la Factura del Pedido: $" << pedido->getTotalFacturas() << "\n";
				std::cout << "________________________________________________________________________\n";
				break;
			}
			case 5: {  // Nueva opción para crear sub-almacenes
				int idSubAlmacen;
				std::cout << "Ingrese ID del sub-almacen: ";
				idSubAlmacen = leerEntrada();
				
				Almacen* subAlmacen = new Almacen(idSubAlmacen);
				almacen.addSubAlmacen(subAlmacen);
				std::cout << "Sub-almacen creado con éxito.\n";
				break;
			}
			case 6: { 
				int idSubAlmacen;
				std::cout << "Ingrese ID del sub-almacen: ";
				idSubAlmacen = leerEntrada();
				
				auto& subAlmacenes = almacen.getSubAlmacenes();
				auto it = std::find_if(subAlmacenes.begin(), subAlmacenes.end(),
									   [idSubAlmacen](const Almacen* subAlmacen) { return subAlmacen->getId() == idSubAlmacen; });
				if (it != subAlmacenes.end()) {
					mostrarDetallesSubAlmacen(**it);
				} else {
					std::cout << "Sub-almacen no encontrado.\n";
				}
				break;
			}
			case 7: { 
				
				std::cout << "Los productos son:";
				std::cout << "\n///////////////////////////////////////////////////////////////////\n";
				std::cout << "producto 1: (Camisa) \n";
				std::cout << "producto 2: (Pantalon) \n";
				std::cout << "producto 3: (Zapatillas) \n";
				std::cout << "producto 4: (Tacones) \n";
				std::cout << "\n///////////////////////////////////////////////////////////////////\n";
				int idProducto, idSubAlmacen;
				std::cout << "Ingrese ID del producto: ";
				idProducto = leerEntrada();
				std::cout << "Ingrese ID del sub-almacen: ";
				idSubAlmacen = leerEntrada();
				
				agregarProductoASubAlmacen(almacen, idProducto, idSubAlmacen);
				break;
			}
			case 8:
				running = false;
				break;
				
			default:
				std::cout << "Opción inválida\n";
			}
		} catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
	}
	
	return 0;
}
