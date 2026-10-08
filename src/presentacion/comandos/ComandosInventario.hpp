/**
 * @file ComandosInventario.hpp
 * @brief Opciones del menú para productos y almacenes.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "presentacion/comandos/Comando.hpp"

namespace almacen::presentacion {

/** @brief Opción "Listar productos". */
class ComandoListarProductos final : public Comando {
public:
    /** @param casoUso Caso de uso que lista los productos. */
    explicit ComandoListarProductos(const aplicacion::ListarProductos& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Listar productos"; }
    void ejecutar(Consola& consola) override;

private:
    const aplicacion::ListarProductos& casoUso_;
};

/** @brief Opción "Crear sub-almacén". */
class ComandoCrearSubAlmacen final : public Comando {
public:
    /** @param casoUso Caso de uso que crea el sub-almacén. */
    explicit ComandoCrearSubAlmacen(aplicacion::CrearSubAlmacen& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Crear sub-almacen"; }
    void ejecutar(Consola& consola) override;

private:
    aplicacion::CrearSubAlmacen& casoUso_;
};

/** @brief Opción "Ver almacén". */
class ComandoVerAlmacen final : public Comando {
public:
    /** @param casoUso Caso de uso que consulta el almacén. */
    explicit ComandoVerAlmacen(const aplicacion::ConsultarAlmacen& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Ver almacen o sub-almacen"; }
    void ejecutar(Consola& consola) override;

private:
    const aplicacion::ConsultarAlmacen& casoUso_;
};

/** @brief Opción "Agregar producto a un sub-almacén". */
class ComandoAsignarProducto final : public Comando {
public:
    /**
     * @param asignar Caso de uso que asigna el producto.
     * @param listarProductos Caso de uso para mostrar el catálogo.
     */
    ComandoAsignarProducto(aplicacion::AsignarProductoAAlmacen& asignar,
                           const aplicacion::ListarProductos& listarProductos)
        : asignar_(asignar), listarProductos_(listarProductos) {}
    std::string titulo() const override { return "Agregar producto a un sub-almacen"; }
    void ejecutar(Consola& consola) override;

private:
    aplicacion::AsignarProductoAAlmacen& asignar_;
    const aplicacion::ListarProductos& listarProductos_;
};

}  // namespace almacen::presentacion
