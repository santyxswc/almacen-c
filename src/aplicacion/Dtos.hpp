/**
 * @file Dtos.hpp
 * @brief Objetos de transferencia de datos que devuelven los casos de uso.
 * @author Santiago Caicedo
 */

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "dominio/Dinero.hpp"

namespace almacen::aplicacion {

/**
 * @brief Datos de un cliente para mostrar.
 */
struct ClienteDto {
    int id = 0;          ///< Id del cliente.
    std::string nombre;  ///< Nombre del cliente.
};

/**
 * @brief Datos de un producto para mostrar.
 */
struct ProductoDto {
    int id = 0;                   ///< Id del producto.
    std::string nombre;           ///< Nombre del producto.
    dominio::Dinero precioBase;   ///< Precio de lista.
    dominio::Dinero precioFinal;  ///< Precio con descuento.
    std::string descuento;        ///< Descripción del descuento.
};

/**
 * @brief Una línea de factura para mostrar.
 */
struct LineaFacturaDto {
    std::string producto;            ///< Nombre del producto.
    int cantidad = 0;                ///< Unidades.
    dominio::Dinero precioUnitario;  ///< Precio con descuento de una unidad.
    dominio::Dinero subtotal;        ///< Precio unitario por cantidad.
};

/**
 * @brief Una factura con sus líneas y totales.
 */
struct FacturaDto {
    int id = 0;                           ///< Id de la factura.
    int pedidoId = 0;                     ///< Pedido al que pertenece.
    std::vector<LineaFacturaDto> lineas;  ///< Productos facturados.
    dominio::Dinero ahorro;               ///< Total ahorrado en descuentos.
    dominio::Dinero total;                ///< Total a pagar.
};

/**
 * @brief Un pedido con sus facturas.
 */
struct PedidoDto {
    int id = 0;                        ///< Id del pedido.
    std::vector<FacturaDto> facturas;  ///< Facturas del pedido.
    dominio::Dinero total;             ///< Suma de las facturas.
};

/**
 * @brief Todo lo que un cliente ha pedido y facturado.
 */
struct ResumenClienteDto {
    ClienteDto cliente;              ///< Datos del cliente.
    std::vector<PedidoDto> pedidos;  ///< Pedidos del cliente.
    dominio::Dinero totalGeneral;    ///< Suma de todos sus pedidos.
};

/**
 * @brief Un almacén con sus productos directos y sus sub-almacenes.
 */
struct AlmacenDto {
    int id = 0;                          ///< Id del almacén.
    std::string nombre;                  ///< Nombre del almacén.
    std::size_t totalProductos = 0;      ///< Productos distintos en todo su árbol.
    std::vector<ProductoDto> productos;  ///< Productos guardados directamente.
    std::vector<int> subAlmacenes;       ///< Ids de los sub-almacenes hijos.
};

/**
 * @brief Un producto pedido en una factura nueva.
 */
struct ItemSolicitado {
    int productoId = 0;  ///< Producto a facturar.
    int cantidad = 0;    ///< Unidades, mayor que cero.
};

/**
 * @brief Datos de entrada para crear una factura.
 */
struct SolicitudFactura {
    int facturaId = 0;                  ///< Id de la factura nueva.
    int pedidoId = 0;                   ///< Pedido al que se agrega.
    std::vector<ItemSolicitado> items;  ///< Productos a facturar.
};

}  // namespace almacen::aplicacion
