/**
 * @file Producto.hpp
 * @brief Producto que se vende en el almacén.
 * @author Santiago Caicedo
 */

#pragma once

#include <memory>
#include <string>

#include "dominio/Dinero.hpp"
#include "dominio/Identificable.hpp"
#include "dominio/PoliticaDescuento.hpp"

namespace almacen::dominio {

/**
 * @brief Producto con precio base y una política de descuento.
 *
 * Usa composición en lugar de herencia: el descuento es una estrategia
 * intercambiable (ver PoliticaDescuento), así que no hacen falta clases
 * como `ProductoConDescuentoFijo`.
 */
class Producto final : public Identificable {
public:
    /**
     * @brief Crea un producto validando sus datos.
     * @param id Identificador positivo.
     * @param nombre Nombre no vacío.
     * @param precioBase Precio antes de descuentos.
     * @param descuento Estrategia de descuento; si es nula se usa SinDescuento.
     * @throws ValorInvalido si el id o el nombre no son válidos.
     */
    Producto(int id, std::string nombre, Dinero precioBase,
             std::unique_ptr<PoliticaDescuento> descuento = std::make_unique<SinDescuento>());

    /**
     * @brief Copia profunda: la copia tiene su propia estrategia de descuento.
     * @param otro Producto a copiar.
     */
    Producto(const Producto& otro);

    /**
     * @brief Asigna una copia profunda de otro producto.
     * @param otro Producto a copiar.
     * @return Este producto.
     */
    Producto& operator=(const Producto& otro);

    /** @brief Mueve un producto sin copiar su descuento. */
    Producto(Producto&&) noexcept = default;

    /** @brief Mueve un producto sin copiar su descuento. @return Este producto. */
    Producto& operator=(Producto&&) noexcept = default;

    int id() const noexcept override { return id_; }

    /** @return Nombre del producto. */
    const std::string& nombre() const noexcept { return nombre_; }

    /** @return Precio antes de aplicar el descuento. */
    Dinero precioBase() const noexcept { return precioBase_; }

    /** @return Precio de venta, con el descuento aplicado. */
    Dinero precioFinal() const;

    /** @return La estrategia de descuento vigente. */
    const PoliticaDescuento& descuento() const noexcept { return *descuento_; }

    /**
     * @brief Cambia la estrategia de descuento.
     * @param descuento Nueva estrategia; si es nula se usa SinDescuento.
     */
    void cambiarDescuento(std::unique_ptr<PoliticaDescuento> descuento);

private:
    int id_;
    std::string nombre_;
    Dinero precioBase_;
    std::unique_ptr<PoliticaDescuento> descuento_;
};

}  // namespace almacen::dominio
