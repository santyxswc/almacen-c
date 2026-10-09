/**
 * @file PaginaProductos.cpp
 * @brief Implementación de la pantalla de productos.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/PaginaProductos.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include "presentacion/escritorio/Comun.hpp"
#include "presentacion/escritorio/DialogoProducto.hpp"

namespace almacen::presentacion::escritorio {

PaginaProductos::PaginaProductos(const aplicacion::ListarProductos& listar, aplicacion::CrearProducto& crear,
                                 const aplicacion::ConsultarArbolAlmacenes& arbol, QWidget* padre)
    : Pagina(padre), listar_(listar), crear_(crear), arbol_(arbol) {
    auto* raiz = new QVBoxLayout(this);
    raiz->setContentsMargins(32, 28, 32, 28);
    raiz->setSpacing(16);

    auto* encabezado = new QHBoxLayout;
    auto* textos = new QVBoxLayout;
    textos->addWidget(crearTitulo(QStringLiteral("Productos")));
    textos->addWidget(crearAyuda(QStringLiteral("Todo lo que se puede vender, con su precio y su descuento.")));
    encabezado->addLayout(textos, 1);
    auto* nuevo = crearBotonPrincipal(QStringLiteral("+ Producto nuevo"));
    encabezado->addWidget(nuevo, 0, Qt::AlignBottom);
    raiz->addLayout(encabezado);
    connect(nuevo, &QPushButton::clicked, this, &PaginaProductos::nuevoProducto);

    buscar_ = new QLineEdit;
    buscar_->setPlaceholderText(QStringLiteral("Buscar producto…"));
    buscar_->setClearButtonEnabled(true);
    raiz->addWidget(buscar_);
    connect(buscar_, &QLineEdit::textChanged, this, &PaginaProductos::filtrar);

    tabla_ = new QTableWidget;
    configurarTabla(tabla_, {QStringLiteral("N.º"), QStringLiteral("Producto"), QStringLiteral("Precio"),
                             QStringLiteral("Descuento"), QStringLiteral("Precio final")});
    alinearColumnasNumericas(tabla_, {2, 3, 4});
    columnaAngosta(tabla_, 0);
    raiz->addWidget(tabla_, 1);
}

void PaginaProductos::refrescar() {
    const auto productos = listar_.ejecutar();
    tabla_->setRowCount(static_cast<int>(productos.size()));
    for (int i = 0; i < static_cast<int>(productos.size()); ++i) {
        const auto& producto = productos[static_cast<std::size_t>(i)];
        ponerCelda(tabla_, i, 0, QString::number(producto.id));
        ponerCelda(tabla_, i, 1, aQString(producto.nombre));
        ponerCelda(tabla_, i, 2, dinero(producto.precioBase), true);
        ponerCelda(tabla_, i, 3, textoDescuento(producto.precioBase, producto.precioFinal), true);
        ponerCelda(tabla_, i, 4, dinero(producto.precioFinal), true);
    }
    filtrar();
}

void PaginaProductos::filtrar() {
    const QString texto = buscar_->text().trimmed();
    for (int i = 0; i < tabla_->rowCount(); ++i) {
        tabla_->setRowHidden(i, !texto.isEmpty() && !tabla_->item(i, 1)->text().contains(texto, Qt::CaseInsensitive));
    }
}

void PaginaProductos::nuevoProducto() {
    DialogoProducto dialogo(arbol_.ejecutar(), this);
    while (dialogo.exec() == QDialog::Accepted) {
        const bool listo = ejecutarSeguro(this, [&] {
            const auto producto = crear_.ejecutar(dialogo.solicitud());
            emit datosCambiados();
            emit mensaje(QStringLiteral("Producto «%1» guardado.").arg(aQString(producto.nombre)));
        });
        if (listo) {
            return;
        }
    }
}

}  // namespace almacen::presentacion::escritorio
