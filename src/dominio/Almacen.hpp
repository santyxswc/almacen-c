/**
 * @file Almacen.hpp
 * @brief Almacén con productos y sub-almacenes anidados.
 * @author Santiago Caicedo
 */

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "dominio/Identificable.hpp"
#include "dominio/Producto.hpp"

namespace almacen::dominio {

/**
 * @brief Patrón Composite: un almacén contiene productos y otros almacenes.
 *
 * Las operaciones de consulta (contar o buscar productos, buscar un
 * sub-almacén) recorren todo el árbol, así que el código cliente trata
 * igual a un almacén con o sin sub-almacenes.
 *
 * Los sub-almacenes pertenecen a su padre (`std::unique_ptr`) y los
 * productos se comparten (`std::shared_ptr`) porque el mismo producto
 * puede estar en varios almacenes. Así no hay fugas de memoria.
 */
class Almacen final : public Identificable {
public:
    /**
     * @param id Identificador positivo, único en todo el árbol.
     * @param nombre Nombre descriptivo no vacío.
     * @throws ValorInvalido si algún dato no es válido.
     */
    Almacen(int id, std::string nombre);

    Almacen(const Almacen&) = delete;
    Almacen& operator=(const Almacen&) = delete;
    Almacen(Almacen&&) noexcept = default;
    Almacen& operator=(Almacen&&) noexcept = default;

    int id() const noexcept override { return id_; }

    /** @return Nombre del almacén. */
    const std::string& nombre() const noexcept { return nombre_; }

    /**
     * @brief Guarda un producto directamente en este almacén.
     * @param producto Producto a guardar; no puede ser nulo.
     * @throws ValorInvalido si el producto es nulo.
     * @throws EntidadDuplicada si este almacén ya tiene un producto con ese id.
     */
    void agregarProducto(std::shared_ptr<const Producto> producto);

    /**
     * @brief Agrega un sub-almacén como hijo directo.
     * @param subAlmacen Almacén hijo; no puede ser nulo.
     * @return Referencia al sub-almacén ya agregado.
     * @throws ValorInvalido si el sub-almacén es nulo.
     * @throws EntidadDuplicada si su id ya existe en el árbol.
     */
    Almacen& agregarSubAlmacen(std::unique_ptr<Almacen> subAlmacen);

    /** @return Productos guardados directamente en este almacén. */
    const std::vector<std::shared_ptr<const Producto>>& productos() const noexcept { return productos_; }

    /** @return Sub-almacenes hijos directos. */
    std::vector<const Almacen*> subAlmacenes() const;

    /**
     * @brief Cuenta los productos distintos de este almacén y de todos sus descendientes.
     *
     * Un producto que está en varios almacenes del árbol cuenta una sola vez.
     */
    std::size_t totalProductos() const;

    /**
     * @brief Busca un producto en este almacén y, si no está, en sus descendientes.
     * @param productoId Id del producto.
     * @return El producto, o nullptr si no existe en el árbol.
     */
    std::shared_ptr<const Producto> buscarProducto(int productoId) const;

    /** @return true si el producto está guardado directamente en este almacén. */
    bool contieneProducto(int productoId) const noexcept;

    /**
     * @brief Busca un almacén por id en el árbol (incluye a este mismo).
     * @param almacenId Id buscado.
     * @return Puntero al almacén, o nullptr si no existe.
     */
    Almacen* buscarAlmacen(int almacenId);

    /** @copydoc buscarAlmacen(int) */
    const Almacen* buscarAlmacen(int almacenId) const;

private:
    void recolectarIds(std::vector<int>& ids) const;

    int id_;
    std::string nombre_;
    std::vector<std::shared_ptr<const Producto>> productos_;
    std::vector<std::unique_ptr<Almacen>> subAlmacenes_;
};

}  // namespace almacen::dominio
