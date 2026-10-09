/**
 * @file PruebasPersistencia.cpp
 * @brief Pruebas del guardado y la carga del archivo de datos.
 * @author Santiago Caicedo
 */

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "MiniTest.hpp"
#include "aplicacion/casos_uso/CasosUsoClientes.hpp"
#include "aplicacion/casos_uso/CasosUsoInventario.hpp"
#include "aplicacion/casos_uso/CasosUsoPedidos.hpp"
#include "infraestructura/ArchivoDatos.hpp"
#include "infraestructura/CatalogoInicial.hpp"
#include "infraestructura/RepositoriosEnMemoria.hpp"

using namespace almacen;
using namespace almacen::aplicacion;
using dominio::Dinero;

namespace {

std::string rutaTemporal(const std::string& nombre) {
    return (std::filesystem::temp_directory_path() / ("almacen-pruebas-" + nombre)).string();
}

}  // namespace

PRUEBA(guardar_y_cargar_conserva_todo) {
    const std::string ruta = rutaTemporal("completo.txt");
    std::filesystem::remove(ruta);

    infraestructura::RepositorioClientesEnMemoria clientes;
    infraestructura::RepositorioPedidosEnMemoria pedidos;
    infraestructura::RepositorioAlmacenesEnMemoria almacenes(infraestructura::crearAlmacenConCatalogoInicial());

    CrearCliente(clientes).ejecutar("Ana Maria\tcon tab");
    CrearSubAlmacen(almacenes).ejecutar(0, "Bodega");
    CrearSubAlmacen(almacenes).ejecutar(0, "Estante", 2);
    AsignarProductoAAlmacen(almacenes).ejecutar(2, 3);
    SolicitudProducto producto;
    producto.nombre = "Gorra";
    producto.precio = Dinero::desdeUnidades(33.33);
    producto.tipoDescuento = TipoDescuento::Porcentual;
    producto.valorDescuento = 12.5;
    producto.almacenId = 2;
    CrearProducto(almacenes).ejecutar(producto);
    SolicitudFactura venta;
    venta.clienteId = 1;
    venta.items = {{1, 2}, {2, 1}, {5, 3}};
    const auto factura = CrearFactura(pedidos, almacenes, clientes).ejecutar(venta);

    const infraestructura::ArchivoDatos archivo(ruta);
    VERIFICAR(!archivo.existe());
    archivo.guardar(clientes, pedidos, almacenes);
    VERIFICAR(archivo.existe());

    auto datos = archivo.cargar();
    infraestructura::RepositorioClientesEnMemoria clientes2;
    infraestructura::RepositorioPedidosEnMemoria pedidos2;
    for (const auto& c : datos.clientes)
        clientes2.guardar(c);
    for (const auto& p : datos.pedidos)
        pedidos2.guardar(p);
    infraestructura::RepositorioAlmacenesEnMemoria almacenes2(std::move(datos.principal));

    VERIFICAR_IGUAL(std::string("Ana Maria\tcon tab"), clientes2.buscar(1)->nombre());
    const auto resumen = ConsultarResumenCliente(clientes2, pedidos2).ejecutar(1);
    VERIFICAR_IGUAL(factura.total, resumen.totalGeneral);
    VERIFICAR_IGUAL(std::size_t{3}, resumen.pedidos.front().facturas.front().lineas.size());
    VERIFICAR_IGUAL(std::size_t{5}, almacenes2.principal().totalProductos());
    const auto estante = ConsultarAlmacen(almacenes2).ejecutar(3);
    VERIFICAR_IGUAL(std::string("Pantalón"), estante.productos.front().nombre);
    const auto gorra = almacenes2.principal().buscarProducto(5);
    VERIFICAR_IGUAL(producto.precio, gorra->precioBase());
    VERIFICAR_IGUAL(std::string("12.5% de descuento"), gorra->descuento().describir());
    VERIFICAR_IGUAL(2, pedidos2.siguienteFacturaId());

    // Guardar de nuevo lo cargado produce el mismo archivo.
    const std::string copia = rutaTemporal("copia.txt");
    infraestructura::ArchivoDatos(copia).guardar(clientes2, pedidos2, almacenes2);
    std::ifstream a(ruta), b(copia);
    const std::string textoA((std::istreambuf_iterator<char>(a)), std::istreambuf_iterator<char>());
    const std::string textoB((std::istreambuf_iterator<char>(b)), std::istreambuf_iterator<char>());
    VERIFICAR(textoA == textoB);
    std::filesystem::remove(ruta);
    std::filesystem::remove(copia);
}

PRUEBA(archivo_danado_da_un_error_claro) {
    const std::string ruta = rutaTemporal("danado.txt");
    {
        std::ofstream salida(ruta);
        salida << "ALMACEN-DATOS\t1\nPRINCIPAL\t1\tPrincipal\nPRODUCTO\t1\tCamisa\tabc\tNINGUNO\t0\n";
    }
    VERIFICAR_LANZA(infraestructura::ArchivoDatos(ruta).cargar(), infraestructura::ErrorArchivo);
    {
        std::ofstream salida(ruta);
        salida << "cualquier cosa\n";
    }
    VERIFICAR_LANZA(infraestructura::ArchivoDatos(ruta).cargar(), infraestructura::ErrorArchivo);
    std::filesystem::remove(ruta);
    VERIFICAR_LANZA(infraestructura::ArchivoDatos(ruta).cargar(), infraestructura::ErrorArchivo);
}
