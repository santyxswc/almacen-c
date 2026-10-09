/**
 * @file SelectorCantidad.cpp
 * @brief Implementación del selector de cantidad.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/SelectorCantidad.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>

namespace almacen::presentacion::escritorio {

SelectorCantidad::SelectorCantidad(int inicial, QWidget* padre) : QWidget(padre) {
    auto* fila = new QHBoxLayout(this);
    fila->setContentsMargins(0, 0, 0, 0);
    fila->setSpacing(0);
    auto* menos = new QPushButton(QStringLiteral("−"));
    menos->setObjectName(QStringLiteral("pasoMenos"));
    auto* mas = new QPushButton(QStringLiteral("+"));
    mas->setObjectName(QStringLiteral("pasoMas"));
    for (auto* boton : {menos, mas}) {
        boton->setCursor(Qt::PointingHandCursor);
        boton->setFocusPolicy(Qt::NoFocus);
        boton->setAutoRepeat(true);
    }
    campo_ = new QSpinBox;
    campo_->setObjectName(QStringLiteral("cantidad"));
    campo_->setRange(1, 9999);
    campo_->setValue(inicial);
    campo_->setFixedWidth(56);
    campo_->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fila->addWidget(menos);
    fila->addWidget(campo_);
    fila->addWidget(mas);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(menos, &QPushButton::clicked, campo_, &QSpinBox::stepDown);
    connect(mas, &QPushButton::clicked, campo_, &QSpinBox::stepUp);
    connect(campo_, qOverload<int>(&QSpinBox::valueChanged), this, &SelectorCantidad::valorCambiado);
}

int SelectorCantidad::valor() const {
    return campo_->value();
}

void SelectorCantidad::ponerValor(int cantidad) {
    const QSignalBlocker bloqueo(campo_);
    campo_->setValue(cantidad);
}

}  // namespace almacen::presentacion::escritorio
