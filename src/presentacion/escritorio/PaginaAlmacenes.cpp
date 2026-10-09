/**
 * @file PaginaAlmacenes.cpp
 * @brief Implementación de la pantalla de almacenes.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaAlmacenes.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "presentacion/escritorio/Comun.hpp"

namespace almacen::presentacion::escritorio {

PaginaAlmacenes::PaginaAlmacenes(const aplicacion::ConsultarArbolAlmacenes& arbol,
                                 const aplicacion::ConsultarAlmacen& consultar, aplicacion::CrearSubAlmacen& crearSub,
                                 aplicacion::AsignarProductoAAlmacen& asignar,
                                 const aplicacion::ListarProductos& productos, QWidget* padre)
    : Pagina(padre),
      arbol_(arbol),
      consultar_(consultar),
      crearSub_(crearSub),
      asignar_(asignar),
      productos_(productos) {
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(32, 28, 32, 28);
    raiz->setSpacing(16);
    raiz->addWidget(crearTitulo(QStringLiteral("Almacenes")));
    raiz->addWidget(
        crearAyuda(QStringLiteral("Organice la mercancía en bodegas, estantes o secciones. Un "
                                  "sub-almacén puede tener otros sub-almacenes dentro.")));

    auto* cuerpo = new QHBoxLayout;
    cuerpo->setSpacing(20);
    auto* izquierda = new QVBoxLayout;
    vistaArbol_ = new QTreeWidget;
    vistaArbol_->setHeaderLabels({QStringLiteral("Almacén"), QStringLiteral("Productos")});
    vistaArbol_->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    vistaArbol_->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    vistaArbol_->headerItem()->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
    izquierda->addWidget(vistaArbol_, 1);
    auto* nuevoSub = new QPushButton(QStringLiteral("+ Sub-almacén dentro del seleccionado"));
    nuevoSub->setCursor(Qt::PointingHandCursor);
    izquierda->addWidget(nuevoSub);
    cuerpo->addLayout(izquierda, 4);

    auto* panel = new QFrame;
    panel->setObjectName(QStringLiteral("tarjeta"));
    auto* derecha = new QVBoxLayout(panel);
    derecha->setContentsMargins(18, 16, 18, 18);
    derecha->setSpacing(8);
    nombre_ = new QLabel;
    nombre_->setObjectName(QStringLiteral("seccion"));
    resumen_ = crearAyuda(QString());
    derecha->addWidget(nombre_);
    derecha->addWidget(resumen_);
    tabla_ = new QTableWidget;
    configurarTabla(tabla_, {QStringLiteral("Producto"), QStringLiteral("Precio final")});
    alinearColumnasNumericas(tabla_, {1});
    derecha->addWidget(tabla_, 1);
    auto* agregar = crearBotonPrincipal(QStringLiteral("Guardar un producto aquí"));
    derecha->addWidget(agregar);
    cuerpo->addWidget(panel, 5);
    raiz->addLayout(cuerpo, 1);

    connect(vistaArbol_, &QTreeWidget::currentItemChanged, this, [this] { mostrarSeleccionado(); });
    connect(nuevoSub, &QPushButton::clicked, this, &PaginaAlmacenes::nuevoSubAlmacen);
    connect(agregar, &QPushButton::clicked, this, &PaginaAlmacenes::agregarProducto);
}

void PaginaAlmacenes::refrescar() {
    const int seleccionado = almacenSeleccionado();
    vistaArbol_->clear();
    agregarNodo(nullptr, arbol_.ejecutar());
    vistaArbol_->expandAll();
    const auto coincidencias = vistaArbol_->findItems(QStringLiteral("*"), Qt::MatchWildcard | Qt::MatchRecursive);
    QTreeWidgetItem* elegir = vistaArbol_->topLevelItem(0);
    for (auto* item : coincidencias) {
        if (item->data(0, Qt::UserRole).toInt() == seleccionado) {
            elegir = item;
        }
    }
    vistaArbol_->setCurrentItem(elegir);
    mostrarSeleccionado();
}

void PaginaAlmacenes::agregarNodo(QTreeWidgetItem* padre, const aplicacion::NodoAlmacenDto& nodo) {
    const QStringList columnas{aQString(nodo.nombre), QString::number(nodo.totalProductos)};
    auto* item = padre == nullptr ? new QTreeWidgetItem(vistaArbol_, columnas) : new QTreeWidgetItem(padre, columnas);
    item->setData(0, Qt::UserRole, nodo.id);
    item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
    for (const auto& hijo : nodo.hijos) {
        agregarNodo(item, hijo);
    }
}

int PaginaAlmacenes::almacenSeleccionado() const {
    const QTreeWidgetItem* item = vistaArbol_->currentItem();
    return item == nullptr ? 0 : item->data(0, Qt::UserRole).toInt();
}

void PaginaAlmacenes::mostrarSeleccionado() {
    const int id = almacenSeleccionado();
    tabla_->setRowCount(0);
    if (id == 0) {
        return;
    }
    ejecutarSeguro(this, [&] {
        const auto almacen = consultar_.ejecutar(id);
        nombre_->setText(aQString(almacen.nombre));
        resumen_->setText(QStringLiteral("%1 producto(s) guardado(s) aquí · %2 en total contando sus sub-almacenes")
                              .arg(almacen.productos.size())
                              .arg(almacen.totalProductos));
        tabla_->setRowCount(static_cast<int>(almacen.productos.size()));
        for (int i = 0; i < static_cast<int>(almacen.productos.size()); ++i) {
            const auto& producto = almacen.productos[static_cast<std::size_t>(i)];
            ponerCelda(tabla_, i, 0, aQString(producto.nombre));
            ponerCelda(tabla_, i, 1, dinero(producto.precioFinal), true);
        }
    });
}

void PaginaAlmacenes::nuevoSubAlmacen() {
    const int padre = almacenSeleccionado();
    const QString nombrePadre = vistaArbol_->currentItem() ? vistaArbol_->currentItem()->text(0) : QString();
    bool aceptado = false;
    const QString nombre =
        QInputDialog::getText(this, QStringLiteral("Sub-almacén nuevo"),
                              QStringLiteral("Nombre del sub-almacén dentro de «%1»:").arg(nombrePadre),
                              QLineEdit::Normal, QString(), &aceptado);
    if (!aceptado) {
        return;
    }
    ejecutarSeguro(this, [&] {
        const auto creado = crearSub_.ejecutar(0, aStd(nombre), padre);
        emit datosCambiados();
        emit mensaje(QStringLiteral("Sub-almacén «%1» creado.").arg(aQString(creado.nombre)));
    });
}

void PaginaAlmacenes::agregarProducto() {
    const int destino = almacenSeleccionado();
    const auto productos = productos_.ejecutar();
    QStringList nombres;
    for (const auto& producto : productos) {
        nombres << aQString(producto.nombre);
    }
    bool aceptado = false;
    const QString elegido = QInputDialog::getItem(this, QStringLiteral("Guardar producto"),
                                                  QStringLiteral("¿Qué producto quiere guardar en este almacén?"),
                                                  nombres, 0, false, &aceptado);
    if (!aceptado) {
        return;
    }
    const auto indice = nombres.indexOf(elegido);
    if (indice < 0) {
        return;
    }
    ejecutarSeguro(this, [&] {
        asignar_.ejecutar(productos[static_cast<std::size_t>(indice)].id, destino);
        emit datosCambiados();
        emit mensaje(QStringLiteral("«%1» guardado en este almacén.").arg(elegido));
    });
}

}  // namespace almacen::presentacion::escritorio
