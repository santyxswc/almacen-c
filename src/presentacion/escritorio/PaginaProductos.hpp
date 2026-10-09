/**
 * @file PaginaProductos.hpp
 * @brief Pantalla con el catálogo de productos.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QLineEdit;
class QTableWidget;

namespace almacen::presentacion::escritorio {

/**
 * @brief Catálogo con búsqueda y botón para registrar productos nuevos.
 */
class PaginaProductos final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param listar Caso de uso que lista los productos.
     * @param crear Caso de uso que registra un producto.
     * @param arbol Caso de uso con los almacenes (para elegir dónde guardar).
     * @param padre Widget padre.
     */
    PaginaProductos(const aplicacion::ListarProductos& listar, aplicacion::CrearProducto& crear,
                    const aplicacion::ConsultarArbolAlmacenes& arbol, QWidget* padre = nullptr);

    void refrescar() override;

private:
    void nuevoProducto();
    void filtrar();

    const aplicacion::ListarProductos& listar_;
    aplicacion::CrearProducto& crear_;
    const aplicacion::ConsultarArbolAlmacenes& arbol_;

    QLineEdit* buscar_ = nullptr;
    QTableWidget* tabla_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
