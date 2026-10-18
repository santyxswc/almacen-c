/**
 * @file Vistas.hpp
 * @brief Formato de los datos para mostrarlos en la consola.
 * @author Santiago Caicedo
 */

#pragma once

#include <iosfwd>
#include <vector>

#include "aplicacion/Dtos.hpp"

namespace almacen::presentacion {

/**
 * @brief Imprime una línea separadora.
 * @param salida Flujo donde se escribe.
 */
void mostrarSeparador(std::ostream& salida);

/**
 * @brief Imprime una tabla de productos con precio base, descuento y precio final.
 * @param salida Flujo donde se escribe.
 * @param productos Productos a mostrar.
 */
void mostrarProductos(std::ostream& salida, const std::vector<aplicacion::ProductoDto>& productos);

/**
 * @brief Imprime la lista de clientes.
 * @param salida Flujo donde se escribe.
 * @param clientes Clientes a mostrar.
 */
void mostrarClientes(std::ostream& salida, const std::vector<aplicacion::ClienteDto>& clientes);

/**
 * @brief Imprime una factura con sus líneas, el ahorro y el total.
 * @param salida Flujo donde se escribe.
 * @param factura Factura a mostrar.
 */
void mostrarFactura(std::ostream& salida, const aplicacion::FacturaDto& factura);

/**
 * @brief Imprime los pedidos y facturas de un cliente y su total general.
 * @param salida Flujo donde se escribe.
 * @param resumen Resumen del cliente.
 */
void mostrarResumenCliente(std::ostream& salida, const aplicacion::ResumenClienteDto& resumen);

/**
 * @brief Imprime un almacén con sus productos y sub-almacenes.
 * @param salida Flujo donde se escribe.
 * @param almacen Almacén a mostrar.
 */
void mostrarAlmacen(std::ostream& salida, const aplicacion::AlmacenDto& almacen);

}  // namespace almacen::presentacion
