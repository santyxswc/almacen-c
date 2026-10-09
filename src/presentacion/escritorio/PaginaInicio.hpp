/**
 * @file PaginaInicio.hpp
 * @brief Pantalla de inicio con las cifras del negocio y accesos rápidos.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoGenerales.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QLabel;
class QGridLayout;
class QTableWidget;

namespace almacen::presentacion::escritorio {

/**
 * @brief Resumen del día a día: totales, últimas facturas y botones para empezar.
 */
class PaginaInicio final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param resumen Caso de uso con las cifras generales.
     * @param pedidos Caso de uso para listar las últimas facturas.
     * @param clientes Caso de uso para mostrar el nombre de cada cliente.
     * @param padre Widget padre.
     */
    PaginaInicio(const aplicacion::ConsultarResumenGeneral& resumen, const aplicacion::ListarPedidos& pedidos,
                 const aplicacion::ListarClientes& clientes, QWidget* padre = nullptr);

    void refrescar() override;

private:
    QLabel* agregarTarjeta(QGridLayout* rejilla, int columna, const QString& titulo);

    const aplicacion::ConsultarResumenGeneral& resumen_;
    const aplicacion::ListarPedidos& pedidos_;
    const aplicacion::ListarClientes& clientes_;
    QLabel* totalVendido_ = nullptr;
    QLabel* facturas_ = nullptr;
    QLabel* clientesTotal_ = nullptr;
    QLabel* productos_ = nullptr;
    QTableWidget* ultimas_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
