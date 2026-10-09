/**
 * @file DialogoProducto.cpp
 * @brief Implementación del formulario de producto nuevo.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/DialogoProducto.hpp"

#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "dominio/PoliticaDescuento.hpp"
#include "presentacion/escritorio/Comun.hpp"

namespace almacen::presentacion::escritorio {

namespace {

void agregarAlmacenes(QComboBox* combo, const aplicacion::NodoAlmacenDto& nodo, int nivel) {
    combo->addItem(QString(nivel * 4, QLatin1Char(' ')) + aQString(nodo.nombre), nodo.id);
    for (const auto& hijo : nodo.hijos) {
        agregarAlmacenes(combo, hijo, nivel + 1);
    }
}

}  // namespace

DialogoProducto::DialogoProducto(const aplicacion::NodoAlmacenDto& almacenes, QWidget* padre) : QDialog(padre) {
    setWindowTitle(QStringLiteral("Producto nuevo"));
    setMinimumWidth(600);
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(24, 20, 24, 20);
    raiz->setSpacing(14);
    raiz->addWidget(crearTitulo(QStringLiteral("Producto nuevo")));

    auto* formulario = new QFormLayout;
    formulario->setSpacing(12);
    nombre_ = new QLineEdit;
    nombre_->setObjectName(QStringLiteral("nombreProducto"));
    nombre_->setPlaceholderText(QStringLiteral("Por ejemplo: Camisa blanca talla M"));
    formulario->addRow(QStringLiteral("Nombre"), nombre_);

    precio_ = new QDoubleSpinBox;
    precio_->setPrefix(QStringLiteral("$ "));
    precio_->setRange(0, 1'000'000'000);
    precio_->setDecimals(2);
    precio_->setGroupSeparatorShown(true);
    precio_->setSingleStep(1000);
    formulario->addRow(QStringLiteral("Precio de venta"), precio_);

    auto* opciones = new QHBoxLayout;
    sinDescuento_ = new QRadioButton(QStringLiteral("Sin descuento"));
    descuentoFijo_ = new QRadioButton(QStringLiteral("Monto fijo"));
    descuentoPorcentaje_ = new QRadioButton(QStringLiteral("Porcentaje"));
    sinDescuento_->setChecked(true);
    auto* grupo = new QButtonGroup(this);
    for (auto* opcion : {sinDescuento_, descuentoFijo_, descuentoPorcentaje_}) {
        grupo->addButton(opcion);
        opciones->addWidget(opcion);
    }
    opciones->addStretch();
    formulario->addRow(QStringLiteral("Descuento"), opciones);

    valorDescuento_ = new QDoubleSpinBox;
    valorDescuento_->setRange(0, 1'000'000'000);
    valorDescuento_->setDecimals(2);
    valorDescuento_->setEnabled(false);
    formulario->addRow(QStringLiteral("Valor del descuento"), valorDescuento_);

    almacen_ = new QComboBox;
    agregarAlmacenes(almacen_, almacenes, 0);
    formulario->addRow(QStringLiteral("Guardar en"), almacen_);
    raiz->addLayout(formulario);

    vistaPrevia_ = new QLabel;
    vistaPrevia_->setObjectName(QStringLiteral("ahorro"));
    raiz->addWidget(vistaPrevia_);

    auto* botones = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    botones->button(QDialogButtonBox::Save)->setText(QStringLiteral("Guardar producto"));
    botones->button(QDialogButtonBox::Save)->setObjectName(QStringLiteral("principal"));
    botones->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Cancelar"));
    raiz->addWidget(botones);
    connect(botones, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(botones, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(grupo, &QButtonGroup::buttonToggled, this, [this] {
        valorDescuento_->setEnabled(!sinDescuento_->isChecked());
        if (descuentoPorcentaje_->isChecked()) {
            valorDescuento_->setPrefix(QString());
            valorDescuento_->setSuffix(QStringLiteral(" %"));
            valorDescuento_->setMaximum(100);
        } else {
            valorDescuento_->setPrefix(QStringLiteral("$ "));
            valorDescuento_->setSuffix(QString());
            valorDescuento_->setMaximum(1'000'000'000);
        }
        actualizarVistaPrevia();
    });
    connect(precio_, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this] { actualizarVistaPrevia(); });
    connect(valorDescuento_, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
            [this] { actualizarVistaPrevia(); });
    actualizarVistaPrevia();
}

aplicacion::SolicitudProducto DialogoProducto::solicitud() const {
    aplicacion::SolicitudProducto datos;
    datos.nombre = aStd(nombre_->text());
    datos.precio = dominio::Dinero::desdeUnidades(precio_->value());
    datos.tipoDescuento = descuentoFijo_->isChecked()         ? aplicacion::TipoDescuento::Fijo
                          : descuentoPorcentaje_->isChecked() ? aplicacion::TipoDescuento::Porcentual
                                                              : aplicacion::TipoDescuento::Ninguno;
    datos.valorDescuento = valorDescuento_->value();
    datos.almacenId = almacen_->currentData().toInt();
    return datos;
}

void DialogoProducto::actualizarVistaPrevia() {
    const auto precio = dominio::Dinero::desdeUnidades(precio_->value());
    dominio::Dinero final = precio;
    if (descuentoFijo_->isChecked()) {
        final = dominio::DescuentoFijo(dominio::Dinero::desdeUnidades(valorDescuento_->value())).aplicar(precio);
    } else if (descuentoPorcentaje_->isChecked()) {
        final = dominio::DescuentoPorcentual(valorDescuento_->value()).aplicar(precio);
    }
    vistaPrevia_->setText(QStringLiteral("El cliente pagará %1 por unidad.").arg(dinero(final)));
}

}  // namespace almacen::presentacion::escritorio
