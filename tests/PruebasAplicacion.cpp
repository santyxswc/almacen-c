/**
 * @file PruebasAplicacion.cpp
 * @brief Pruebas de los casos de uso con repositorios en memoria.
 * @author Santiago Caicedo
 */

#include "MiniTest.hpp"
#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoGenerales.hpp"
#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "dominio/Excepciones.hpp"
#include "infraestructura/CatalogoInicial.hpp"
#include "infraestructura/RepositoriosEnMemoria.hpp"

using namespace almacen;
using namespace almacen::aplicacion;
using dominio::Dinero;

namespace {

/** @brief Repositorios y casos de uso listos para cada prueba. */
struct Escenario {
    infraestructura::RepositorioClientesEnMemoria clientes;
    infraestructura::RepositorioPedidosEnMemoria pedidos;
    infraestructura::RepositorioAlmacenesEnMemoria almacenes{infraestructura::crearAlmacenConCatalogoInicial()};

    CrearCliente crearCliente{clientes};
    CrearPedido crearPedido{clientes, pedidos};
    CrearFactura crearFactura{pedidos, almacenes};
    ConsultarResumenCliente resumen{clientes, pedidos};
    ListarProductos listarProductos{almacenes};
    CrearSubAlmacen crearSubAlmacen{almacenes};
    ConsultarAlmacen consultarAlmacen{almacenes};
    AsignarProductoAAlmacen asignar{almacenes};
    CrearFactura vender{pedidos, almacenes, clientes};
    CrearProducto crearProducto{almacenes};
    ListarPedidos listarPedidos{pedidos};
    ConsultarArbolAlmacenes arbol{almacenes};
    ConsultarResumenGeneral resumenGeneral{clientes, pedidos, almacenes};
};

}  // namespace

PRUEBA(crear_cliente_rechaza_ids_repetidos) {
    Escenario e;
    e.crearCliente.ejecutar(1, "Ana");
    VERIFICAR_LANZA(e.crearCliente.ejecutar(1, "Luis"), dominio::EntidadDuplicada);
}

PRUEBA(crear_pedido_exige_un_cliente_existente) {
    Escenario e;
    VERIFICAR_LANZA(e.crearPedido.ejecutar(1, 99), dominio::EntidadNoEncontrada);
}

PRUEBA(flujo_completo_de_facturacion) {
    Escenario e;
    e.crearCliente.ejecutar(1, "Ana");
    e.crearPedido.ejecutar(10, 1);
    // Camisa: 100 - 10 fijo = 90. Pantalon: 200 - 20% = 160.
    const auto factura = e.crearFactura.ejecutar({100, 10, {{1, 2}, {2, 1}}});
    VERIFICAR_IGUAL(Dinero::desdeUnidades(340), factura.total);

    const auto resumen = e.resumen.ejecutar(1);
    VERIFICAR_IGUAL(std::size_t{1}, resumen.pedidos.size());
    VERIFICAR_IGUAL(Dinero::desdeUnidades(340), resumen.totalGeneral);
}

PRUEBA(los_ids_de_factura_son_unicos_entre_pedidos) {
    Escenario e;
    e.crearCliente.ejecutar(1, "Ana");
    e.crearPedido.ejecutar(10, 1);
    e.crearPedido.ejecutar(11, 1);
    e.crearFactura.ejecutar({100, 10, {{1, 1}}});
    VERIFICAR_LANZA(e.crearFactura.ejecutar({100, 11, {{1, 1}}}), dominio::EntidadDuplicada);
}

PRUEBA(crear_factura_valida_productos_y_pedido) {
    Escenario e;
    e.crearCliente.ejecutar(1, "Ana");
    e.crearPedido.ejecutar(10, 1);
    VERIFICAR_LANZA(e.crearFactura.ejecutar({100, 99, {{1, 1}}}), dominio::EntidadNoEncontrada);
    VERIFICAR_LANZA(e.crearFactura.ejecutar({100, 10, {{42, 1}}}), dominio::EntidadNoEncontrada);
    VERIFICAR_LANZA(e.crearFactura.ejecutar({100, 10, {}}), dominio::ValorInvalido);
}

PRUEBA(una_factura_fallida_no_modifica_el_pedido) {
    Escenario e;
    e.crearCliente.ejecutar(1, "Ana");
    e.crearPedido.ejecutar(10, 1);
    VERIFICAR_LANZA(e.crearFactura.ejecutar({100, 10, {{1, 1}, {2, -3}}}), dominio::ValorInvalido);
    VERIFICAR(e.resumen.ejecutar(1).totalGeneral.esCero());
}

PRUEBA(sub_almacenes_anidados_y_asignacion_de_productos) {
    Escenario e;
    e.crearSubAlmacen.ejecutar(2, "Bodega");
    e.crearSubAlmacen.ejecutar(3, "Estante", 2);
    e.asignar.ejecutar(4, 3);

    const auto estante = e.consultarAlmacen.ejecutar(3);
    VERIFICAR_IGUAL(std::size_t{1}, estante.productos.size());
    VERIFICAR_IGUAL(std::string("Tacones"), estante.productos.front().nombre);
    VERIFICAR_IGUAL(std::size_t{1}, e.consultarAlmacen.ejecutar(2).totalProductos);
    VERIFICAR_IGUAL(std::size_t{4}, e.consultarAlmacen.ejecutar(1).totalProductos);
    VERIFICAR_LANZA(e.asignar.ejecutar(4, 3), dominio::EntidadDuplicada);
    VERIFICAR_LANZA(e.crearSubAlmacen.ejecutar(4, "X", 99), dominio::EntidadNoEncontrada);
}

PRUEBA(listar_productos_no_repite_los_asignados_a_sub_almacenes) {
    Escenario e;
    e.crearSubAlmacen.ejecutar(2, "Bodega");
    e.asignar.ejecutar(1, 2);
    VERIFICAR_IGUAL(std::size_t{4}, e.listarProductos.ejecutar().size());
}

PRUEBA(los_ids_se_asignan_solos) {
    Escenario e;
    VERIFICAR_IGUAL(1, e.crearCliente.ejecutar("Ana").id);
    VERIFICAR_IGUAL(2, e.crearCliente.ejecutar("Luis").id);
    VERIFICAR_IGUAL(2, e.crearSubAlmacen.ejecutar(0, "Bodega").id);
    VERIFICAR_IGUAL(3, e.crearSubAlmacen.ejecutar(0, "Estante", 2).id);
}

PRUEBA(vender_sin_pedido_abre_uno_nuevo_con_ids_automaticos) {
    Escenario e;
    const int cliente = e.crearCliente.ejecutar("Ana").id;
    SolicitudFactura venta;
    venta.clienteId = cliente;
    venta.items = {{1, 1}};
    const auto primera = e.vender.ejecutar(venta);
    const auto segunda = e.vender.ejecutar(venta);
    VERIFICAR_IGUAL(1, primera.id);
    VERIFICAR_IGUAL(2, segunda.id);
    VERIFICAR(primera.pedidoId != segunda.pedidoId);
    VERIFICAR_IGUAL(std::size_t{2}, e.listarPedidos.ejecutar().size());
    VERIFICAR_IGUAL(cliente, e.listarPedidos.ejecutar().front().clienteId);
}

PRUEBA(una_venta_invalida_no_deja_pedidos_vacios) {
    Escenario e;
    SolicitudFactura venta;
    venta.clienteId = e.crearCliente.ejecutar("Ana").id;
    venta.items = {{1, 1}, {99, 1}};
    VERIFICAR_LANZA(e.vender.ejecutar(venta), dominio::EntidadNoEncontrada);
    VERIFICAR(e.listarPedidos.ejecutar().empty());

    venta.clienteId = 42;
    venta.items = {{1, 1}};
    VERIFICAR_LANZA(e.vender.ejecutar(venta), dominio::EntidadNoEncontrada);
}

PRUEBA(crear_producto_con_cada_tipo_de_descuento) {
    Escenario e;
    SolicitudProducto solicitud;
    solicitud.nombre = "Gorra";
    solicitud.precio = Dinero::desdeUnidades(50);
    VERIFICAR_IGUAL(5, e.crearProducto.ejecutar(solicitud).id);

    solicitud.tipoDescuento = TipoDescuento::Fijo;
    solicitud.valorDescuento = 5;
    VERIFICAR_IGUAL(Dinero::desdeUnidades(45), e.crearProducto.ejecutar(solicitud).precioFinal);

    solicitud.tipoDescuento = TipoDescuento::Porcentual;
    solicitud.valorDescuento = 10;
    VERIFICAR_IGUAL(Dinero::desdeUnidades(45), e.crearProducto.ejecutar(solicitud).precioFinal);

    solicitud.valorDescuento = 150;
    VERIFICAR_LANZA(e.crearProducto.ejecutar(solicitud), dominio::ValorInvalido);
    solicitud.almacenId = 77;
    solicitud.valorDescuento = 0;
    VERIFICAR_LANZA(e.crearProducto.ejecutar(solicitud), dominio::EntidadNoEncontrada);
    VERIFICAR_IGUAL(std::size_t{7}, e.listarProductos.ejecutar().size());
}

PRUEBA(arbol_y_resumen_general) {
    Escenario e;
    e.crearSubAlmacen.ejecutar(0, "Bodega");
    e.crearSubAlmacen.ejecutar(0, "Estante", 2);
    e.asignar.ejecutar(1, 3);
    const auto arbol = e.arbol.ejecutar();
    VERIFICAR_IGUAL(std::size_t{1}, arbol.hijos.size());
    VERIFICAR_IGUAL(std::string("Estante"), arbol.hijos.front().hijos.front().nombre);
    VERIFICAR_IGUAL(std::size_t{1}, arbol.hijos.front().totalProductos);

    SolicitudFactura venta;
    venta.clienteId = e.crearCliente.ejecutar("Ana").id;
    venta.items = {{1, 2}};
    e.vender.ejecutar(venta);
    const auto resumen = e.resumenGeneral.ejecutar();
    VERIFICAR_IGUAL(std::size_t{1}, resumen.clientes);
    VERIFICAR_IGUAL(std::size_t{4}, resumen.productos);
    VERIFICAR_IGUAL(std::size_t{3}, resumen.almacenes);
    VERIFICAR_IGUAL(std::size_t{1}, resumen.facturas);
    VERIFICAR_IGUAL(Dinero::desdeUnidades(180), resumen.totalFacturado);
}
