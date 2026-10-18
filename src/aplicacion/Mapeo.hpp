/**
 * @file Mapeo.hpp
 * @brief Conversión de entidades del dominio a DTOs.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/Dtos.hpp"
#include "dominio/Almacen.hpp"
#include "dominio/Cliente.hpp"
#include "dominio/Pedido.hpp"

namespace almacen::aplicacion {

/**
 * @brief Convierte un cliente en su DTO.
 * @param cliente Cliente a convertir.
 * @return Sus datos para mostrar.
 */
ClienteDto aDto(const dominio::Cliente& cliente);

/**
 * @brief Convierte un producto en su DTO.
 * @param producto Producto a convertir.
 * @return Sus datos, con el precio final y el descuento descrito.
 */
ProductoDto aDto(const dominio::Producto& producto);

/**
 * @brief Convierte una factura en su DTO.
 * @param factura Factura a convertir.
 * @param pedidoId Pedido al que pertenece.
 * @return Sus líneas y totales.
 */
FacturaDto aDto(const dominio::Factura& factura, int pedidoId);

/**
 * @brief Convierte un pedido (con sus facturas) en su DTO.
 * @param pedido Pedido a convertir.
 * @return El pedido con sus facturas y su total.
 */
PedidoDto aDto(const dominio::Pedido& pedido);

/**
 * @brief Convierte un almacén en su DTO.
 * @param almacen Almacén a convertir.
 * @return Sus productos directos, los ids de sus sub-almacenes y el total del árbol.
 */
AlmacenDto aDto(const dominio::Almacen& almacen);

}  // namespace almacen::aplicacion
