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
#include "dominio/PoliticaDescuento.hpp"

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

void recolectarIdsAlmacenes(const dominio::Almacen& almacen, int& mayor) {
    mayor = std::max(mayor, almacen.id());
    for (const auto* sub : almacen.subAlmacenes()) {
        recolectarIdsAlmacenes(*sub, mayor);
    }
}

NodoAlmacenDto aNodo(const dominio::Almacen& almacen) {
    NodoAlmacenDto nodo;
    nodo.id = almacen.id();
    nodo.nombre = almacen.nombre();
    nodo.totalProductos = almacen.totalProductos();
    nodo.productosPropios = almacen.productos().size();
    for (const auto* sub : almacen.subAlmacenes()) {
        nodo.hijos.push_back(aNodo(*sub));
    }
    return nodo;
}

std::unique_ptr<dominio::PoliticaDescuento> crearDescuento(TipoDescuento tipo, double valor) {
    switch (tipo) {
        case TipoDescuento::Fijo:
            return std::make_unique<dominio::DescuentoFijo>(dominio::Dinero::desdeUnidades(valor));
        case TipoDescuento::Porcentual:
            return std::make_unique<dominio::DescuentoPorcentual>(valor);
        case TipoDescuento::Ninguno:
            break;
    }
    return std::make_unique<dominio::SinDescuento>();
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

ProductoDto CrearProducto::ejecutar(const SolicitudProducto& solicitud) {
    dominio::Almacen& principal = almacenes_.principal();
    dominio::Almacen* destino = solicitud.almacenId == 0 ? &principal : principal.buscarAlmacen(solicitud.almacenId);
    if (destino == nullptr) {
        throw dominio::EntidadNoEncontrada("Almacen", solicitud.almacenId);
    }
    std::map<int, ProductoDto> existentes;
    recolectarProductos(principal, existentes);
    const int id = existentes.empty() ? 1 : existentes.rbegin()->first + 1;

    auto producto = std::make_shared<const dominio::Producto>(
        id, solicitud.nombre, solicitud.precio, crearDescuento(solicitud.tipoDescuento, solicitud.valorDescuento));
    destino->agregarProducto(producto);
    return aDto(*producto);
}

AlmacenDto CrearSubAlmacen::ejecutar(int id, const std::string& nombre, int padreId) {
    dominio::Almacen& principal = almacenes_.principal();
    dominio::Almacen* padre = padreId == 0 ? &principal : principal.buscarAlmacen(padreId);
    if (padre == nullptr) {
        throw dominio::EntidadNoEncontrada("Almacen", padreId);
    }
    if (id == 0) {
        int mayor = 0;
        recolectarIdsAlmacenes(principal, mayor);
        id = mayor + 1;
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

NodoAlmacenDto ConsultarArbolAlmacenes::ejecutar() const {
    return aNodo(almacenes_.principal());
}

}  // namespace almacen::aplicacion
