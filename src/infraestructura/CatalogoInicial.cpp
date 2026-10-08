/**
 * @file CatalogoInicial.cpp
 * @brief Implementación del catálogo de ejemplo.
 * @author Santiago Caicedo
 */

#include "infraestructura/CatalogoInicial.hpp"

#include <memory>

#include "dominio/PoliticaDescuento.hpp"

namespace almacen::infraestructura {

dominio::Almacen crearAlmacenConCatalogoInicial() {
    using dominio::DescuentoFijo;
    using dominio::DescuentoPorcentual;
    using dominio::Dinero;
    using dominio::Producto;

    dominio::Almacen principal(1, "Almacen principal");
    principal.agregarProducto(std::make_shared<const Producto>(
        1, "Camisa", Dinero::desdeUnidades(100), std::make_unique<DescuentoFijo>(Dinero::desdeUnidades(10))));
    principal.agregarProducto(std::make_shared<const Producto>(2, "Pantalon", Dinero::desdeUnidades(200),
                                                               std::make_unique<DescuentoPorcentual>(20)));
    principal.agregarProducto(std::make_shared<const Producto>(
        3, "Zapatillas", Dinero::desdeUnidades(300), std::make_unique<DescuentoFijo>(Dinero::desdeUnidades(30))));
    principal.agregarProducto(std::make_shared<const Producto>(4, "Tacones", Dinero::desdeUnidades(400),
                                                               std::make_unique<DescuentoPorcentual>(40)));
    return principal;
}

}  // namespace almacen::infraestructura
