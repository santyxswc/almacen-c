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

/** @brief Convierte un cliente en su DTO. */
ClienteDto aDto(const dominio::Cliente& cliente);

/** @brief Convierte un producto en su DTO. */
ProductoDto aDto(const dominio::Producto& producto);

/**
 * @brief Convierte una factura en su DTO.
 * @param factura Factura a convertir.
 * @param pedidoId Pedido al que pertenece.
 */
FacturaDto aDto(const dominio::Factura& factura, int pedidoId);

/** @brief Convierte un pedido (con sus facturas) en su DTO. */
PedidoDto aDto(const dominio::Pedido& pedido);

/** @brief Convierte un almacén en su DTO. */
AlmacenDto aDto(const dominio::Almacen& almacen);

}  // namespace almacen::aplicacion
