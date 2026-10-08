/**
 * @file PruebasDominio.cpp
 * @brief Pruebas de las reglas del negocio (sin infraestructura ni consola).
 * @author Santiago Caicedo
 */

#include <memory>

#include "MiniTest.hpp"
#include "dominio/Almacen.hpp"
#include "dominio/Cliente.hpp"
#include "dominio/Excepciones.hpp"
#include "dominio/Pedido.hpp"
#include "dominio/PoliticaDescuento.hpp"
#include "dominio/Producto.hpp"

using namespace almacen::dominio;

namespace {

Dinero pesos(double valor) {
    return Dinero::desdeUnidades(valor);
}

std::shared_ptr<const Producto> producto(int id, double precio,
                                         std::unique_ptr<PoliticaDescuento> descuento = nullptr) {
    return std::make_shared<const Producto>(id, "Producto " + std::to_string(id), pesos(precio), std::move(descuento));
}

}  // namespace

PRUEBA(dinero_suma_sin_errores_de_redondeo) {
    VERIFICAR_IGUAL(pesos(0.3), pesos(0.1) + pesos(0.2));
}

PRUEBA(dinero_se_formatea_con_dos_decimales) {
    VERIFICAR_IGUAL(std::string("$1250.05"), Dinero::desdeCentavos(125005).formatear());
}

PRUEBA(dinero_no_acepta_negativos) {
    VERIFICAR_LANZA(Dinero::desdeUnidades(-1), ValorInvalido);
}

PRUEBA(descuento_fijo_resta_el_monto) {
    VERIFICAR_IGUAL(pesos(90), DescuentoFijo(pesos(10)).aplicar(pesos(100)));
}

PRUEBA(descuento_fijo_nunca_deja_el_precio_negativo) {
    VERIFICAR_IGUAL(Dinero(), DescuentoFijo(pesos(500)).aplicar(pesos(100)));
}

PRUEBA(descuento_porcentual_aplica_el_porcentaje) {
    VERIFICAR_IGUAL(pesos(160), DescuentoPorcentual(20).aplicar(pesos(200)));
}

PRUEBA(descuento_porcentual_fuera_de_rango_es_invalido) {
    VERIFICAR_LANZA(DescuentoPorcentual(120), ValorInvalido);
}

PRUEBA(producto_sin_descuento_cuesta_su_precio_base) {
    VERIFICAR_IGUAL(pesos(50), producto(1, 50)->precioFinal());
}

PRUEBA(producto_valida_id_y_nombre) {
    VERIFICAR_LANZA(Producto(0, "X", pesos(1)), ValorInvalido);
    VERIFICAR_LANZA(Producto(1, "   ", pesos(1)), ValorInvalido);
}

PRUEBA(copiar_un_producto_copia_su_descuento) {
    Producto original(1, "Camisa", pesos(100), std::make_unique<DescuentoPorcentual>(50));
    Producto copia(original);
    original.cambiarDescuento(nullptr);
    VERIFICAR_IGUAL(pesos(50), copia.precioFinal());
    VERIFICAR_IGUAL(pesos(100), original.precioFinal());
}

PRUEBA(factura_usa_el_precio_con_descuento) {
    Factura factura(1, 1);
    factura.agregarProducto(*producto(1, 100, std::make_unique<DescuentoFijo>(pesos(10))), 2);
    factura.agregarProducto(*producto(2, 200, std::make_unique<DescuentoPorcentual>(20)), 1);
    VERIFICAR_IGUAL(pesos(340), factura.total());
    VERIFICAR_IGUAL(pesos(60), factura.ahorroTotal());
}

PRUEBA(factura_suma_cantidades_del_mismo_producto) {
    Factura factura(1, 1);
    const auto camisa = producto(1, 10);
    factura.agregarProducto(*camisa, 2);
    factura.agregarProducto(*camisa, 3);
    VERIFICAR_IGUAL(std::size_t{1}, factura.lineas().size());
    VERIFICAR_IGUAL(5, factura.lineas().front().cantidad());
}

PRUEBA(factura_rechaza_cantidades_no_positivas) {
    Factura factura(1, 1);
    VERIFICAR_LANZA(factura.agregarProducto(*producto(1, 10), 0), ValorInvalido);
}

PRUEBA(pedido_suma_sus_facturas) {
    Pedido pedido(1, 7);
    Factura primera(1, 7);
    primera.agregarProducto(*producto(1, 10), 1);
    Factura segunda(2, 7);
    segunda.agregarProducto(*producto(2, 5), 2);
    pedido.agregarFactura(primera);
    pedido.agregarFactura(segunda);
    VERIFICAR_IGUAL(pesos(20), pedido.total());
}

PRUEBA(pedido_rechaza_facturas_vacias_de_otro_cliente_o_repetidas) {
    Pedido pedido(1, 7);
    VERIFICAR_LANZA(pedido.agregarFactura(Factura(1, 7)), ValorInvalido);

    Factura deOtro(2, 8);
    deOtro.agregarProducto(*producto(1, 10), 1);
    VERIFICAR_LANZA(pedido.agregarFactura(deOtro), ValorInvalido);

    Factura valida(3, 7);
    valida.agregarProducto(*producto(1, 10), 1);
    pedido.agregarFactura(valida);
    VERIFICAR_LANZA(pedido.agregarFactura(valida), EntidadDuplicada);
}

PRUEBA(almacen_cuenta_productos_de_todo_el_arbol_sin_repetir) {
    Almacen principal(1, "Principal");
    const auto camisa = producto(1, 10);
    principal.agregarProducto(camisa);
    principal.agregarProducto(producto(2, 20));
    Almacen& bodega = principal.agregarSubAlmacen(std::make_unique<Almacen>(2, "Bodega"));
    bodega.agregarProducto(camisa);
    bodega.agregarProducto(producto(3, 30));
    Almacen& estante = bodega.agregarSubAlmacen(std::make_unique<Almacen>(3, "Estante"));
    estante.agregarProducto(producto(4, 40));

    VERIFICAR_IGUAL(std::size_t{4}, principal.totalProductos());
    VERIFICAR_IGUAL(std::size_t{3}, bodega.totalProductos());
}

PRUEBA(almacen_busca_productos_y_almacenes_en_los_descendientes) {
    Almacen principal(1, "Principal");
    Almacen& bodega = principal.agregarSubAlmacen(std::make_unique<Almacen>(2, "Bodega"));
    Almacen& estante = bodega.agregarSubAlmacen(std::make_unique<Almacen>(3, "Estante"));
    estante.agregarProducto(producto(9, 10));

    VERIFICAR(principal.buscarProducto(9) != nullptr);
    VERIFICAR(principal.buscarProducto(99) == nullptr);
    VERIFICAR(principal.buscarAlmacen(3) == &estante);
    VERIFICAR(principal.buscarAlmacen(42) == nullptr);
}

PRUEBA(almacen_no_permite_ids_repetidos) {
    Almacen principal(1, "Principal");
    principal.agregarSubAlmacen(std::make_unique<Almacen>(2, "Bodega"));
    VERIFICAR_LANZA(principal.agregarSubAlmacen(std::make_unique<Almacen>(2, "Otra")), EntidadDuplicada);
    VERIFICAR_LANZA(principal.agregarSubAlmacen(std::make_unique<Almacen>(1, "Raiz")), EntidadDuplicada);

    principal.agregarProducto(producto(1, 10));
    VERIFICAR_LANZA(principal.agregarProducto(producto(1, 10)), EntidadDuplicada);
}

PRUEBA(cliente_recorta_el_nombre) {
    VERIFICAR_IGUAL(std::string("Ana Maria"), Cliente(1, "  Ana Maria ").nombre());
}
