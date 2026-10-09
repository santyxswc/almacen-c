/**
 * @file PruebasEscritorio.cpp
 * @brief Pruebas de la aplicación de escritorio con clics y teclas simulados (Qt Test).
 * @author Santiago Caicedo
 *
 * Se ejecutan sin pantalla con `QT_QPA_PLATFORM=offscreen`.
 */

#include <QApplication>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest>
#include <memory>

#include "escritorio/AplicacionEscritorio.hpp"
#include "infraestructura/ArchivoDatos.hpp"
#include "presentacion/escritorio/PaginaVenta.hpp"
#include "presentacion/escritorio/VentanaPrincipal.hpp"

using namespace almacen;
using namespace almacen::presentacion::escritorio;

namespace {

QPushButton* botonConTexto(QWidget* raiz, const QString& texto) {
    for (auto* boton : raiz->findChildren<QPushButton*>()) {
        if (boton->text() == texto && boton->isVisible()) {
            return boton;
        }
    }
    return nullptr;
}

/** @brief Cierra el próximo cuadro de mensaje en cuanto aparezca (revisa cada 20 ms, hasta 5 s). */
void aceptarProximoMensaje() {
    auto* temporizador = new QTimer;
    auto intentos = std::make_shared<int>(0);
    QObject::connect(temporizador, &QTimer::timeout, [temporizador, intentos] {
        for (auto* ventana : QApplication::topLevelWidgets()) {
            if (auto* caja = qobject_cast<QMessageBox*>(ventana); caja && caja->isVisible()) {
                caja->accept();
                temporizador->deleteLater();
                return;
            }
        }
        if (++*intentos > 250) {
            temporizador->deleteLater();
        }
    });
    temporizador->start(20);
}

}  // namespace

/** @brief Pruebas de punta a punta de la ventana. */
class PruebasEscritorio : public QObject {
    Q_OBJECT

private slots:
    void registrarClienteYVenderQuedaGuardado() {
        QTemporaryDir carpeta;
        const std::string ruta = (carpeta.path() + QStringLiteral("/datos.txt")).toStdString();
        {
            escritorio::AplicacionEscritorio app(ruta);
            auto& ventana = app.ventana();
            ventana.show();

            ventana.mostrarPagina(PaginaDeClientes);
            QWidget* clientes = ventana.pagina(PaginaDeClientes);
            auto* nombre = clientes->findChild<QLineEdit*>(QStringLiteral("nombreCliente"));
            QVERIFY(nombre != nullptr);
            QTest::keyClicks(nombre, QStringLiteral("Ferreteria El Tornillo"));
            QTest::mouseClick(botonConTexto(clientes, QStringLiteral("Registrar cliente")), Qt::LeftButton);
            QCOMPARE(clientes->findChildren<QTableWidget*>().front()->rowCount(), 1);

            ventana.mostrarPagina(PaginaDeVenta);
            auto* venta = static_cast<PaginaVenta*>(ventana.pagina(PaginaDeVenta));
            venta->seleccionarCliente(1);
            venta->agregarAlCarrito(1, 2);  // 2 camisas a $90
            venta->agregarAlCarrito(2, 1);  // 1 pantalón a $160
            aceptarProximoMensaje();
            QTest::mouseClick(botonConTexto(venta, QStringLiteral("Emitir factura")), Qt::LeftButton);
            QVERIFY(!botonConTexto(venta, QStringLiteral("Emitir factura"))->isEnabled());  // carrito vacío
        }

        // Los datos sobreviven a cerrar y abrir el programa.
        auto datos = infraestructura::ArchivoDatos(ruta).cargar();
        QCOMPARE(datos.clientes.size(), std::size_t{1});
        QCOMPARE(QString::fromStdString(datos.clientes.front().nombre()), QStringLiteral("Ferreteria El Tornillo"));
        QCOMPARE(datos.pedidos.size(), std::size_t{1});
        QCOMPARE(datos.pedidos.front().total(), dominio::Dinero::desdeUnidades(340));
    }

    void venderSinClienteNoEmiteFactura() {
        QTemporaryDir carpeta;
        const std::string ruta = (carpeta.path() + QStringLiteral("/datos.txt")).toStdString();
        escritorio::AplicacionEscritorio app(ruta);
        app.ventana().show();
        app.ventana().mostrarPagina(PaginaDeVenta);
        auto* venta = static_cast<PaginaVenta*>(app.ventana().pagina(PaginaDeVenta));
        venta->agregarAlCarrito(1, 1);
        aceptarProximoMensaje();
        QTest::mouseClick(botonConTexto(venta, QStringLiteral("Emitir factura")), Qt::LeftButton);
        QVERIFY(botonConTexto(venta, QStringLiteral("Emitir factura"))->isEnabled());  // sigue en el carrito
        QVERIFY(!infraestructura::ArchivoDatos(ruta).existe());
    }
};

QTEST_MAIN(PruebasEscritorio)
#include "PruebasEscritorio.moc"
