/**
 * @file Cliente.hpp
 * @brief Cliente del almacén.
 * @author Santiago Caicedo
 */

#pragma once

#include <string>

#include "dominio/Identificable.hpp"

namespace almacen::dominio {

/**
 * @brief Persona o empresa que hace pedidos.
 *
 * Antes el cliente guardaba punteros a sus facturas y existía una subclase
 * `ClienteConPedido`. Ahora el cliente solo se conoce a sí mismo: sus
 * pedidos se consultan en el repositorio de pedidos, lo que elimina las
 * dependencias circulares entre Cliente, Pedido y Factura.
 */
class Cliente final : public Identificable {
public:
    /**
     * @param id Identificador positivo.
     * @param nombre Nombre no vacío.
     * @throws ValorInvalido si algún dato no es válido.
     */
    Cliente(int id, std::string nombre);

    int id() const noexcept override { return id_; }

    /** @return Nombre del cliente. */
    const std::string& nombre() const noexcept { return nombre_; }

private:
    int id_;
    std::string nombre_;
};

}  // namespace almacen::dominio
