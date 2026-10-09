/**
 * @file CasosUsoGenerales.hpp
 * @brief Casos de uso que reúnen información de todo el negocio.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/Dtos.hpp"
#include "aplicacion/puertos/RepositorioAlmacenes.hpp"
#include "aplicacion/puertos/RepositorioClientes.hpp"
#include "aplicacion/puertos/RepositorioPedidos.hpp"

namespace almacen::aplicacion {

/**
 * @brief Calcula las cifras generales que se muestran en la pantalla de inicio.
 */
class ConsultarResumenGeneral {
public:
    /**
     * @param clientes Repositorio de clientes.
     * @param pedidos Repositorio de pedidos.
     * @param almacenes Repositorio de almacenes.
     */
    ConsultarResumenGeneral(const RepositorioClientes& clientes, const RepositorioPedidos& pedidos,
                            const RepositorioAlmacenes& almacenes)
        : clientes_(clientes), pedidos_(pedidos), almacenes_(almacenes) {}

    /** @return Cantidad de clientes, productos, almacenes, pedidos, facturas y total facturado. */
    ResumenGeneralDto ejecutar() const;

private:
    const RepositorioClientes& clientes_;
    const RepositorioPedidos& pedidos_;
    const RepositorioAlmacenes& almacenes_;
};

}  // namespace almacen::aplicacion
