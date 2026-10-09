/**
 * @file PaginaAlmacenes.hpp
 * @brief Pantalla con el árbol de almacenes y sub-almacenes.
 * @author Santiago Caicedo
 */

#pragma once

#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "presentacion/escritorio/Pagina.hpp"

class QLabel;
class QTableWidget;
class QTreeWidget;
class QTreeWidgetItem;

namespace almacen::presentacion::escritorio {

/**
 * @brief Árbol de almacenes (patrón Composite) a la izquierda y el contenido del elegido a la derecha.
 */
class PaginaAlmacenes final : public Pagina {
    Q_OBJECT

public:
    /**
     * @param arbol Caso de uso con el árbol completo.
     * @param consultar Caso de uso con el contenido de un almacén.
     * @param crearSub Caso de uso que crea sub-almacenes.
     * @param asignar Caso de uso que coloca un producto en un almacén.
     * @param productos Caso de uso que lista el catálogo.
     * @param padre Widget padre.
     */
    PaginaAlmacenes(const aplicacion::ConsultarArbolAlmacenes& arbol, const aplicacion::ConsultarAlmacen& consultar,
                    aplicacion::CrearSubAlmacen& crearSub, aplicacion::AsignarProductoAAlmacen& asignar,
                    const aplicacion::ListarProductos& productos, QWidget* padre = nullptr);

    void refrescar() override;

private:
    void agregarNodo(QTreeWidgetItem* padre, const aplicacion::NodoAlmacenDto& nodo);
    void mostrarSeleccionado();
    int almacenSeleccionado() const;
    void nuevoSubAlmacen();
    void agregarProducto();

    const aplicacion::ConsultarArbolAlmacenes& arbol_;
    const aplicacion::ConsultarAlmacen& consultar_;
    aplicacion::CrearSubAlmacen& crearSub_;
    aplicacion::AsignarProductoAAlmacen& asignar_;
    const aplicacion::ListarProductos& productos_;

    QTreeWidget* vistaArbol_ = nullptr;
    QLabel* nombre_ = nullptr;
    QLabel* resumen_ = nullptr;
    QTableWidget* tabla_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
