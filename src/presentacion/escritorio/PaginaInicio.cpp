/**
 * @file PaginaInicio.cpp
 * @brief Implementación de la pantalla de inicio.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaInicio.hpp"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <algorithm>
#include <map>

#include "presentacion/escritorio/Comun.hpp"

namespace almacen::presentacion::escritorio {

PaginaInicio::PaginaInicio(const aplicacion::ConsultarResumenGeneral& resumen, const aplicacion::ListarPedidos& pedidos,
                           const aplicacion::ListarClientes& clientes, QWidget* padre)
    : Pagina(padre), resumen_(resumen), pedidos_(pedidos), clientes_(clientes) {
    auto* columna = new QVBoxLayout(this);
    columna->setContentsMargins(32, 28, 32, 28);
    columna->setSpacing(18);
    columna->addWidget(crearTitulo(QStringLiteral("Inicio")));
    columna->addWidget(crearAyuda(QStringLiteral("Resumen del negocio. Use los botones para empezar una tarea.")));

    auto* rejilla = new QGridLayout;
    rejilla->setSpacing(16);
    totalVendido_ = agregarTarjeta(rejilla, 0, QStringLiteral("Total vendido"));
    facturas_ = agregarTarjeta(rejilla, 1, QStringLiteral("Facturas emitidas"));
    clientesTotal_ = agregarTarjeta(rejilla, 2, QStringLiteral("Clientes"));
    productos_ = agregarTarjeta(rejilla, 3, QStringLiteral("Productos"));
    columna->addLayout(rejilla);

    auto* acciones = new QHBoxLayout;
    acciones->setSpacing(12);
    auto* vender = crearBotonPrincipal(QStringLiteral("Nueva venta"));
    auto* cliente = new QPushButton(QStringLiteral("Registrar cliente"));
    auto* producto = new QPushButton(QStringLiteral("Registrar producto"));
    auto* almacenes = new QPushButton(QStringLiteral("Ver almacenes"));
    for (auto* boton : {cliente, producto, almacenes}) {
        boton->setCursor(Qt::PointingHandCursor);
    }
    acciones->addWidget(vender);
    acciones->addWidget(cliente);
    acciones->addWidget(producto);
    acciones->addWidget(almacenes);
    acciones->addStretch();
    columna->addLayout(acciones);
    connect(vender, &QPushButton::clicked, this, [this] { emit irA(PaginaDeVenta); });
    connect(cliente, &QPushButton::clicked, this, [this] { emit irA(PaginaDeClientes); });
    connect(producto, &QPushButton::clicked, this, [this] { emit irA(PaginaDeProductos); });
    connect(almacenes, &QPushButton::clicked, this, [this] { emit irA(PaginaDeAlmacenes); });

    auto* seccion = new QLabel(QStringLiteral("Últimas facturas"));
    seccion->setObjectName(QStringLiteral("seccion"));
    columna->addSpacing(6);
    columna->addWidget(seccion);
    ultimas_ = new QTableWidget;
    configurarTabla(ultimas_, {QStringLiteral("Factura"), QStringLiteral("Pedido"), QStringLiteral("Cliente"),
                               QStringLiteral("Productos"), QStringLiteral("Total")});
    alinearColumnasNumericas(ultimas_, {4});
    columna->addWidget(ultimas_, 1);
}

QLabel* PaginaInicio::agregarTarjeta(QGridLayout* rejilla, int columna, const QString& titulo) {
    auto* tarjeta = new QFrame;
    tarjeta->setObjectName(QStringLiteral("tarjeta"));
    auto* contenido = new QVBoxLayout(tarjeta);
    contenido->setContentsMargins(20, 16, 20, 16);
    auto* etiqueta = new QLabel(titulo);
    etiqueta->setObjectName(QStringLiteral("tarjetaTitulo"));
    auto* valor = new QLabel(QStringLiteral("0"));
    valor->setObjectName(QStringLiteral("tarjetaValor"));
    contenido->addWidget(etiqueta);
    contenido->addWidget(valor);
    rejilla->addWidget(tarjeta, 0, columna);
    return valor;
}

void PaginaInicio::refrescar() {
    const auto resumen = resumen_.ejecutar();
    totalVendido_->setText(dinero(resumen.totalFacturado));
    facturas_->setText(QString::number(resumen.facturas));
    clientesTotal_->setText(QString::number(resumen.clientes));
    productos_->setText(QString::number(resumen.productos));

    std::map<int, QString> nombres;
    for (const auto& cliente : clientes_.ejecutar()) {
        nombres[cliente.id] = aQString(cliente.nombre);
    }
    struct Fila {
        int factura;
        int pedido;
        QString cliente;
        int unidades;
        dominio::Dinero total;
    };
    std::vector<Fila> filas;
    for (const auto& pedido : pedidos_.ejecutar()) {
        for (const auto& factura : pedido.facturas) {
            int unidades = 0;
            for (const auto& linea : factura.lineas) {
                unidades += linea.cantidad;
            }
            filas.push_back({factura.id, pedido.id, nombres[pedido.clienteId], unidades, factura.total});
        }
    }
    std::sort(filas.begin(), filas.end(), [](const Fila& a, const Fila& b) { return a.factura > b.factura; });
    if (filas.size() > 8) {
        filas.resize(8);
    }
    ultimas_->setRowCount(static_cast<int>(filas.size()));
    for (int i = 0; i < static_cast<int>(filas.size()); ++i) {
        const auto& fila = filas[static_cast<std::size_t>(i)];
        ponerCelda(ultimas_, i, 0, QStringLiteral("N.º %1").arg(fila.factura));
        ponerCelda(ultimas_, i, 1, QStringLiteral("N.º %1").arg(fila.pedido));
        ponerCelda(ultimas_, i, 2, fila.cliente);
        ponerCelda(ultimas_, i, 3,
                   fila.unidades == 1 ? QStringLiteral("1 unidad") : QStringLiteral("%1 unidades").arg(fila.unidades));
        ponerCelda(ultimas_, i, 4, dinero(fila.total), true);
    }
}

}  // namespace almacen::presentacion::escritorio
