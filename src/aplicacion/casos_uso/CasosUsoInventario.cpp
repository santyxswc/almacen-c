/**
 * @file CasosUsoInventario.cpp
 * @brief Implementación de los casos de uso de productos y almacenes.
 * @author Santiago Caicedo
 */

#include "aplicacion/casos_uso/CasosUsoInventario.hpp"

#include <algorithm>
#include <map>
#include <memory>

#include "aplicacion/Mapeo.hpp"
#include "dominio/Excepciones.hpp"

namespace almacen::aplicacion {

namespace {

void recolectarProductos(const dominio::Almacen& almacen, std::map<int, ProductoDto>& productos) {
    for (const auto& producto : almacen.productos()) {
        productos.emplace(producto->id(), aDto(*producto));
    }
    for (const auto* sub : almacen.subAlmacenes()) {
        recolectarProductos(*sub, productos);
    }
}

}  // namespace

std::vector<ProductoDto> ListarProductos::ejecutar() const {
    std::map<int, ProductoDto> porId;
    recolectarProductos(almacenes_.principal(), porId);
    std::vector<ProductoDto> resultado;
    resultado.reserve(porId.size());
    for (auto& [id, producto] : porId) {
        resultado.push_back(std::move(producto));
    }
    return resultado;
}

AlmacenDto CrearSubAlmacen::ejecutar(int id, const std::string& nombre, int padreId) {
    dominio::Almacen& principal = almacenes_.principal();
    dominio::Almacen* padre = padreId == 0 ? &principal : principal.buscarAlmacen(padreId);
    if (padre == nullptr) {
        throw dominio::EntidadNoEncontrada("Almacen", padreId);
    }
    if (principal.buscarAlmacen(id) != nullptr) {
        throw dominio::EntidadDuplicada("almacen", id);
    }
    return aDto(padre->agregarSubAlmacen(std::make_unique<dominio::Almacen>(id, nombre)));
}

AlmacenDto ConsultarAlmacen::ejecutar(int almacenId) const {
    const dominio::Almacen* almacen = almacenes_.principal().buscarAlmacen(almacenId);
    if (almacen == nullptr) {
        throw dominio::EntidadNoEncontrada("Almacen", almacenId);
    }
    return aDto(*almacen);
}

AlmacenDto AsignarProductoAAlmacen::ejecutar(int productoId, int almacenId) {
    dominio::Almacen& principal = almacenes_.principal();
    auto producto = principal.buscarProducto(productoId);
    if (!producto) {
        throw dominio::EntidadNoEncontrada("Producto", productoId);
    }
    dominio::Almacen* destino = principal.buscarAlmacen(almacenId);
    if (destino == nullptr) {
        throw dominio::EntidadNoEncontrada("Almacen", almacenId);
    }
    destino->agregarProducto(std::move(producto));
    return aDto(*destino);
}

}  // namespace almacen::aplicacion
