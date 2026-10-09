/**
 * @file ArchivoDatos.hpp
 * @brief Guarda y carga todos los datos del negocio en un archivo de texto.
 * @author Santiago Caicedo
 */

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "aplicacion/puertos/RepositorioAlmacenes.hpp"
#include "aplicacion/puertos/RepositorioClientes.hpp"
#include "aplicacion/puertos/RepositorioPedidos.hpp"
#include "dominio/Almacen.hpp"
#include "dominio/Cliente.hpp"
#include "dominio/Pedido.hpp"

namespace almacen::infraestructura {

/**
 * @brief El archivo de datos no se pudo leer o escribir, o está dañado.
 */
class ErrorArchivo : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * @brief Todo lo que contiene un archivo de datos, listo para llenar los repositorios.
 */
struct DatosAlmacen {
    dominio::Almacen principal;              ///< Almacén raíz con sus sub-almacenes y productos.
    std::vector<dominio::Cliente> clientes;  ///< Clientes registrados.
    std::vector<dominio::Pedido> pedidos;    ///< Pedidos con sus facturas.
};

/**
 * @brief Persistencia en un archivo de texto plano (una línea por registro).
 *
 * Solo usa los puertos de la capa de aplicación para leer el estado, así que
 * sirve con cualquier implementación de los repositorios. El formato es
 * legible y versionado:
 *
 * ```
 * ALMACEN-DATOS	1
 * PRODUCTO	1	Camisa	10000	FIJO	1000
 * ALMACEN	2	Bodega	1
 * UBICACION	2	1
 * CLIENTE	1	Ana
 * PEDIDO	1	1
 * FACTURA	1	1
 * LINEA	1	1	Camisa	2	10000	9000
 * ```
 *
 * Se escribe primero en un archivo temporal y luego se reemplaza el original,
 * así un corte de luz a mitad del guardado no deja el archivo a medias.
 */
class ArchivoDatos {
public:
    /** @param ruta Ruta completa del archivo de datos. */
    explicit ArchivoDatos(std::string ruta) : ruta_(std::move(ruta)) {}

    /** @return La ruta del archivo. */
    const std::string& ruta() const noexcept { return ruta_; }

    /** @return true si el archivo ya existe. */
    bool existe() const;

    /**
     * @brief Lee el archivo completo.
     * @return Los datos reconstruidos.
     * @throws ErrorArchivo si no se puede leer o el contenido no es válido.
     */
    DatosAlmacen cargar() const;

    /**
     * @brief Escribe el estado actual de los repositorios.
     * @throws ErrorArchivo si no se puede escribir.
     */
    void guardar(const aplicacion::RepositorioClientes& clientes, const aplicacion::RepositorioPedidos& pedidos,
                 const aplicacion::RepositorioAlmacenes& almacenes) const;

private:
    std::string ruta_;
};

}  // namespace almacen::infraestructura
