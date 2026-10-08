/**
 * @file Dinero.cpp
 * @brief Implementación del objeto de valor Dinero.
 * @author Santiago Caicedo
 */

#include "dominio/Dinero.hpp"

#include <cmath>
#include <cstdio>

#include "dominio/Excepciones.hpp"

namespace almacen::dominio {

Dinero Dinero::desdeCentavos(std::int64_t centavos) {
    if (centavos < 0) {
        throw ValorInvalido("El dinero no puede ser negativo.");
    }
    return Dinero(centavos);
}

Dinero Dinero::desdeUnidades(double unidades) {
    if (!std::isfinite(unidades) || unidades < 0.0) {
        throw ValorInvalido("El dinero debe ser un numero mayor o igual a cero.");
    }
    return Dinero(static_cast<std::int64_t>(std::llround(unidades * 100.0)));
}

Dinero Dinero::operator+(Dinero otro) const noexcept {
    return Dinero(centavos_ + otro.centavos_);
}

Dinero& Dinero::operator+=(Dinero otro) noexcept {
    centavos_ += otro.centavos_;
    return *this;
}

Dinero Dinero::restarHastaCero(Dinero otro) const noexcept {
    return otro.centavos_ >= centavos_ ? Dinero() : Dinero(centavos_ - otro.centavos_);
}

Dinero Dinero::multiplicar(int cantidad) const {
    if (cantidad < 0) {
        throw ValorInvalido("La cantidad no puede ser negativa.");
    }
    return Dinero(centavos_ * cantidad);
}

Dinero Dinero::porcentaje(double porcentaje) const {
    if (!std::isfinite(porcentaje) || porcentaje < 0.0 || porcentaje > 100.0) {
        throw ValorInvalido("El porcentaje debe estar entre 0 y 100.");
    }
    return Dinero(static_cast<std::int64_t>(std::llround(static_cast<double>(centavos_) * porcentaje / 100.0)));
}

std::string Dinero::formatear() const {
    char texto[32];
    std::snprintf(texto, sizeof texto, "$%lld.%02lld", static_cast<long long>(centavos_ / 100),
                  static_cast<long long>(centavos_ % 100));
    return texto;
}

}  // namespace almacen::dominio
