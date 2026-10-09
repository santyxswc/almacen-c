/**
 * @file Pagina.hpp
 * @brief Clase base de las pantallas de la aplicación de escritorio.
 * @author Santiago Caicedo
 */

#pragma once

#include <QString>
#include <QWidget>

namespace almacen::presentacion::escritorio {

/**
 * @brief Una pantalla del menú lateral.
 *
 * Cada pantalla recibe por el constructor solo los casos de uso que
 * necesita (segregación de interfaces) y avisa con `datosCambiados()`
 * cuando modifica algo, para que la ventana guarde y las demás pantallas
 * se actualicen (patrón Observer con señales de Qt).
 */
class Pagina : public QWidget {
    Q_OBJECT

public:
    using QWidget::QWidget;

    /** @brief Vuelve a leer los datos y actualiza lo que se muestra. */
    virtual void refrescar() = 0;

signals:
    /** @brief La pantalla modificó datos del negocio. */
    void datosCambiados();

    /** @brief Mensaje corto para la barra de estado. */
    void mensaje(const QString& texto);

    /** @brief Pide a la ventana que muestre otra pantalla. */
    void irA(int pagina);
};

/** @brief Posición de cada pantalla en el menú lateral. */
enum IndicePagina : int {
    PaginaDeInicio = 0,
    PaginaDeVenta,
    PaginaDeHistorial,
    PaginaDeClientes,
    PaginaDeProductos,
    PaginaDeAlmacenes,
};

}  // namespace almacen::presentacion::escritorio
