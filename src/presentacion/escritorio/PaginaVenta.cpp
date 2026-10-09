/**
 * @file PaginaVenta.cpp
 * @brief Implementación de la pantalla de venta.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaVenta.hpp"

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <algorithm>

#include "presentacion/escritorio/Comun.hpp"
#include "presentacion/escritorio/SelectorCantidad.hpp"

namespace almacen::presentacion::escritorio {

namespace {

QLabel* crearSeccion(const QString& texto) {
    auto* etiqueta = new QLabel(texto);
    etiqueta->setObjectName(QStringLiteral("seccion"));
    return etiqueta;
}

}  // namespace

PaginaVenta::PaginaVenta(const aplicacion::ListarClientes& listarClientes, aplicacion::CrearCliente& crearCliente,
                         const aplicacion::ListarProductos& listarProductos, aplicacion::CrearFactura& crearFactura,
                         QWidget* padre)
    : Pagina(padre),
      listarClientes_(listarClientes),
      crearCliente_(crearCliente),
      listarProductos_(listarProductos),
      crearFactura_(crearFactura) {
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(32, 28, 32, 28);
    raiz->setSpacing(16);
    raiz->addWidget(crearTitulo(QStringLiteral("Nueva venta")));
    raiz->addWidget(crearAyuda(
        QStringLiteral("1. Elija el cliente.  2. Busque los productos y agréguelos.  3. Pulse «Emitir factura».")));

    // Paso 1: cliente
    auto* filaCliente = new QHBoxLayout;
    filaCliente->setSpacing(10);
    filaCliente->addWidget(crearSeccion(QStringLiteral("Cliente")));
    cliente_ = new QComboBox;
    cliente_->setMinimumWidth(320);
    cliente_->setPlaceholderText(QStringLiteral("Seleccione un cliente"));
    auto* botonCliente = new QPushButton(QStringLiteral("+ Cliente nuevo"));
    botonCliente->setCursor(Qt::PointingHandCursor);
    filaCliente->addWidget(cliente_);
    filaCliente->addWidget(botonCliente);
    filaCliente->addStretch();
    raiz->addLayout(filaCliente);
    connect(botonCliente, &QPushButton::clicked, this, &PaginaVenta::nuevoCliente);

    auto* cuerpo = new QHBoxLayout;
    cuerpo->setSpacing(20);

    // Paso 2: catálogo
    auto* izquierda = new QVBoxLayout;
    izquierda->setSpacing(10);
    izquierda->addWidget(crearSeccion(QStringLiteral("Productos")));
    buscar_ = new QLineEdit;
    buscar_->setPlaceholderText(QStringLiteral("Buscar por nombre…"));
    buscar_->setClearButtonEnabled(true);
    izquierda->addWidget(buscar_);
    catalogo_ = new QTableWidget;
    configurarTabla(catalogo_, {QStringLiteral("Producto"), QStringLiteral("Precio"), QStringLiteral("Descuento"),
                                QStringLiteral("Precio final")});
    alinearColumnasNumericas(catalogo_, {1, 2, 3});
    izquierda->addWidget(catalogo_, 1);
    auto* filaAgregar = new QHBoxLayout;
    filaAgregar->addWidget(new QLabel(QStringLiteral("Cantidad")));
    cantidad_ = new SelectorCantidad(1);
    filaAgregar->addWidget(cantidad_);
    auto* agregar = crearBotonPrincipal(QStringLiteral("Agregar a la venta"));
    filaAgregar->addWidget(agregar);
    filaAgregar->addStretch();
    izquierda->addLayout(filaAgregar);
    izquierda->addWidget(crearAyuda(QStringLiteral("Consejo: doble clic en un producto lo agrega directamente.")));
    cuerpo->addLayout(izquierda, 11);

    connect(buscar_, &QLineEdit::textChanged, this, &PaginaVenta::filtrarCatalogo);
    connect(agregar, &QPushButton::clicked, this, &PaginaVenta::agregarSeleccionado);
    connect(catalogo_, &QTableWidget::cellDoubleClicked, this, [this](int, int) { agregarSeleccionado(); });
    connect(buscar_, &QLineEdit::returnPressed, this, &PaginaVenta::agregarSeleccionado);

    // Paso 3: venta en curso
    auto* panel = new QFrame;
    panel->setObjectName(QStringLiteral("tarjeta"));
    auto* derecha = new QVBoxLayout(panel);
    derecha->setContentsMargins(18, 16, 18, 18);
    derecha->setSpacing(10);
    derecha->addWidget(crearSeccion(QStringLiteral("Venta actual")));
    tablaCarrito_ = new QTableWidget;
    configurarTabla(tablaCarrito_,
                    {QStringLiteral("Producto"), QStringLiteral("Cantidad"), QStringLiteral("Subtotal"), QString()});
    alinearColumnasNumericas(tablaCarrito_, {2});
    tablaCarrito_->setSelectionMode(QAbstractItemView::NoSelection);
    tablaCarrito_->verticalHeader()->setDefaultSectionSize(56);
    tablaCarrito_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tablaCarrito_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    tablaCarrito_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tablaCarrito_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    tablaCarrito_->horizontalHeader()->setStretchLastSection(false);
    tablaCarrito_->setColumnWidth(1, 132);
    tablaCarrito_->setColumnWidth(3, 84);
    tablaCarrito_->setWordWrap(true);
    derecha->addWidget(tablaCarrito_, 1);

    auto* totales = new QVBoxLayout;
    totales->setSpacing(4);
    subtotal_ = new QLabel;
    ahorro_ = new QLabel;
    ahorro_->setObjectName(QStringLiteral("ahorro"));
    auto* etiquetaTotal = new QLabel(QStringLiteral("Total a pagar"));
    etiquetaTotal->setObjectName(QStringLiteral("tarjetaTitulo"));
    total_ = new QLabel;
    total_->setObjectName(QStringLiteral("totalGrande"));
    totales->addWidget(subtotal_);
    totales->addWidget(ahorro_);
    totales->addSpacing(6);
    totales->addWidget(etiquetaTotal);
    totales->addWidget(total_);
    derecha->addLayout(totales);

    emitir_ = crearBotonPrincipal(QStringLiteral("Emitir factura"));
    emitir_->setMinimumHeight(48);
    cancelar_ = new QPushButton(QStringLiteral("Cancelar venta"));
    cancelar_->setObjectName(QStringLiteral("peligro"));
    derecha->addWidget(emitir_);
    derecha->addWidget(cancelar_);
    cuerpo->addWidget(panel, 10);
    raiz->addLayout(cuerpo, 1);

    connect(emitir_, &QPushButton::clicked, this, &PaginaVenta::emitirFactura);
    connect(cancelar_, &QPushButton::clicked, this, &PaginaVenta::cancelarVenta);
    mostrarCarrito();
}

void PaginaVenta::refrescar() {
    const int seleccionado = cliente_->currentData().isValid() ? cliente_->currentData().toInt() : 0;
    llenarClientes(seleccionado);
    productos_ = listarProductos_.ejecutar();
    filtrarCatalogo();
}

void PaginaVenta::llenarClientes(int seleccionar) {
    cliente_->clear();
    for (const auto& cliente : listarClientes_.ejecutar()) {
        cliente_->addItem(aQString(cliente.nombre), cliente.id);
    }
    cliente_->setCurrentIndex(seleccionar == 0 ? -1 : cliente_->findData(seleccionar));
}

void PaginaVenta::seleccionarCliente(int clienteId) {
    cliente_->setCurrentIndex(cliente_->findData(clienteId));
}

void PaginaVenta::filtrarCatalogo() {
    const QString filtro = buscar_->text().trimmed();
    catalogo_->setRowCount(0);
    for (const auto& producto : productos_) {
        const QString nombre = aQString(producto.nombre);
        if (!filtro.isEmpty() && !nombre.contains(filtro, Qt::CaseInsensitive)) {
            continue;
        }
        const int fila = catalogo_->rowCount();
        catalogo_->insertRow(fila);
        ponerCelda(catalogo_, fila, 0, nombre);
        catalogo_->item(fila, 0)->setData(Qt::UserRole, producto.id);
        ponerCelda(catalogo_, fila, 1, dinero(producto.precioBase), true);
        ponerCelda(catalogo_, fila, 2, textoDescuento(producto.precioBase, producto.precioFinal), true);
        ponerCelda(catalogo_, fila, 3, dinero(producto.precioFinal), true);
    }
    if (catalogo_->rowCount() > 0) {
        catalogo_->selectRow(0);
    }
}

void PaginaVenta::agregarSeleccionado() {
    const int fila = catalogo_->currentRow();
    if (fila < 0) {
        emit mensaje(QStringLiteral("Seleccione un producto de la lista."));
        return;
    }
    agregarAlCarrito(catalogo_->item(fila, 0)->data(Qt::UserRole).toInt(), cantidad_->valor());
    cantidad_->ponerValor(1);
}

void PaginaVenta::agregarAlCarrito(int productoId, int cantidad) {
    const auto producto =
        std::find_if(productos_.begin(), productos_.end(), [productoId](const auto& p) { return p.id == productoId; });
    if (producto == productos_.end() || cantidad <= 0) {
        return;
    }
    auto renglon = std::find_if(carrito_.begin(), carrito_.end(),
                                [productoId](const Renglon& r) { return r.producto.id == productoId; });
    if (renglon != carrito_.end()) {
        renglon->cantidad += cantidad;
    } else {
        carrito_.push_back({*producto, cantidad});
    }
    mostrarCarrito();
    emit mensaje(QStringLiteral("Agregado: %1 × %2").arg(cantidad).arg(aQString(producto->nombre)));
}

void PaginaVenta::mostrarCarrito() {
    tablaCarrito_->setRowCount(static_cast<int>(carrito_.size()));
    dominio::Dinero lista;
    dominio::Dinero total;
    for (int i = 0; i < static_cast<int>(carrito_.size()); ++i) {
        const Renglon& renglon = carrito_[static_cast<std::size_t>(i)];
        const auto subtotal = renglon.producto.precioFinal.multiplicar(renglon.cantidad);
        lista += renglon.producto.precioBase.multiplicar(renglon.cantidad);
        total += subtotal;

        ponerCelda(tablaCarrito_, i, 0,
                   aQString(renglon.producto.nombre) + QStringLiteral("\n") + dinero(renglon.producto.precioFinal) +
                       QStringLiteral(" c/u"));
        auto* cantidad = new SelectorCantidad(renglon.cantidad);
        auto* celda = new QWidget;
        auto* centrado = new QHBoxLayout(celda);
        centrado->setContentsMargins(4, 0, 4, 0);
        centrado->addWidget(cantidad);
        const int productoId = renglon.producto.id;
        connect(cantidad, &SelectorCantidad::valorCambiado, this, [this, productoId](int valor) {
            for (auto& r : carrito_) {
                if (r.producto.id == productoId) {
                    r.cantidad = valor;
                }
            }
            QMetaObject::invokeMethod(this, [this] { mostrarCarrito(); }, Qt::QueuedConnection);
        });
        tablaCarrito_->setCellWidget(i, 1, celda);
        ponerCelda(tablaCarrito_, i, 2, dinero(subtotal), true);
        auto* quitar = new QPushButton(QStringLiteral("Quitar"));
        quitar->setObjectName(QStringLiteral("peligroPequeno"));
        quitar->setCursor(Qt::PointingHandCursor);
        connect(quitar, &QPushButton::clicked, this, [this, productoId] {
            carrito_.erase(std::remove_if(carrito_.begin(), carrito_.end(),
                                          [productoId](const Renglon& r) { return r.producto.id == productoId; }),
                           carrito_.end());
            QMetaObject::invokeMethod(this, [this] { mostrarCarrito(); }, Qt::QueuedConnection);
        });
        tablaCarrito_->setCellWidget(i, 3, quitar);
    }
    const auto ahorro = lista.restarHastaCero(total);
    subtotal_->setText(QStringLiteral("Precio de lista: %1").arg(dinero(lista)));
    ahorro_->setText(ahorro.esCero() ? QString() : QStringLiteral("Ahorro por descuentos: %1").arg(dinero(ahorro)));
    total_->setText(dinero(total));
    emitir_->setEnabled(!carrito_.empty());
    cancelar_->setEnabled(!carrito_.empty());
}

void PaginaVenta::nuevoCliente() {
    bool aceptado = false;
    const QString nombre =
        QInputDialog::getText(this, QStringLiteral("Cliente nuevo"), QStringLiteral("Nombre del cliente:"),
                              QLineEdit::Normal, QString(), &aceptado);
    if (!aceptado) {
        return;
    }
    ejecutarSeguro(this, [&] {
        const auto cliente = crearCliente_.ejecutar(aStd(nombre));
        llenarClientes(cliente.id);
        emit datosCambiados();
        emit mensaje(QStringLiteral("Cliente «%1» registrado.").arg(aQString(cliente.nombre)));
    });
}

void PaginaVenta::emitirFactura() {
    if (!cliente_->currentData().isValid()) {
        QMessageBox::information(this, QStringLiteral("Falta el cliente"),
                                 QStringLiteral("Elija el cliente antes de emitir la factura."));
        cliente_->setFocus();
        return;
    }
    aplicacion::SolicitudFactura solicitud;
    solicitud.clienteId = cliente_->currentData().toInt();
    for (const auto& renglon : carrito_) {
        solicitud.items.push_back({renglon.producto.id, renglon.cantidad});
    }
    ejecutarSeguro(this, [&] {
        const auto factura = crearFactura_.ejecutar(solicitud);
        carrito_.clear();
        mostrarCarrito();
        emit datosCambiados();
        emit mensaje(QStringLiteral("Factura N.º %1 emitida por %2.").arg(factura.id).arg(dinero(factura.total)));
        QString detalle;
        for (const auto& linea : factura.lineas) {
            detalle += QStringLiteral("%1 × %2   %3\n")
                           .arg(linea.cantidad)
                           .arg(aQString(linea.producto), dinero(linea.subtotal));
        }
        if (!factura.ahorro.esCero()) {
            detalle += QStringLiteral("\nAhorro: %1").arg(dinero(factura.ahorro));
        }
        QMessageBox::information(this, QStringLiteral("Factura emitida"),
                                 QStringLiteral("Factura N.º %1 (pedido N.º %2)\n\n%3\nTotal: %4")
                                     .arg(factura.id)
                                     .arg(factura.pedidoId)
                                     .arg(detalle, dinero(factura.total)));
    });
}

void PaginaVenta::cancelarVenta() {
    if (QMessageBox::question(this, QStringLiteral("Cancelar venta"),
                              QStringLiteral("¿Quitar todos los productos de la venta actual?")) == QMessageBox::Yes) {
        carrito_.clear();
        mostrarCarrito();
    }
}

}  // namespace almacen::presentacion::escritorio
