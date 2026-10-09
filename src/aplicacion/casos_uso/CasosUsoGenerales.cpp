/**
 * @file CasosUsoGenerales.cpp
 * @brief Implementación de los casos de uso generales.
 * @author Santiago Caicedo
 */

#include "aplicacion/casos_uso/CasosUsoGenerales.hpp"

namespace almacen::aplicacion {

namespace {

std::size_t contarAlmacenes(const dominio::Almacen& almacen) {
    std::size_t total = 1;
    for (const auto* sub : almacen.subAlmacenes()) {
        total += contarAlmacenes(*sub);
    }
    return total;
}

}  // namespace

ResumenGeneralDto ConsultarResumenGeneral::ejecutar() const {
    ResumenGeneralDto resumen;
    resumen.clientes = clientes_.listar().size();
    resumen.productos = almacenes_.principal().totalProductos();
    resumen.almacenes = contarAlmacenes(almacenes_.principal());
    for (const auto& pedido : pedidos_.listar()) {
        ++resumen.pedidos;
        resumen.facturas += pedido.facturas().size();
        resumen.totalFacturado += pedido.total();
    }
    return resumen;
}

}  // namespace almacen::aplicacion
