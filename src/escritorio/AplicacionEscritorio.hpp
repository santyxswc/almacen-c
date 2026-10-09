/**
 * @file AplicacionEscritorio.hpp
 * @brief Raíz de composición de la aplicación de escritorio.
 * @author Santiago Caicedo
 */

#pragma once

#include <memory>
#include <string>

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoGenerales.hpp"
#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "infraestructura/ArchivoDatos.hpp"
#include "infraestructura/RepositoriosEnMemoria.hpp"

namespace almacen::presentacion::escritorio {
class VentanaPrincipal;
}

namespace almacen::escritorio {

/**
 * @brief Crea los repositorios, los casos de uso y la ventana, y los conecta.
 *
 * Es el equivalente gráfico de `Aplicacion.cpp`: la consola y la ventana
 * usan exactamente los mismos casos de uso, solo cambia la presentación.
 * Además carga los datos del archivo al iniciar y los guarda cada vez que
 * algo cambia.
 */
class AplicacionEscritorio {
public:
    /**
     * @param rutaDatos Archivo donde se guardan los datos; si no existe, se
     *        empieza con el catálogo de ejemplo.
     * @throws infraestructura::ErrorArchivo si el archivo existe pero está dañado.
     */
    explicit AplicacionEscritorio(const std::string& rutaDatos);
    ~AplicacionEscritorio();

    AplicacionEscritorio(const AplicacionEscritorio&) = delete;
    AplicacionEscritorio& operator=(const AplicacionEscritorio&) = delete;

    /** @return La ventana principal, lista para mostrarse. */
    presentacion::escritorio::VentanaPrincipal& ventana() { return *ventana_; }

    /** @brief Guarda los datos en el archivo. @return Mensaje de error, o vacío si salió bien. */
    std::string guardar();

    /** @name Casos de uso (para cargar datos de ejemplo desde herramientas) */
    ///@{
    aplicacion::CrearCliente& crearCliente() { return crearCliente_; }
    aplicacion::CrearFactura& crearFactura() { return crearFactura_; }
    aplicacion::CrearProducto& crearProducto() { return crearProducto_; }
    aplicacion::CrearSubAlmacen& crearSubAlmacen() { return crearSubAlmacen_; }
    aplicacion::AsignarProductoAAlmacen& asignarProducto() { return asignarProducto_; }
    const aplicacion::ConsultarArbolAlmacenes& arbolAlmacenes() const { return arbolAlmacenes_; }
    ///@}

private:
    infraestructura::ArchivoDatos archivo_;
    infraestructura::RepositorioClientesEnMemoria clientes_;
    infraestructura::RepositorioPedidosEnMemoria pedidos_;
    std::unique_ptr<infraestructura::RepositorioAlmacenesEnMemoria> almacenes_;

    aplicacion::CrearCliente crearCliente_{clientes_};
    aplicacion::ListarClientes listarClientes_{clientes_};
    aplicacion::ListarPedidos listarPedidos_{pedidos_};
    aplicacion::ConsultarResumenCliente resumenCliente_{clientes_, pedidos_};
    aplicacion::CrearFactura crearFactura_;
    aplicacion::ListarProductos listarProductos_;
    aplicacion::CrearProducto crearProducto_;
    aplicacion::CrearSubAlmacen crearSubAlmacen_;
    aplicacion::ConsultarAlmacen consultarAlmacen_;
    aplicacion::AsignarProductoAAlmacen asignarProducto_;
    aplicacion::ConsultarArbolAlmacenes arbolAlmacenes_;
    aplicacion::ConsultarResumenGeneral resumenGeneral_;

    std::unique_ptr<presentacion::escritorio::VentanaPrincipal> ventana_;
};

}  // namespace almacen::escritorio
