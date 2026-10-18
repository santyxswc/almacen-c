# Historial de versiones

Formato basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/); las versiones siguen
[versionado semántico](https://semver.org/lang/es/). Para publicar una versión, ver
[`docs/DESARROLLO.md`](docs/DESARROLLO.md#publicar-una-versión).

## [Sin publicar]

### Cambiado
- Documentación de la API completa: todas las clases, funciones y parámetros de `src/` tienen su
  comentario Doxygen, y la CI falla si aparece código sin documentar.
- README con una sección para futuros cambios, guía de desarrollo (`docs/DESARROLLO.md`) y manual de uso
  (`docs/MANUAL_USUARIO.md`).

## [2.1.0] — 2026-10-09

### Agregado
- **Aplicación de escritorio con Qt 6**: pantallas de inicio, nueva venta, pedidos y facturas, clientes,
  productos y almacenes, pensadas para quien trabaja en el almacén.
- **Guardado automático** en un archivo de texto en la carpeta del usuario, a prueba de cortes de luz.
- Números de cliente, pedido, factura, producto y sub-almacén asignados automáticamente.
- Casos de uso `CrearProducto`, `ListarPedidos`, `ConsultarArbolAlmacenes` y `ConsultarResumenGeneral`.
- `CrearFactura` abre el pedido si hace falta, sin dejar pedidos vacíos si algo falla.
- Pruebas de la ventana con clics simulados (Qt Test) y de guardado y carga del archivo.
- Flujo de GitHub Actions que empaqueta `SistemaAlmacen.exe` para Windows con las bibliotecas de Qt.

### Corregido
- Las tildes y la eñe se ven bien en la consola de Windows.
- Las columnas de la consola quedan alineadas aunque los nombres tengan tildes.

## [2.0.0] — 2026-10-08

### Cambiado
- Reescritura completa con **arquitectura limpia** (dominio, aplicación, infraestructura y presentación),
  principios SOLID y patrones Strategy, Composite, Command y Repository.
- Compilación con CMake en Linux, Windows y macOS (antes era un proyecto de ZinjaI).

### Corregido
- La factura cobraba el precio sin descuento.
- Fugas de memoria por `new` sin `delete`.
- Solo se podía tener un cliente y un pedido a la vez, y las facturas tenían exactamente cuatro productos.
- El dinero en `float` acumulaba errores de redondeo.
- Al cerrarse la entrada, el programa quedaba en un ciclo infinito.

### Agregado
- 31 pruebas automáticas, integración continua y documentación con Doxygen.

## [1.0.0] — 2024-10-06

- Versión original del curso de Programación Orientada a Objetos: menú de consola con clientes, pedidos,
  facturas y sub-almacenes.
