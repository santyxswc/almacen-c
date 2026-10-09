/**
 * @file SelectorCantidad.hpp
 * @brief Campo de cantidad con botones grandes de menos y más.
 * @author Santiago Caicedo
 */

#pragma once

#include <QWidget>

class QSpinBox;

namespace almacen::presentacion::escritorio {

/**
 * @brief `[ − ] 3 [ + ]`: más fácil de usar con el mouse o una pantalla táctil que las flechas pequeñas.
 */
class SelectorCantidad final : public QWidget {
    Q_OBJECT

public:
    /**
     * @param inicial Cantidad inicial.
     * @param padre Widget padre.
     */
    explicit SelectorCantidad(int inicial = 1, QWidget* padre = nullptr);

    /** @return La cantidad elegida (al menos 1). */
    int valor() const;

    /** @brief Cambia la cantidad sin emitir `valorCambiado`. */
    void ponerValor(int cantidad);

signals:
    /** @brief El usuario cambió la cantidad. */
    void valorCambiado(int valor);

private:
    QSpinBox* campo_ = nullptr;
};

}  // namespace almacen::presentacion::escritorio
