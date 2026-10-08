/**
 * @file Almacen.cpp
 * @brief Implementación de Almacen.
 * @author Santiago Caicedo
 */

#include "dominio/Almacen.hpp"

#include <algorithm>
#include <utility>

#include "dominio/Excepciones.hpp"
#include "dominio/Validaciones.hpp"

namespace almacen::dominio {

Almacen::Almacen(int id, std::string nombre)
    : id_(validarId(id, "almacen")), nombre_(validarNombre(std::move(nombre), "almacen")) {}

void Almacen::agregarProducto(std::shared_ptr<const Producto> producto) {
    if (!producto) {
        throw ValorInvalido("No se puede agregar un producto nulo.");
    }
    if (contieneProducto(producto->id())) {
        throw EntidadDuplicada("producto en el almacen " + std::to_string(id_), producto->id());
    }
    productos_.push_back(std::move(producto));
}

Almacen& Almacen::agregarSubAlmacen(std::unique_ptr<Almacen> subAlmacen) {
    if (!subAlmacen) {
        throw ValorInvalido("No se puede agregar un sub-almacen nulo.");
    }
    if (buscarAlmacen(subAlmacen->id()) != nullptr) {
        throw EntidadDuplicada("almacen", subAlmacen->id());
    }
    subAlmacenes_.push_back(std::move(subAlmacen));
    return *subAlmacenes_.back();
}

std::vector<const Almacen*> Almacen::subAlmacenes() const {
    std::vector<const Almacen*> hijos;
    hijos.reserve(subAlmacenes_.size());
    for (const auto& sub : subAlmacenes_) {
        hijos.push_back(sub.get());
    }
    return hijos;
}

std::size_t Almacen::totalProductos() const {
    std::vector<int> ids;
    recolectarIds(ids);
    std::sort(ids.begin(), ids.end());
    return static_cast<std::size_t>(std::unique(ids.begin(), ids.end()) - ids.begin());
}

void Almacen::recolectarIds(std::vector<int>& ids) const {
    for (const auto& producto : productos_) {
        ids.push_back(producto->id());
    }
    for (const auto& sub : subAlmacenes_) {
        sub->recolectarIds(ids);
    }
}

std::shared_ptr<const Producto> Almacen::buscarProducto(int productoId) const {
    for (const auto& producto : productos_) {
        if (producto->id() == productoId) {
            return producto;
        }
    }
    for (const auto& sub : subAlmacenes_) {
        if (auto encontrado = sub->buscarProducto(productoId)) {
            return encontrado;
        }
    }
    return nullptr;
}

bool Almacen::contieneProducto(int productoId) const noexcept {
    return std::any_of(productos_.begin(), productos_.end(),
                       [productoId](const auto& producto) { return producto->id() == productoId; });
}

Almacen* Almacen::buscarAlmacen(int almacenId) {
    return const_cast<Almacen*>(std::as_const(*this).buscarAlmacen(almacenId));
}

const Almacen* Almacen::buscarAlmacen(int almacenId) const {
    if (id_ == almacenId) {
        return this;
    }
    for (const auto& sub : subAlmacenes_) {
        if (const Almacen* encontrado = sub->buscarAlmacen(almacenId)) {
            return encontrado;
        }
    }
    return nullptr;
}

}  // namespace almacen::dominio
