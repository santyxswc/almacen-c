/**
 * @file PaginaHistorial.hpp
 * @brief Pantalla con todos los pedidos y facturas emitidas.
 * @author Santiago Caicedo
 */

#pragma once

#include <vector>

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QComboBox;
class QLabel;
class QTableWidget;
class QTreeWidget;

namespace almacen::presentacion::escritorio {

/**
 * @brief Historial: pedidos con sus facturas a la izquierda y el detalle de la factura elegida a la derecha.
 */
class PaginaHistorial final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param pedidos Caso de uso que lista los pedidos.
     * @param clientes Caso de uso que lista los clientes (para el filtro y los nombres).
     * @param padre Widget padre.
     */
    PaginaHistorial(const aplicacion::ListarPedidos& pedidos, const aplicacion::ListarClientes& clientes,
                    QWidget* padre = nullptr);

    void refrescar() override;

private:
    void llenarArbol();
    void mostrarDetalle();

    const aplicacion::ListarPedidos& listarPedidos_;
    const aplicacion::ListarClientes& listarClientes_;
    std::vector<aplicacion::PedidoDto> pedidos_;

    QComboBox* filtro_ = nullptr;
    QTreeWidget* arbol_ = nullptr;
    QLabel* tituloDetalle_ = nullptr;
    QTableWidget* lineas_ = nullptr;
    QLabel* ahorro_ = nullptr;
    QLabel* total_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
