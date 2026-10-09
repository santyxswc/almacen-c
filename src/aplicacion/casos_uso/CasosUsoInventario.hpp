/**
 * @file CasosUsoInventario.hpp
 * @brief Casos de uso de productos y almacenes.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>
#include <vector>

#include "aplicacion/Dtos.hpp"
#include "aplicacion/puertos/RepositorioAlmacenes.hpp"

namespace almacen::aplicacion {

/**
 * @brief Lista los productos distintos de todo el árbol de almacenes.
 */
class ListarProductos {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit ListarProductos(const RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /** @return Productos ordenados por id, sin repetir. */
    std::vector<ProductoDto> ejecutar() const;

private:
    const RepositorioAlmacenes& almacenes_;
};

/**
 * @brief Registra un producto nuevo en el catálogo con el siguiente id libre.
 */
class CrearProducto {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit CrearProducto(RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /**
     * @param solicitud Nombre, precio, descuento y almacén donde se guarda.
     * @return El producto creado, con su id y su precio final.
     * @throws dominio::ValorInvalido si el nombre está vacío o el descuento no es válido.
     * @throws dominio::EntidadNoEncontrada si el almacén no existe.
     */
    ProductoDto ejecutar(const SolicitudProducto& solicitud);

private:
    RepositorioAlmacenes& almacenes_;
};

/**
 * @brief Crea un sub-almacén dentro de otro almacén del árbol.
 */
class CrearSubAlmacen {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit CrearSubAlmacen(RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /**
     * @param id Id del sub-almacén nuevo, único en el árbol; 0 = el siguiente libre.
     * @param nombre Nombre del sub-almacén.
     * @param padreId Almacén donde se crea; 0 significa el almacén principal.
     * @return El sub-almacén creado.
     * @throws dominio::EntidadNoEncontrada si el almacén padre no existe.
     * @throws dominio::EntidadDuplicada si el id ya existe en el árbol.
     */
    AlmacenDto ejecutar(int id, const std::string& nombre, int padreId = 0);

private:
    RepositorioAlmacenes& almacenes_;
};

/**
 * @brief Muestra un almacén con sus productos y sub-almacenes.
 */
class ConsultarAlmacen {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit ConsultarAlmacen(const RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /**
     * @param almacenId Almacén a consultar.
     * @return Los datos del almacén.
     * @throws dominio::EntidadNoEncontrada si el almacén no existe.
     */
    AlmacenDto ejecutar(int almacenId) const;

private:
    const RepositorioAlmacenes& almacenes_;
};

/**
 * @brief Coloca un producto existente en otro almacén del árbol.
 */
class AsignarProductoAAlmacen {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit AsignarProductoAAlmacen(RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /**
     * @param productoId Producto a asignar; debe existir en algún almacén.
     * @param almacenId Almacén destino.
     * @return El almacén destino actualizado.
     * @throws dominio::EntidadNoEncontrada si el producto o el almacén no existen.
     * @throws dominio::EntidadDuplicada si el almacén ya tenía ese producto.
     */
    AlmacenDto ejecutar(int productoId, int almacenId);

private:
    RepositorioAlmacenes& almacenes_;
};

/**
 * @brief Devuelve el árbol completo de almacenes para mostrarlo.
 */
class ConsultarArbolAlmacenes {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit ConsultarArbolAlmacenes(const RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /** @return El almacén principal con todos sus descendientes. */
    NodoAlmacenDto ejecutar() const;

private:
    const RepositorioAlmacenes& almacenes_;
};

}  // namespace almacen::aplicacion
