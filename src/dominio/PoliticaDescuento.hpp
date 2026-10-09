/**
 * @file PoliticaDescuento.hpp
 * @brief Estrategias de descuento que se pueden asignar a un producto.
 * @author Santiago Caicedo
 */

#pragma once

#include <memory>
#include <string>

#include "dominio/Dinero.hpp"

namespace almacen::dominio {

/**
 * @brief Patrón Strategy: cómo se calcula el precio final de un producto.
 *
 * En la versión anterior cada tipo de descuento era una subclase de
 * Producto. Separarlo en una estrategia permite agregar descuentos nuevos
 * sin tocar Producto (principio abierto/cerrado) y cambiar el descuento de
 * un producto en tiempo de ejecución.
 */
class PoliticaDescuento {
public:
    virtual ~PoliticaDescuento() = default;

    /**
     * @brief Aplica el descuento a un precio.
     * @param precio Precio base del producto.
     * @return Precio con el descuento aplicado; nunca negativo.
     */
    virtual Dinero aplicar(Dinero precio) const = 0;

    /** @return Descripción corta para mostrar al usuario, por ejemplo `20% de descuento`. */
    virtual std::string describir() const = 0;

    /** @return Una copia independiente de la estrategia (patrón Prototype). */
    virtual std::unique_ptr<PoliticaDescuento> clonar() const = 0;
};

/**
 * @brief Objeto nulo: el producto se vende a su precio base.
 */
class SinDescuento final : public PoliticaDescuento {
public:
    Dinero aplicar(Dinero precio) const override;
    std::string describir() const override;
    std::unique_ptr<PoliticaDescuento> clonar() const override;
};

/**
 * @brief Resta un monto fijo al precio (sin bajar de cero).
 */
class DescuentoFijo final : public PoliticaDescuento {
public:
    /** @param monto Cantidad que se descuenta de cada unidad. */
    explicit DescuentoFijo(Dinero monto);

    Dinero aplicar(Dinero precio) const override;
    std::string describir() const override;
    std::unique_ptr<PoliticaDescuento> clonar() const override;

    /** @return Cantidad que se descuenta de cada unidad. */
    Dinero monto() const noexcept { return monto_; }

private:
    Dinero monto_;
};

/**
 * @brief Descuenta un porcentaje del precio.
 */
class DescuentoPorcentual final : public PoliticaDescuento {
public:
    /**
     * @param porcentaje Valor entre 0 y 100.
     * @throws ValorInvalido si el porcentaje está fuera de rango.
     */
    explicit DescuentoPorcentual(double porcentaje);

    Dinero aplicar(Dinero precio) const override;
    std::string describir() const override;
    std::unique_ptr<PoliticaDescuento> clonar() const override;

    /** @return Porcentaje que se descuenta, entre 0 y 100. */
    double porcentaje() const noexcept { return porcentaje_; }

private:
    double porcentaje_;
};

}  // namespace almacen::dominio
