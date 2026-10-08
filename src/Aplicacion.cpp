/**
 * @file Aplicacion.cpp
 * @brief Implementación de la raíz de composición.
 * @author Santiago Caicedo
 */

#include "Aplicacion.hpp"

#include <memory>
#include <ostream>

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "infraestructura/CatalogoInicial.hpp"
#include "infraestructura/RepositoriosEnMemoria.hpp"
#include "presentacion/Consola.hpp"
#include "presentacion/MenuPrincipal.hpp"
#include "presentacion/comandos/ComandosClientes.hpp"
#include "presentacion/comandos/ComandosInventario.hpp"
#include "presentacion/comandos/ComandosPedidos.hpp"

namespace almacen {

void ejecutarAplicacion(std::istream& entrada, std::ostream& salida) {
    using namespace aplicacion;
    using namespace presentacion;

    // Infraestructura
    infraestructura::RepositorioClientesEnMemoria clientes;
    infraestructura::RepositorioPedidosEnMemoria pedidos;
    infraestructura::RepositorioAlmacenesEnMemoria almacenes(infraestructura::crearAlmacenConCatalogoInicial());

    // Aplicación
    CrearCliente crearCliente(clientes);
    ListarClientes listarClientes(clientes);
    CrearPedido crearPedido(clientes, pedidos);
    CrearFactura crearFactura(pedidos, almacenes);
    ConsultarResumenCliente resumenCliente(clientes, pedidos);
    ListarProductos listarProductos(almacenes);
    CrearSubAlmacen crearSubAlmacen(almacenes);
    ConsultarAlmacen consultarAlmacen(almacenes);
    AsignarProductoAAlmacen asignarProducto(almacenes);

    // Presentación
    Consola consola(entrada, salida);
    MenuPrincipal menu(consola);
    menu.agregar(std::make_unique<ComandoCrearCliente>(crearCliente));
    menu.agregar(std::make_unique<ComandoListarClientes>(listarClientes));
    menu.agregar(std::make_unique<ComandoCrearPedido>(crearPedido));
    menu.agregar(std::make_unique<ComandoCrearFactura>(crearFactura, listarProductos));
    menu.agregar(std::make_unique<ComandoResumenCliente>(resumenCliente));
    menu.agregar(std::make_unique<ComandoListarProductos>(listarProductos));
    menu.agregar(std::make_unique<ComandoCrearSubAlmacen>(crearSubAlmacen));
    menu.agregar(std::make_unique<ComandoVerAlmacen>(consultarAlmacen));
    menu.agregar(std::make_unique<ComandoAsignarProducto>(asignarProducto, listarProductos));

    salida << "Sistema de almacen\n";
    salida << "Productos en el almacen principal: " << almacenes.principal().totalProductos() << '\n';
    menu.ejecutar();
}

}  // namespace almacen
