/**
 * @file main_escritorio.cpp
 * @brief Punto de entrada de la aplicación de escritorio.
 * @author Santiago Caicedo
 */

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>
#include <memory>

#include "escritorio/AplicacionEscritorio.hpp"
#include "presentacion/escritorio/Comun.hpp"
#include "presentacion/escritorio/VentanaPrincipal.hpp"

/**
 * @brief Abre la ventana con los datos guardados en la carpeta del usuario.
 *
 * Si el archivo de datos está dañado, ofrece guardarlo aparte y empezar de
 * nuevo en lugar de cerrarse sin explicación.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName(QStringLiteral("SantiagoCaicedo"));
    QApplication::setApplicationName(QStringLiteral("SistemaAlmacen"));
    QApplication::setApplicationDisplayName(QStringLiteral("Sistema de Almacén"));
    almacen::presentacion::escritorio::aplicarAparienciaGeneral(app);

    const QString carpeta = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(carpeta);
    const QString ruta = carpeta + QStringLiteral("/datos-almacen.txt");

    std::unique_ptr<almacen::escritorio::AplicacionEscritorio> aplicacion;
    try {
        aplicacion = std::make_unique<almacen::escritorio::AplicacionEscritorio>(ruta.toStdString());
    } catch (const std::exception& error) {
        const auto respuesta = QMessageBox::critical(
            nullptr, QStringLiteral("No se pudieron leer los datos"),
            QString::fromStdString(error.what()) +
                QStringLiteral("\n\n¿Guardar una copia del archivo dañado y empezar con datos nuevos?"),
            QMessageBox::Yes | QMessageBox::No);
        if (respuesta != QMessageBox::Yes) {
            return 1;
        }
        const QString copia = ruta + QStringLiteral(".danado");
        QFile::remove(copia);
        QFile::rename(ruta, copia);
        aplicacion = std::make_unique<almacen::escritorio::AplicacionEscritorio>(ruta.toStdString());
    }
    aplicacion->ventana().show();
    return QApplication::exec();
}
