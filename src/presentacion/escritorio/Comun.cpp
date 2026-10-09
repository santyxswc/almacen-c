/**
 * @file Comun.cpp
 * @brief Implementación de las utilidades de la aplicación de escritorio.
 * @author Santiago Caicedo
 */

#include "presentacion/escritorio/Comun.hpp"

#include <QApplication>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QStyleFactory>
#include <QTableWidget>
#include <exception>

#include "dominio/Excepciones.hpp"

namespace almacen::presentacion::escritorio {

QString aQString(const std::string& texto) {
    return QString::fromStdString(texto);
}

std::string aStd(const QString& texto) {
    return texto.trimmed().toStdString();
}

QString dinero(dominio::Dinero valor) {
    static const QLocale colombia(QLocale::Spanish, QLocale::Colombia);
    const double pesos = static_cast<double>(valor.centavos()) / 100.0;
    return QStringLiteral("$ ") + colombia.toString(pesos, 'f', 2);
}

QString textoDescuento(dominio::Dinero precioBase, dominio::Dinero precioFinal) {
    const auto ahorro = precioBase.restarHastaCero(precioFinal);
    return ahorro.esCero() ? QStringLiteral("—") : QStringLiteral("− ") + dinero(ahorro);
}

bool ejecutarSeguro(QWidget* padre, const std::function<void()>& accion) {
    try {
        accion();
        return true;
    } catch (const dominio::ErrorDominio& error) {
        QMessageBox::warning(padre, QStringLiteral("No se pudo completar"), QString::fromStdString(error.what()));
    } catch (const std::exception& error) {
        QMessageBox::critical(padre, QStringLiteral("Error inesperado"), QString::fromStdString(error.what()));
    }
    return false;
}

QLabel* crearTitulo(const QString& texto, QWidget* padre) {
    auto* etiqueta = new QLabel(texto, padre);
    etiqueta->setObjectName(QStringLiteral("titulo"));
    return etiqueta;
}

QLabel* crearAyuda(const QString& texto, QWidget* padre) {
    auto* etiqueta = new QLabel(texto, padre);
    etiqueta->setObjectName(QStringLiteral("ayuda"));
    etiqueta->setWordWrap(true);
    return etiqueta;
}

QPushButton* crearBotonPrincipal(const QString& texto, QWidget* padre) {
    auto* boton = new QPushButton(texto, padre);
    boton->setObjectName(QStringLiteral("principal"));
    boton->setCursor(Qt::PointingHandCursor);
    return boton;
}

void configurarTabla(QTableWidget* tabla, const QStringList& encabezados) {
    tabla->setColumnCount(static_cast<int>(encabezados.size()));
    tabla->setHorizontalHeaderLabels(encabezados);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setAlternatingRowColors(true);
    tabla->setShowGrid(false);
    tabla->verticalHeader()->setVisible(false);
    tabla->verticalHeader()->setDefaultSectionSize(38);
    tabla->horizontalHeader()->setStretchLastSection(true);
    tabla->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tabla->horizontalHeader()->setHighlightSections(false);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ponerCelda(QTableWidget* tabla, int fila, int columna, const QString& texto, bool alineadoDerecha) {
    auto* celda = new QTableWidgetItem(texto);
    celda->setTextAlignment(alineadoDerecha ? (Qt::AlignRight | Qt::AlignVCenter) : (Qt::AlignLeft | Qt::AlignVCenter));
    tabla->setItem(fila, columna, celda);
}

void aplicarAparienciaGeneral(QApplication& app) {
    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Colombia));
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    app.setStyleSheet(hojaDeEstilos());
}

void alinearColumnasNumericas(QTableWidget* tabla, std::initializer_list<int> columnas) {
    for (int columna : columnas) {
        if (auto* encabezado = tabla->horizontalHeaderItem(columna)) {
            encabezado->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
    }
}

void columnaAngosta(QTableWidget* tabla, int columna) {
    tabla->horizontalHeader()->setSectionResizeMode(columna, QHeaderView::ResizeToContents);
}

QString hojaDeEstilos() {
    return QStringLiteral(R"(
* { font-size: 11pt; }
QMainWindow, QDialog, QWidget#contenido { background: #f4f5f2; }
QWidget#menu { background: #17453b; }
QLabel#marca { color: white; font-size: 15pt; font-weight: 700; padding: 22px 18px 4px 18px; }
QLabel#submarca { color: #a7c7bd; font-size: 9.5pt; padding: 0 18px 18px 18px; }
QPushButton#opcionMenu {
    color: #e4f0ec; background: transparent; border: none; text-align: left;
    padding: 12px 18px; margin: 2px 10px; border-radius: 8px; font-size: 11.5pt;
}
QPushButton#opcionMenu:hover { background: #1f5a4d; }
QPushButton#opcionMenu:checked { background: white; color: #17453b; font-weight: 600; }
QLabel#titulo { font-size: 20pt; font-weight: 700; color: #1d2a26; }
QLabel#ayuda { color: #66706c; font-size: 10.5pt; }
QLabel#seccion { font-size: 12.5pt; font-weight: 600; color: #1d2a26; }
QFrame#tarjeta { background: white; border: 1px solid #dfe3df; border-radius: 12px; }
QLabel#tarjetaTitulo { color: #66706c; font-size: 10pt; }
QLabel#tarjetaValor { color: #1d2a26; font-size: 19pt; font-weight: 700; }
QLabel#totalGrande { color: #17453b; font-size: 26pt; font-weight: 800; }
QLabel#ahorro { color: #1f6f5c; font-weight: 600; }
QPushButton {
    background: white; border: 1px solid #cdd3cf; border-radius: 8px; padding: 9px 16px; color: #1d2a26;
}
QPushButton:hover { background: #eef2ef; }
QPushButton:disabled { color: #a3aaa6; background: #f1f2f0; }
QPushButton#principal {
    background: #1f6f5c; border: 1px solid #1f6f5c; color: white; font-weight: 600; padding: 11px 20px;
}
QPushButton#principal:hover { background: #195c4c; }
QPushButton#principal:disabled { background: #9dbbb2; border-color: #9dbbb2; }
QPushButton#peligro { color: #b42318; border-color: #f0c4c0; }
QPushButton#peligro:hover { background: #fdf0ef; }
QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
    background: white; border: 1px solid #cdd3cf; border-radius: 8px; padding: 7px 10px; min-height: 22px;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #1f6f5c; padding: 6px 9px; }
QAbstractSpinBox::up-button, QAbstractSpinBox::down-button { width: 0; border: none; }
QSpinBox#cantidad { qproperty-alignment: AlignCenter; border-radius: 0; border-left: none; border-right: none; }
QPushButton#pasoMas { min-width: 30px; max-width: 30px; padding: 6px 0; font-size: 13pt; font-weight: 700;
    color: #17453b; border-top-left-radius: 0; border-bottom-left-radius: 0; }
QPushButton#pasoMenos { min-width: 30px; max-width: 30px; padding: 6px 0; font-size: 13pt; font-weight: 700;
    color: #17453b; border-top-right-radius: 0; border-bottom-right-radius: 0; }
QPushButton#peligroPequeno {
    color: #b42318; border: 1px solid #f0c4c0; border-radius: 6px; padding: 4px 8px; margin: 10px 6px;
    font-size: 10pt; background: white;
}
QPushButton#peligroPequeno:hover { background: #fdf0ef; }
QTableWidget, QTreeWidget {
    background: white; border: 1px solid #dfe3df; border-radius: 10px; alternate-background-color: #f8faf8;
    selection-background-color: #d5ebe3; selection-color: #1d2a26;
}
QHeaderView::section {
    background: #f1f3f1; color: #4b5753; border: none; border-bottom: 1px solid #dfe3df;
    padding: 9px 10px; font-weight: 600; font-size: 10pt;
}
QTreeWidget::item { padding: 6px 2px; }
QStatusBar { background: white; color: #4b5753; border-top: 1px solid #dfe3df; }
QRadioButton { spacing: 8px; }
)");
}

}  // namespace almacen::presentacion::escritorio
