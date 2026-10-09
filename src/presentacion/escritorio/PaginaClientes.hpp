/**
 * @file PaginaClientes.hpp
 * @brief Pantalla para registrar y consultar clientes.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QLineEdit;
class QTableWidget;

namespace almacen::presentacion::escritorio {

/**
 * @brief Lista de clientes con lo que ha comprado cada uno y un formulario para agregar más.
 */
class PaginaClientes final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param listar Caso de uso que lista los clientes.
     * @param crear Caso de uso que registra un cliente.
     * @param resumen Caso de uso con los pedidos y el total de cada cliente.
     * @param padre Widget padre.
     */
    PaginaClientes(const aplicacion::ListarClientes& listar, aplicacion::CrearCliente& crear,
                   const aplicacion::ConsultarResumenCliente& resumen, QWidget* padre = nullptr);

    void refrescar() override;

private:
    void registrar();
    void filtrar();

    const aplicacion::ListarClientes& listar_;
    aplicacion::CrearCliente& crear_;
    const aplicacion::ConsultarResumenCliente& resumen_;

    QLineEdit* nombre_ = nullptr;
    QLineEdit* buscar_ = nullptr;
    QTableWidget* tabla_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
