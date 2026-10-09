/**
 * @file VentanaPrincipal.hpp
 * @brief Ventana con el menú lateral y las pantallas.
 * @author Santiago Caicedo
 */

#pragma once

#include <QMainWindow>
#include <functional>
#include <vector>

#include "presentacion/escritorio/Pagina.hpp"

class QButtonGroup;
class QStackedWidget;
class QVBoxLayout;

namespace almacen::presentacion::escritorio {

/**
 * @brief Ventana principal: menú a la izquierda y la pantalla elegida a la derecha.
 *
 * No conoce los casos de uso: recibe las pantallas ya construidas. Cuando
 * una pantalla avisa que cambió datos, llama a la función de guardado y
 * actualiza todas las pantallas (patrón Mediator).
 */
class VentanaPrincipal final : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @param guardar Función que guarda los datos; devuelve un mensaje de error o vacío si todo salió bien.
     * @param padre Widget padre.
     */
    explicit VentanaPrincipal(std::function<QString()> guardar, QWidget* padre = nullptr);

    /**
     * @brief Agrega una pantalla al menú.
     * @param titulo Texto de la opción en el menú.
     * @param pagina Pantalla; la ventana pasa a ser su dueña.
     */
    void agregarPagina(const QString& titulo, Pagina* pagina);

    /** @brief Muestra la pantalla en la posición indicada y la actualiza. */
    void mostrarPagina(int indice);

    /** @brief Actualiza todas las pantallas con los datos actuales. */
    void refrescarTodo();

    /** @return La pantalla en la posición indicada. */
    Pagina* pagina(int indice) const { return paginas_.at(static_cast<std::size_t>(indice)); }

private:
    void alCambiarDatos();

    std::function<QString()> guardar_;
    QStackedWidget* pila_ = nullptr;
    QButtonGroup* opciones_ = nullptr;
    QVBoxLayout* menu_ = nullptr;
    std::vector<Pagina*> paginas_;
};

}  // namespace almacen::presentacion::escritorio
