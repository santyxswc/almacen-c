/**
 * @file PaginaClientes.cpp
 * @brief Implementación de la pantalla de clientes.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaClientes.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include "presentacion/escritorio/Comun.hpp"

namespace almacen::presentacion::escritorio {

PaginaClientes::PaginaClientes(const aplicacion::ListarClientes& listar, aplicacion::CrearCliente& crear,
                               const aplicacion::ConsultarResumenCliente& resumen, QWidget* padre)
    : Pagina(padre), listar_(listar), crear_(crear), resumen_(resumen) {
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(32, 28, 32, 28);
    raiz->setSpacing(16);
    raiz->addWidget(crearTitulo(QStringLiteral("Clientes")));
    raiz->addWidget(
        crearAyuda(QStringLiteral("Registre a sus clientes para poder venderles y ver lo que han "
                                  "comprado. El número de cliente se asigna solo.")));

    auto* formulario = new QFrame;
    formulario->setObjectName(QStringLiteral("tarjeta"));
    auto* fila = new QHBoxLayout(formulario);
    fila->setContentsMargins(18, 14, 18, 14);
    fila->setSpacing(10);
    auto* etiqueta = new QLabel(QStringLiteral("Cliente nuevo"));
    etiqueta->setObjectName(QStringLiteral("seccion"));
    nombre_ = new QLineEdit;
    nombre_->setObjectName(QStringLiteral("nombreCliente"));
    nombre_->setPlaceholderText(QStringLiteral("Nombre completo o razón social"));
    auto* registrar = crearBotonPrincipal(QStringLiteral("Registrar cliente"));
    fila->addWidget(etiqueta);
    fila->addWidget(nombre_, 1);
    fila->addWidget(registrar);
    raiz->addWidget(formulario);
    connect(registrar, &QPushButton::clicked, this, &PaginaClientes::registrar);
    connect(nombre_, &QLineEdit::returnPressed, this, &PaginaClientes::registrar);

    buscar_ = new QLineEdit;
    buscar_->setPlaceholderText(QStringLiteral("Buscar cliente…"));
    buscar_->setClearButtonEnabled(true);
    raiz->addWidget(buscar_);
    connect(buscar_, &QLineEdit::textChanged, this, &PaginaClientes::filtrar);

    tabla_ = new QTableWidget;
    configurarTabla(tabla_, {QStringLiteral("N.º"), QStringLiteral("Nombre"), QStringLiteral("Pedidos"),
                             QStringLiteral("Total comprado")});
    alinearColumnasNumericas(tabla_, {2, 3});
    columnaAngosta(tabla_, 0);
    raiz->addWidget(tabla_, 1);
}

void PaginaClientes::refrescar() {
    const auto clientes = listar_.ejecutar();
    tabla_->setRowCount(static_cast<int>(clientes.size()));
    for (int i = 0; i < static_cast<int>(clientes.size()); ++i) {
        const auto& cliente = clientes[static_cast<std::size_t>(i)];
        const auto resumen = resumen_.ejecutar(cliente.id);
        ponerCelda(tabla_, i, 0, QString::number(cliente.id));
        ponerCelda(tabla_, i, 1, aQString(cliente.nombre));
        ponerCelda(tabla_, i, 2, QString::number(resumen.pedidos.size()), true);
        ponerCelda(tabla_, i, 3, dinero(resumen.totalGeneral), true);
    }
    filtrar();
}

void PaginaClientes::filtrar() {
    const QString texto = buscar_->text().trimmed();
    for (int i = 0; i < tabla_->rowCount(); ++i) {
        tabla_->setRowHidden(i, !texto.isEmpty() && !tabla_->item(i, 1)->text().contains(texto, Qt::CaseInsensitive));
    }
}

void PaginaClientes::registrar() {
    ejecutarSeguro(this, [this] {
        const auto cliente = crear_.ejecutar(aStd(nombre_->text()));
        nombre_->clear();
        emit datosCambiados();
        emit mensaje(
            QStringLiteral("Cliente «%1» registrado con el número %2.").arg(aQString(cliente.nombre)).arg(cliente.id));
    });
}

}  // namespace almacen::presentacion::escritorio
