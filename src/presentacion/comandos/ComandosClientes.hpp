/**
 * @file ComandosClientes.hpp
 * @brief Opciones del menú para clientes.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "presentacion/comandos/Comando.hpp"

namespace almacen::presentacion {

/** @brief Opción "Crear cliente". */
class ComandoCrearCliente final : public Comando {
public:
    /** @param casoUso Caso de uso que registra el cliente. */
    explicit ComandoCrearCliente(aplicacion::CrearCliente& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Crear cliente"; }
    void ejecutar(Consola& consola) override;

private:
    aplicacion::CrearCliente& casoUso_;
};

/** @brief Opción "Listar clientes". */
class ComandoListarClientes final : public Comando {
public:
    /** @param casoUso Caso de uso que lista los clientes. */
    explicit ComandoListarClientes(const aplicacion::ListarClientes& casoUso) : casoUso_(casoUso) {}
    std::string titulo() const override { return "Listar clientes"; }
    void ejecutar(Consola& consola) override;

private:
    const aplicacion::ListarClientes& casoUso_;
};

}  // namespace almacen::presentacion
