/**
 * @file VentanaPrincipal.cpp
 * @brief Implementación de la ventana principal.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/VentanaPrincipal.hpp"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <utility>

namespace almacen::presentacion::escritorio {

VentanaPrincipal::VentanaPrincipal(std::function<QString()> guardar, QWidget* padre)
    : QMainWindow(padre), guardar_(std::move(guardar)) {
    setWindowTitle(QStringLiteral("Sistema de Almacén"));
    resize(1280, 800);
    setMinimumSize(1024, 680);

    auto* central = new QWidget;
    auto* fila = new QHBoxLayout(central);
    fila->setContentsMargins(0, 0, 0, 0);
    fila->setSpacing(0);

    auto* menu = new QWidget;
    menu->setObjectName(QStringLiteral("menu"));
    menu->setFixedWidth(230);
    menu_ = new QVBoxLayout(menu);
    menu_->setContentsMargins(0, 0, 0, 16);
    menu_->setSpacing(0);
    auto* marca = new QLabel(QStringLiteral("Almacén"));
    marca->setObjectName(QStringLiteral("marca"));
    auto* submarca = new QLabel(QStringLiteral("Ventas e inventario"));
    submarca->setObjectName(QStringLiteral("submarca"));
    menu_->addWidget(marca);
    menu_->addWidget(submarca);
    menu_->addStretch();
    opciones_ = new QButtonGroup(this);
    opciones_->setExclusive(true);

    pila_ = new QStackedWidget;
    pila_->setObjectName(QStringLiteral("contenido"));
    fila->addWidget(menu);
    fila->addWidget(pila_, 1);
    setCentralWidget(central);
    statusBar()->showMessage(QStringLiteral("Listo. Los cambios se guardan automáticamente."));

    connect(opciones_, &QButtonGroup::idClicked, this, &VentanaPrincipal::mostrarPagina);
}

void VentanaPrincipal::agregarPagina(const QString& titulo, Pagina* pagina) {
    const int indice = static_cast<int>(paginas_.size());
    paginas_.push_back(pagina);
    pila_->addWidget(pagina);

    auto* boton = new QPushButton(titulo);
    boton->setObjectName(QStringLiteral("opcionMenu"));
    boton->setCheckable(true);
    boton->setCursor(Qt::PointingHandCursor);
    opciones_->addButton(boton, indice);
    menu_->insertWidget(menu_->count() - 1, boton);

    connect(pagina, &Pagina::datosCambiados, this, &VentanaPrincipal::alCambiarDatos);
    connect(pagina, &Pagina::mensaje, this, [this](const QString& texto) { statusBar()->showMessage(texto, 8000); });
    connect(pagina, &Pagina::irA, this, &VentanaPrincipal::mostrarPagina);
}

void VentanaPrincipal::mostrarPagina(int indice) {
    if (indice < 0 || indice >= static_cast<int>(paginas_.size())) {
        return;
    }
    opciones_->button(indice)->setChecked(true);
    paginas_[static_cast<std::size_t>(indice)]->refrescar();
    pila_->setCurrentIndex(indice);
}

void VentanaPrincipal::refrescarTodo() {
    for (auto* pagina : paginas_) {
        pagina->refrescar();
    }
}

void VentanaPrincipal::alCambiarDatos() {
    const QString error = guardar_();
    if (!error.isEmpty()) {
        QMessageBox::critical(this, QStringLiteral("No se pudo guardar"),
                              error + QStringLiteral("\n\nLos datos siguen en pantalla. Revise que la carpeta de "
                                                     "datos exista y tenga permisos de escritura."));
    }
    refrescarTodo();
}

}  // namespace almacen::presentacion::escritorio
