/**
 * @file RepositoriosEnMemoria.hpp
 * @brief Implementaciones en memoria de los repositorios.
 * @author Santiago Caicedo
 */

#pragma once

#include <map>

#include "aplicacion/puertos/RepositorioAlmacenes.hpp"
#include "aplicacion/puertos/RepositorioClientes.hpp"
#include "aplicacion/puertos/RepositorioPedidos.hpp"

namespace almacen::infraestructura {

/**
 * @brief Guarda los clientes en un `std::map` mientras el programa está abierto.
 *
 * Para usar un archivo o una base de datos basta con escribir otra clase
 * que implemente RepositorioClientes; los casos de uso no cambian.
 */
class RepositorioClientesEnMemoria final : public aplicacion::RepositorioClientes {
public:
    void guardar(const dominio::Cliente& cliente) override;
    std::optional<dominio::Cliente> buscar(int id) const override;
    std::vector<dominio::Cliente> listar() const override;
    int siguienteId() const override;

private:
    std::map<int, dominio::Cliente> clientes_;
};

/**
 * @brief Guarda los pedidos (con sus facturas) en un `std::map`.
 */
class RepositorioPedidosEnMemoria final : public aplicacion::RepositorioPedidos {
public:
    void guardar(const dominio::Pedido& pedido) override;
    std::optional<dominio::Pedido> buscar(int id) const override;
    std::vector<dominio::Pedido> listarPorCliente(int clienteId) const override;
    bool existeFactura(int facturaId) const override;
    std::vector<dominio::Pedido> listar() const override;
    int siguienteId() const override;
    int siguienteFacturaId() const override;

private:
    std::map<int, dominio::Pedido> pedidos_;
};

/**
 * @brief Mantiene en memoria el almacén principal y su árbol.
 */
class RepositorioAlmacenesEnMemoria final : public aplicacion::RepositorioAlmacenes {
public:
    /** @param principal El almacén raíz, que pasa a ser propiedad del repositorio. */
    explicit RepositorioAlmacenesEnMemoria(dominio::Almacen principal);

    dominio::Almacen& principal() override { return principal_; }
    const dominio::Almacen& principal() const override { return principal_; }

private:
    dominio::Almacen principal_;
};

}  // namespace almacen::infraestructura
