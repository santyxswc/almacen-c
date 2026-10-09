/**
 * @file PaginaVenta.hpp
 * @brief Pantalla para registrar una venta y emitir su factura.
 * @author Santiago Caicedo
 */

#pragma once

#include <vector>

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

namespace almacen::presentacion::escritorio {

/**
 * @brief Venta en tres pasos: elegir el cliente, agregar productos y emitir la factura.
 *
 * El carrito muestra el total con descuentos mientras se arma; la factura
 * real la calcula el caso de uso CrearFactura, que abre un pedido nuevo.
 */
class PaginaVenta final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param listarClientes Para llenar la lista de clientes.
     * @param crearCliente Para registrar un cliente sin salir de la venta.
     * @param listarProductos Para mostrar el catálogo.
     * @param crearFactura Para emitir la factura.
     * @param padre Widget padre.
     */
    PaginaVenta(const aplicacion::ListarClientes& listarClientes, aplicacion::CrearCliente& crearCliente,
                const aplicacion::ListarProductos& listarProductos, aplicacion::CrearFactura& crearFactura,
                QWidget* padre = nullptr);

    void refrescar() override;

    /**
     * @brief Agrega unidades de un producto a la venta en curso.
     * @param productoId Producto del catálogo.
     * @param cantidad Unidades a sumar.
     */
    void agregarAlCarrito(int productoId, int cantidad);

    /** @brief Selecciona un cliente en la lista, si existe. */
    void seleccionarCliente(int clienteId);

private:
    /** @brief Un renglón de la venta en curso. */
    struct Renglon {
        aplicacion::ProductoDto producto;  ///< Producto vendido.
        int cantidad = 0;                  ///< Unidades.
    };

    void llenarClientes(int seleccionar);
    void filtrarCatalogo();
    void agregarSeleccionado();
    void mostrarCarrito();
    void nuevoCliente();
    void emitirFactura();
    void cancelarVenta();

    const aplicacion::ListarClientes& listarClientes_;
    aplicacion::CrearCliente& crearCliente_;
    const aplicacion::ListarProductos& listarProductos_;
    aplicacion::CrearFactura& crearFactura_;

    std::vector<aplicacion::ProductoDto> productos_;
    std::vector<Renglon> carrito_;

    QComboBox* cliente_ = nullptr;
    QLineEdit* buscar_ = nullptr;
    QTableWidget* catalogo_ = nullptr;
    class SelectorCantidad* cantidad_ = nullptr;
    QTableWidget* tablaCarrito_ = nullptr;
    QLabel* subtotal_ = nullptr;
    QLabel* ahorro_ = nullptr;
    QLabel* total_ = nullptr;
    QPushButton* emitir_ = nullptr;
    QPushButton* cancelar_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
