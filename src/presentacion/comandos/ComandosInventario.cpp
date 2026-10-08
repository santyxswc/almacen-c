/**
 * @file ComandosInventario.cpp
 * @brief Implementación de las opciones del menú para productos y almacenes.
 * @author Santiago Caicedo
 */

#include "presentacion/comandos/ComandosInventario.hpp"

#include <ostream>

#include "presentacion/Consola.hpp"
#include "presentacion/Vistas.hpp"

namespace almacen::presentacion {

void ComandoListarProductos::ejecutar(Consola& consola) {
    mostrarProductos(consola.salida(), casoUso_.ejecutar());
}

void ComandoCrearSubAlmacen::ejecutar(Consola& consola) {
    const int id = consola.leerEntero("ID del sub-almacen: ");
    const std::string nombre = consola.leerTexto("Nombre del sub-almacen: ");
    const int padreId = consola.leerEntero("ID del almacen donde se crea (0 = principal): ");
    const auto almacen = casoUso_.ejecutar(id, nombre, padreId);
    consola.salida() << "Sub-almacen " << almacen.id << " (" << almacen.nombre << ") creado con exito.\n";
}

void ComandoVerAlmacen::ejecutar(Consola& consola) {
    const int id = consola.leerEntero("ID del almacen (1 = principal): ");
    mostrarAlmacen(consola.salida(), casoUso_.ejecutar(id));
}

void ComandoAsignarProducto::ejecutar(Consola& consola) {
    consola.salida() << "Productos disponibles:\n";
    mostrarProductos(consola.salida(), listarProductos_.ejecutar());
    const int productoId = consola.leerEntero("ID del producto: ");
    const int almacenId = consola.leerEntero("ID del sub-almacen destino: ");
    const auto almacen = asignar_.ejecutar(productoId, almacenId);
    consola.salida() << "Producto agregado con exito. El almacen " << almacen.id << " ahora tiene "
                     << almacen.productos.size() << " producto(s) propio(s).\n";
}

}  // namespace almacen::presentacion
