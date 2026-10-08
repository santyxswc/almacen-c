/**
 * @file PoliticaDescuento.cpp
 * @brief Implementación de las estrategias de descuento.
 * @author Santiago Caicedo
 */

#include "dominio/PoliticaDescuento.hpp"

#include <cmath>
#include <sstream>

#include "dominio/Excepciones.hpp"

namespace almacen::dominio {

Dinero SinDescuento::aplicar(Dinero precio) const {
    return precio;
}

std::string SinDescuento::describir() const {
    return "Sin descuento";
}

std::unique_ptr<PoliticaDescuento> SinDescuento::clonar() const {
    return std::make_unique<SinDescuento>(*this);
}

DescuentoFijo::DescuentoFijo(Dinero monto) : monto_(monto) {}

Dinero DescuentoFijo::aplicar(Dinero precio) const {
    return precio.restarHastaCero(monto_);
}

std::string DescuentoFijo::describir() const {
    return monto_.formatear() + " de descuento";
}

std::unique_ptr<PoliticaDescuento> DescuentoFijo::clonar() const {
    return std::make_unique<DescuentoFijo>(*this);
}

DescuentoPorcentual::DescuentoPorcentual(double porcentaje) : porcentaje_(porcentaje) {
    if (!std::isfinite(porcentaje) || porcentaje < 0.0 || porcentaje > 100.0) {
        throw ValorInvalido("El porcentaje de descuento debe estar entre 0 y 100.");
    }
}

Dinero DescuentoPorcentual::aplicar(Dinero precio) const {
    return precio.restarHastaCero(precio.porcentaje(porcentaje_));
}

std::string DescuentoPorcentual::describir() const {
    std::ostringstream texto;
    texto << porcentaje_ << "% de descuento";
    return texto.str();
}

std::unique_ptr<PoliticaDescuento> DescuentoPorcentual::clonar() const {
    return std::make_unique<DescuentoPorcentual>(*this);
}

}  // namespace almacen::dominio
