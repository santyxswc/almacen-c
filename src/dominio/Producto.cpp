/**
 * @file Producto.cpp
 * @brief Implementación de Producto.
 * @author Santiago Caicedo
 */

#include "dominio/Producto.hpp"

#include <utility>

#include "dominio/Excepciones.hpp"
#include "dominio/Validaciones.hpp"

namespace almacen::dominio {

Producto::Producto(int id, std::string nombre, Dinero precioBase, std::unique_ptr<PoliticaDescuento> descuento)
    : id_(validarId(id, "producto")),
      nombre_(validarNombre(std::move(nombre), "producto")),
      precioBase_(precioBase),
      descuento_(descuento ? std::move(descuento) : std::make_unique<SinDescuento>()) {}

Producto::Producto(const Producto& otro)
    : id_(otro.id_), nombre_(otro.nombre_), precioBase_(otro.precioBase_), descuento_(otro.descuento_->clonar()) {}

Producto& Producto::operator=(const Producto& otro) {
    if (this != &otro) {
        Producto copia(otro);
        *this = std::move(copia);
    }
    return *this;
}

Dinero Producto::precioFinal() const {
    return descuento_->aplicar(precioBase_);
}

void Producto::cambiarDescuento(std::unique_ptr<PoliticaDescuento> descuento) {
    descuento_ = descuento ? std::move(descuento) : std::make_unique<SinDescuento>();
}

}  // namespace almacen::dominio
