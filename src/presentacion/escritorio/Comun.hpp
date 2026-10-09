/**
 * @file Comun.hpp
 * @brief Utilidades compartidas por todas las pantallas de la aplicación de escritorio.
 * @author Santiago Caicedo
 */

#pragma once

#include <QString>
#include <QWidget>
#include <functional>
#include <initializer_list>
#include <string>

#include "dominio/Dinero.hpp"

class QLabel;
class QTableWidget;
class QPushButton;

namespace almacen::presentacion::escritorio {

/** @brief Convierte texto de la capa de aplicación (UTF-8) a QString. */
QString aQString(const std::string& texto);

/** @brief Convierte un QString a texto UTF-8 para la capa de aplicación. */
std::string aStd(const QString& texto);

/**
 * @brief Formatea dinero como se escribe en Colombia.
 * @param valor Cantidad de dinero.
 * @return Texto como `$ 1.250,50`.
 */
QString dinero(dominio::Dinero valor);

/**
 * @brief Ejecuta una acción y muestra cualquier error en un cuadro de diálogo.
 *
 * Los errores del negocio (por ejemplo, "la cantidad debe ser mayor que
 * cero") se muestran como advertencia; los inesperados, como error.
 *
 * @param padre Ventana sobre la que se muestra el mensaje.
 * @param accion Código a ejecutar.
 * @return true si la acción terminó sin errores.
 */
bool ejecutarSeguro(QWidget* padre, const std::function<void()>& accion);

/** @brief Crea el título grande de una pantalla. */
QLabel* crearTitulo(const QString& texto, QWidget* padre = nullptr);

/** @brief Crea un texto de ayuda gris debajo de un título. */
QLabel* crearAyuda(const QString& texto, QWidget* padre = nullptr);

/** @brief Crea un botón con el estilo principal (verde). */
QPushButton* crearBotonPrincipal(const QString& texto, QWidget* padre = nullptr);

/**
 * @brief Prepara una tabla de solo lectura con encabezados y selección por fila.
 * @param tabla Tabla a configurar.
 * @param encabezados Títulos de las columnas.
 */
void configurarTabla(QTableWidget* tabla, const QStringList& encabezados);

/**
 * @brief Pone un valor en una celda de solo lectura.
 * @param tabla Tabla destino.
 * @param fila Fila.
 * @param columna Columna.
 * @param texto Texto de la celda.
 * @param alineadoDerecha true para números y dinero.
 */
void ponerCelda(QTableWidget* tabla, int fila, int columna, const QString& texto, bool alineadoDerecha = false);

/**
 * @brief Describe el descuento de un producto como el dinero que se ahorra por unidad.
 * @return Texto como `− $ 10.000,00`, o una raya si no tiene descuento.
 */
QString textoDescuento(dominio::Dinero precioBase, dominio::Dinero precioFinal);

/**
 * @brief Alinea a la derecha el encabezado de las columnas con números o dinero.
 * @param tabla Tabla ya configurada.
 * @param columnas Índices de las columnas numéricas.
 */
void alinearColumnasNumericas(QTableWidget* tabla, std::initializer_list<int> columnas);

/**
 * @brief Hace que una columna use solo el ancho de su contenido (por ejemplo, la de números de id).
 */
void columnaAngosta(QTableWidget* tabla, int columna);

/**
 * @brief Aplica el estilo visual y el formato colombiano de números a toda la aplicación.
 * @param app Aplicación de Qt ya creada.
 */
void aplicarAparienciaGeneral(class QApplication& app);

/** @return La hoja de estilos de toda la aplicación. */
QString hojaDeEstilos();

}  // namespace almacen::presentacion::escritorio
