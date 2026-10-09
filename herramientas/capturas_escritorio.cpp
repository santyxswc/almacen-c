/**
 * @file capturas_escritorio.cpp
 * @brief Genera las capturas de pantalla del README con datos de ejemplo.
 * @author Santiago Caicedo
 *
 * Uso: `QT_QPA_PLATFORM=offscreen ./capturas_escritorio <carpeta-de-salida>`
 */

#include <QApplication>
#include <QDir>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QStatusBar>
#include <QTemporaryDir>
#include <QTest>

#include "escritorio/AplicacionEscritorio.hpp"
#include "presentacion/escritorio/Comun.hpp"
#include "presentacion/escritorio/DialogoProducto.hpp"
#include "presentacion/escritorio/PaginaVenta.hpp"
#include "presentacion/escritorio/VentanaPrincipal.hpp"

using namespace almacen;

namespace {

void cargarEjemplo(escritorio::AplicacionEscritorio& app) {
    for (const char* nombre :
         {"Ferretería El Tornillo", "María Fernanda López", "Distribuidora Andina S.A.S.", "Carlos Restrepo"}) {
        app.crearCliente().ejecutar(nombre);
    }
    auto producto = [&](const char* nombre, double precio, aplicacion::TipoDescuento tipo, double valor) {
        aplicacion::SolicitudProducto solicitud;
        solicitud.nombre = nombre;
        solicitud.precio = dominio::Dinero::desdeUnidades(precio);
        solicitud.tipoDescuento = tipo;
        solicitud.valorDescuento = valor;
        app.crearProducto().ejecutar(solicitud);
    };
    using T = aplicacion::TipoDescuento;
    producto("Chaqueta impermeable", 189900, T::Porcentual, 15);
    producto("Gorra deportiva", 35000, T::Ninguno, 0);
    producto("Medias x3 pares", 24900, T::Fijo, 4900);
    producto("Cinturón de cuero", 65000, T::Ninguno, 0);

    app.crearSubAlmacen().ejecutar(0, "Bodega norte");
    app.crearSubAlmacen().ejecutar(0, "Estante A - Calzado", 2);
    app.crearSubAlmacen().ejecutar(0, "Estante B - Ropa", 2);
    app.crearSubAlmacen().ejecutar(0, "Bodega sur");
    app.asignarProducto().ejecutar(3, 3);
    app.asignarProducto().ejecutar(4, 3);
    app.asignarProducto().ejecutar(1, 4);
    app.asignarProducto().ejecutar(2, 4);
    app.asignarProducto().ejecutar(5, 4);

    auto vender = [&](int cliente, std::vector<aplicacion::ItemSolicitado> items) {
        aplicacion::SolicitudFactura venta;
        venta.clienteId = cliente;
        venta.items = std::move(items);
        app.crearFactura().ejecutar(venta);
    };
    vender(1, {{1, 3}, {6, 2}});
    vender(2, {{5, 1}, {7, 4}});
    vender(3, {{2, 10}, {1, 12}, {8, 5}});
    vender(4, {{3, 1}});
    vender(2, {{4, 1}, {6, 1}});
}

}  // namespace

int main(int argc, char* argv[]) {
    QApplication qt(argc, argv);
    presentacion::escritorio::aplicarAparienciaGeneral(qt);
    const QString salida = argc > 1 ? QString::fromLocal8Bit(argv[1]) : QStringLiteral("docs/capturas");
    QDir().mkpath(salida);

    QTemporaryDir temporal;
    escritorio::AplicacionEscritorio app((temporal.path() + QStringLiteral("/datos.txt")).toStdString());
    cargarEjemplo(app);

    auto& ventana = app.ventana();
    ventana.resize(1366, 820);
    ventana.show();

    using namespace presentacion::escritorio;
    auto* venta = static_cast<PaginaVenta*>(ventana.pagina(PaginaDeVenta));
    ventana.mostrarPagina(PaginaDeVenta);
    venta->seleccionarCliente(2);
    venta->agregarAlCarrito(5, 1);
    venta->agregarAlCarrito(1, 2);
    venta->agregarAlCarrito(7, 3);

    const struct {
        int pagina;
        const char* archivo;
    } capturas[] = {{PaginaDeInicio, "inicio"},     {PaginaDeVenta, "venta"},         {PaginaDeHistorial, "historial"},
                    {PaginaDeClientes, "clientes"}, {PaginaDeProductos, "productos"}, {PaginaDeAlmacenes, "almacenes"}};
    for (const auto& captura : capturas) {
        if (captura.pagina != PaginaDeVenta) {
            ventana.mostrarPagina(captura.pagina);
        } else {
            ventana.mostrarPagina(PaginaDeVenta);
            venta->seleccionarCliente(2);
        }
        ventana.statusBar()->clearMessage();
        QTest::qWait(150);
        ventana.grab().save(salida + QLatin1Char('/') + QString::fromLatin1(captura.archivo) + QStringLiteral(".png"));
    }

    DialogoProducto dialogo(app.arbolAlmacenes().ejecutar(), &ventana);
    dialogo.findChild<QLineEdit*>(QStringLiteral("nombreProducto"))->setText(QStringLiteral("Botas de seguridad"));
    dialogo.findChildren<QDoubleSpinBox*>().at(0)->setValue(159900);
    for (auto* opcion : dialogo.findChildren<QRadioButton*>()) {
        if (opcion->text() == QStringLiteral("Porcentaje")) {
            opcion->setChecked(true);
        }
    }
    dialogo.findChildren<QDoubleSpinBox*>().at(1)->setValue(10);
    dialogo.show();
    QTest::qWait(150);
    dialogo.grab().save(salida + QStringLiteral("/producto-nuevo.png"));
    return 0;
}
