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
 * @brief Crea un sub-almacén dentro de otro almacén del árbol.
 */
class CrearSubAlmacen {
public:
    /** @param almacenes Repositorio de almacenes. */
    explicit CrearSubAlmacen(RepositorioAlmacenes& almacenes) : almacenes_(almacenes) {}

    /**
     * @param id Id del sub-almacén nuevo, único en el árbol.
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

}  // namespace almacen::aplicacion
