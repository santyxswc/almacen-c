/**
 * @file AplicacionEscritorio.cpp
 * @brief Implementación de la raíz de composición de escritorio.
 * @author Santiago Caicedo
 */

#include "escritorio/AplicacionEscritorio.hpp"

#include <exception>
#include <utility>

#include "infraestructura/CatalogoInicial.hpp"
#include "presentacion/escritorio/PaginaAlmacenes.hpp"
#include "presentacion/escritorio/PaginaClientes.hpp"
#include "presentacion/escritorio/PaginaHistorial.hpp"
#include "presentacion/escritorio/PaginaInicio.hpp"
#include "presentacion/escritorio/PaginaProductos.hpp"
#include "presentacion/escritorio/PaginaVenta.hpp"
#include "presentacion/escritorio/VentanaPrincipal.hpp"

namespace almacen::escritorio {

namespace {

std::unique_ptr<infraestructura::RepositorioAlmacenesEnMemoria> cargarDatos(
    const infraestructura::ArchivoDatos& archivo, infraestructura::RepositorioClientesEnMemoria& clientes,
    infraestructura::RepositorioPedidosEnMemoria& pedidos) {
    if (!archivo.existe()) {
        return std::make_unique<infraestructura::RepositorioAlmacenesEnMemoria>(
            infraestructura::crearAlmacenConCatalogoInicial());
    }
    auto datos = archivo.cargar();
    for (const auto& cliente : datos.clientes) {
        clientes.guardar(cliente);
    }
    for (const auto& pedido : datos.pedidos) {
        pedidos.guardar(pedido);
    }
    return std::make_unique<infraestructura::RepositorioAlmacenesEnMemoria>(std::move(datos.principal));
}

}  // namespace

AplicacionEscritorio::AplicacionEscritorio(const std::string& rutaDatos)
    : archivo_(rutaDatos),
      almacenes_(cargarDatos(archivo_, clientes_, pedidos_)),
      crearFactura_(pedidos_, *almacenes_, clientes_),
      listarProductos_(*almacenes_),
      crearProducto_(*almacenes_),
      crearSubAlmacen_(*almacenes_),
      consultarAlmacen_(*almacenes_),
      asignarProducto_(*almacenes_),
      arbolAlmacenes_(*almacenes_),
      resumenGeneral_(clientes_, pedidos_, *almacenes_) {
    using namespace presentacion::escritorio;
    ventana_ = std::make_unique<VentanaPrincipal>([this] { return QString::fromStdString(guardar()); });
    auto& v = *ventana_;
    v.agregarPagina(QStringLiteral("Inicio"), new PaginaInicio(resumenGeneral_, listarPedidos_, listarClientes_));
    v.agregarPagina(QStringLiteral("Nueva venta"),
                    new PaginaVenta(listarClientes_, crearCliente_, listarProductos_, crearFactura_));
    v.agregarPagina(QStringLiteral("Pedidos y facturas"), new PaginaHistorial(listarPedidos_, listarClientes_));
    v.agregarPagina(QStringLiteral("Clientes"), new PaginaClientes(listarClientes_, crearCliente_, resumenCliente_));
    v.agregarPagina(QStringLiteral("Productos"),
                    new PaginaProductos(listarProductos_, crearProducto_, arbolAlmacenes_));
    v.agregarPagina(
        QStringLiteral("Almacenes"),
        new PaginaAlmacenes(arbolAlmacenes_, consultarAlmacen_, crearSubAlmacen_, asignarProducto_, listarProductos_));
    v.mostrarPagina(PaginaDeInicio);
}

AplicacionEscritorio::~AplicacionEscritorio() = default;

std::string AplicacionEscritorio::guardar() {
    try {
        archivo_.guardar(clientes_, pedidos_, *almacenes_);
        return {};
    } catch (const std::exception& error) {
        return error.what();
    }
}

}  // namespace almacen::escritorio
