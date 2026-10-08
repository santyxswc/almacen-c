/**
 * @file Dinero.hpp
 * @brief Objeto de valor para manejar dinero sin errores de redondeo.
 * @author Santiago Caicedo
 */

#pragma once

#include <cstdint>
#include <string>

namespace almacen::dominio {

/**
 * @brief Cantidad de dinero exacta, guardada en centavos.
 *
 * Usar `float` para dinero acumula errores (0.1 + 0.2 != 0.3). Este
 * objeto de valor guarda enteros y es inmutable: cada operación devuelve
 * un valor nuevo. Nunca es negativo.
 */
class Dinero {
public:
    /** @brief Cero pesos. */
    constexpr Dinero() noexcept = default;

    /**
     * @brief Crea un valor a partir de centavos.
     * @param centavos Cantidad en centavos, mayor o igual a cero.
     * @throws ValorInvalido si la cantidad es negativa.
     */
    static Dinero desdeCentavos(std::int64_t centavos);

    /**
     * @brief Crea un valor a partir de unidades (pesos), redondeando al centavo.
     * @param unidades Cantidad en pesos, mayor o igual a cero.
     * @throws ValorInvalido si la cantidad es negativa o no es un número.
     */
    static Dinero desdeUnidades(double unidades);

    /** @return La cantidad en centavos. */
    std::int64_t centavos() const noexcept { return centavos_; }

    /** @return true si el valor es cero. */
    bool esCero() const noexcept { return centavos_ == 0; }

    /** @brief Suma dos cantidades. */
    Dinero operator+(Dinero otro) const noexcept;

    /** @brief Suma otra cantidad a esta. */
    Dinero& operator+=(Dinero otro) noexcept;

    /**
     * @brief Resta sin bajar de cero.
     * @param otro Cantidad a restar.
     * @return La diferencia, o cero si `otro` es mayor.
     */
    Dinero restarHastaCero(Dinero otro) const noexcept;

    /**
     * @brief Multiplica por una cantidad de unidades.
     * @param cantidad Número de unidades, mayor o igual a cero.
     * @throws ValorInvalido si la cantidad es negativa.
     */
    Dinero multiplicar(int cantidad) const;

    /**
     * @brief Calcula un porcentaje de esta cantidad, redondeado al centavo.
     * @param porcentaje Valor entre 0 y 100.
     * @throws ValorInvalido si el porcentaje está fuera de rango.
     */
    Dinero porcentaje(double porcentaje) const;

    /** @return Texto legible, por ejemplo `$1250.50`. */
    std::string formatear() const;

    bool operator==(Dinero otro) const noexcept { return centavos_ == otro.centavos_; }
    bool operator!=(Dinero otro) const noexcept { return centavos_ != otro.centavos_; }
    bool operator<(Dinero otro) const noexcept { return centavos_ < otro.centavos_; }
    bool operator>(Dinero otro) const noexcept { return centavos_ > otro.centavos_; }

private:
    explicit constexpr Dinero(std::int64_t centavos) noexcept : centavos_(centavos) {}

    std::int64_t centavos_ = 0;
};

}  // namespace almacen::dominio
