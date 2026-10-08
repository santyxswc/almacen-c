/**
 * @file ComandosPedidos.hpp
 * @brief Opciones del menú para pedidos y facturas.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "presentacion/comandos/Comando.hpp"

namespace almacen::presentacion {

/** @brief Opción "Crear pedido". */
class ComandoCrearPedido final : public Comando {
public:
    /** @param casoUso Caso de uso que abre el pedido. */
    explicit ComandoCrearPedido(aplicacion::CrearPedido& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Crear pedido"; }
    void ejecutar(Consola& consola) override;

private:
    aplicacion::CrearPedido& casoUso_;
};

/** @brief Opción "Crear factura": muestra el catálogo y pide productos hasta que el usuario escribe 0. */
class ComandoCrearFactura final : public Comando {
public:
    /**
     * @param crearFactura Caso de uso que emite la factura.
     * @param listarProductos Caso de uso para mostrar el catálogo.
     */
    ComandoCrearFactura(aplicacion::CrearFactura& crearFactura, const aplicacion::ListarProductos& listarProductos)
        : crearFactura_(crearFactura), listarProductos_(listarProductos) {}
    std::string titulo() const override { return "Crear factura"; }
    void ejecutar(Consola& consola) override;

private:
    aplicacion::CrearFactura& crearFactura_;
    const aplicacion::ListarProductos& listarProductos_;
};

/** @brief Opción "Ver resumen de un cliente". */
class ComandoResumenCliente final : public Comando {
public:
    /** @param casoUso Caso de uso que arma el resumen. */
    explicit ComandoResumenCliente(const aplicacion::ConsultarResumenCliente& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Ver pedidos y facturas de un cliente"; }
    void ejecutar(Consola& consola) override;

private:
    const aplicacion::ConsultarResumenCliente& casoUso_;
};

}  // namespace almacen::presentacion
