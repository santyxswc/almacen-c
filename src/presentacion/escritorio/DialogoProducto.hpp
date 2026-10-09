/**
 * @file DialogoProducto.hpp
 * @brief Formulario para registrar un producto nuevo.
 * @author Santiago Caicedo
 */

#pragma once

#include <QDialog>

#include "aplicacion/Dtos.hpp"

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QRadioButton;

namespace almacen::presentacion::escritorio {

/**
 * @brief Pide nombre, precio, descuento y almacén, y muestra el precio final mientras se escribe.
 */
class DialogoProducto final : public QDialog {
    Q_OBJECT

public:
    /**
     * @param almacenes Árbol de almacenes para elegir dónde se guarda el producto.
     * @param padre Ventana padre.
     */
    explicit DialogoProducto(const aplicacion::NodoAlmacenDto& almacenes, QWidget* padre = nullptr);

    /** @return Los datos escritos, listos para el caso de uso CrearProducto. */
    aplicacion::SolicitudProducto solicitud() const;

private:
    void actualizarVistaPrevia();

    QLineEdit* nombre_ = nullptr;
    QDoubleSpinBox* precio_ = nullptr;
    QRadioButton* sinDescuento_ = nullptr;
    QRadioButton* descuentoFijo_ = nullptr;
    QRadioButton* descuentoPorcentaje_ = nullptr;
    QDoubleSpinBox* valorDescuento_ = nullptr;
    QComboBox* almacen_ = nullptr;
    QLabel* vistaPrevia_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
