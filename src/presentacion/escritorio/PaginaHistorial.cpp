/**
 * @file PaginaHistorial.cpp
 * @brief Implementación de la pantalla de historial.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaHistorial.hpp"

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <map>

#include "presentacion/escritorio/Comun.hpp"

namespace almacen::presentacion::escritorio {

namespace {
constexpr int kRolPedido = Qt::UserRole;
constexpr int kRolFactura = Qt::UserRole + 1;
}  // namespace

PaginaHistorial::PaginaHistorial(const aplicacion::ListarPedidos& pedidos, const aplicacion::ListarClientes& clientes,
                                 QWidget* padre)
    : Pagina(padre), listarPedidos_(pedidos), listarClientes_(clientes) {
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(32, 28, 32, 28);
    raiz->setSpacing(16);
    raiz->addWidget(crearTitulo(QStringLiteral("Pedidos y facturas")));
    raiz->addWidget(
        crearAyuda(QStringLiteral("Cada venta abre un pedido con su factura. Elija una factura para ver "
                                  "qué productos se vendieron.")));

    auto* filaFiltro = new QHBoxLayout;
    filaFiltro->addWidget(new QLabel(QStringLiteral("Mostrar")));
    filtro_ = new QComboBox;
    filtro_->setMinimumWidth(300);
    filaFiltro->addWidget(filtro_);
    filaFiltro->addStretch();
    raiz->addLayout(filaFiltro);

    auto* cuerpo = new QHBoxLayout;
    cuerpo->setSpacing(20);
    arbol_ = new QTreeWidget;
    arbol_->setHeaderLabels({QStringLiteral("Pedido / factura"), QStringLiteral("Cliente"), QStringLiteral("Total")});
    arbol_->header()->setSectionResizeMode(QHeaderView::Stretch);
    arbol_->headerItem()->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
    arbol_->setAlternatingRowColors(true);
    arbol_->setRootIsDecorated(true);
    cuerpo->addWidget(arbol_, 5);

    auto* panel = new QFrame;
    panel->setObjectName(QStringLiteral("tarjeta"));
    auto* detalle = new QVBoxLayout(panel);
    detalle->setContentsMargins(18, 16, 18, 18);
    tituloDetalle_ = new QLabel(QStringLiteral("Seleccione una factura"));
    tituloDetalle_->setObjectName(QStringLiteral("seccion"));
    detalle->addWidget(tituloDetalle_);
    lineas_ = new QTableWidget;
    configurarTabla(lineas_, {QStringLiteral("Producto"), QStringLiteral("Cantidad"), QStringLiteral("Precio"),
                              QStringLiteral("Subtotal")});
    alinearColumnasNumericas(lineas_, {1, 2, 3});
    detalle->addWidget(lineas_, 1);
    ahorro_ = new QLabel;
    ahorro_->setObjectName(QStringLiteral("ahorro"));
    total_ = new QLabel;
    total_->setObjectName(QStringLiteral("totalGrande"));
    detalle->addWidget(ahorro_);
    detalle->addWidget(total_);
    cuerpo->addWidget(panel, 4);
    raiz->addLayout(cuerpo, 1);

    connect(filtro_, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) { llenarArbol(); });
    connect(arbol_, &QTreeWidget::currentItemChanged, this, [this] { mostrarDetalle(); });
}

void PaginaHistorial::refrescar() {
    const int clienteActual = filtro_->currentData().isValid() ? filtro_->currentData().toInt() : 0;
    pedidos_ = listarPedidos_.ejecutar();
    filtro_->blockSignals(true);
    filtro_->clear();
    filtro_->addItem(QStringLiteral("Todos los clientes"), 0);
    for (const auto& cliente : listarClientes_.ejecutar()) {
        filtro_->addItem(aQString(cliente.nombre), cliente.id);
    }
    filtro_->setCurrentIndex(std::max(0, filtro_->findData(clienteActual)));
    filtro_->blockSignals(false);
    llenarArbol();
}

void PaginaHistorial::llenarArbol() {
    std::map<int, QString> nombres;
    for (int i = 1; i < filtro_->count(); ++i) {
        nombres[filtro_->itemData(i).toInt()] = filtro_->itemText(i);
    }
    const int cliente = filtro_->currentData().toInt();
    arbol_->clear();
    for (auto pedido = pedidos_.rbegin(); pedido != pedidos_.rend(); ++pedido) {
        if (cliente != 0 && pedido->clienteId != cliente) {
            continue;
        }
        auto* nodo = new QTreeWidgetItem(arbol_, {QStringLiteral("Pedido N.º %1").arg(pedido->id),
                                                  nombres[pedido->clienteId], dinero(pedido->total)});
        nodo->setData(0, kRolPedido, pedido->id);
        nodo->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
        for (const auto& factura : pedido->facturas) {
            auto* hoja = new QTreeWidgetItem(
                nodo, {QStringLiteral("Factura N.º %1").arg(factura.id), QString(), dinero(factura.total)});
            hoja->setData(0, kRolPedido, pedido->id);
            hoja->setData(0, kRolFactura, factura.id);
            hoja->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
        }
        nodo->setExpanded(true);
    }
    if (arbol_->topLevelItemCount() > 0 && arbol_->topLevelItem(0)->childCount() > 0) {
        arbol_->setCurrentItem(arbol_->topLevelItem(0)->child(0));
    } else {
        mostrarDetalle();
    }
}

void PaginaHistorial::mostrarDetalle() {
    lineas_->setRowCount(0);
    ahorro_->clear();
    total_->clear();
    const QTreeWidgetItem* actual = arbol_->currentItem();
    if (actual == nullptr || !actual->data(0, kRolFactura).isValid()) {
        tituloDetalle_->setText(arbol_->topLevelItemCount() == 0 ? QStringLiteral("Todavía no hay ventas")
                                                                 : QStringLiteral("Seleccione una factura"));
        return;
    }
    const int pedidoId = actual->data(0, kRolPedido).toInt();
    const int facturaId = actual->data(0, kRolFactura).toInt();
    for (const auto& pedido : pedidos_) {
        if (pedido.id != pedidoId) {
            continue;
        }
        for (const auto& factura : pedido.facturas) {
            if (factura.id != facturaId) {
                continue;
            }
            tituloDetalle_->setText(QStringLiteral("Factura N.º %1").arg(factura.id));
            lineas_->setRowCount(static_cast<int>(factura.lineas.size()));
            for (int i = 0; i < static_cast<int>(factura.lineas.size()); ++i) {
                const auto& linea = factura.lineas[static_cast<std::size_t>(i)];
                ponerCelda(lineas_, i, 0, aQString(linea.producto));
                ponerCelda(lineas_, i, 1, QString::number(linea.cantidad), true);
                ponerCelda(lineas_, i, 2, dinero(linea.precioUnitario), true);
                ponerCelda(lineas_, i, 3, dinero(linea.subtotal), true);
            }
            if (!factura.ahorro.esCero()) {
                ahorro_->setText(QStringLiteral("Ahorro por descuentos: %1").arg(dinero(factura.ahorro)));
            }
            total_->setText(dinero(factura.total));
        }
    }
}

}  // namespace almacen::presentacion::escritorio
