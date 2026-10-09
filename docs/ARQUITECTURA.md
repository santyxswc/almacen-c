# Arquitectura

Este documento explica cómo está organizado el código y por qué. Está pensado para quien quiera
entender el diseño antes de leer las clases.

## Capas

El proyecto sigue la **arquitectura limpia** (Clean Architecture). El código se divide en cuatro capas
concéntricas y **las dependencias solo apuntan hacia adentro**: el dominio no conoce a nadie, y la
consola, la ventana o la forma de guardar los datos son detalles que se pueden cambiar sin tocar las
reglas del negocio.

La prueba de que funciona: la aplicación de escritorio y el guardado en archivo se agregaron después,
como adaptadores nuevos, sin modificar ninguna regla del dominio.

```mermaid
flowchart TB
    subgraph P["Presentación (consola)"]
        Menu["MenuPrincipal"] --> Cmd["Comandos"]
        Cmd --> Vistas
    end
    subgraph E["Presentación (escritorio, Qt)"]
        Ventana["VentanaPrincipal"] --> Paginas["Una clase por pantalla"]
    end
    subgraph I["Infraestructura"]
        Repos["Repositorios en memoria"]
        Archivo["ArchivoDatos"]
        Catalogo["CatalogoInicial"]
    end
    subgraph A["Aplicación"]
        CU["Casos de uso"] --> Puertos["Puertos (interfaces de repositorio)"]
        CU --> DTOs
    end
    subgraph D["Dominio"]
        Entidades["Almacen, Producto, Cliente, Pedido, Factura"]
        Valor["Dinero, LineaFactura, PoliticaDescuento"]
    end
    Cmd --> CU
    Paginas --> CU
    Repos -. implementa .-> Puertos
    Archivo -. lee con .-> Puertos
    CU --> Entidades
    Repos --> Entidades
    Raiz["Aplicacion.cpp / AplicacionEscritorio.cpp<br/>raíces de composición"] --> P
    Raiz --> E
    Raiz --> I
    Raiz --> A
```

| Capa | Carpeta | Responsabilidad | Depende de |
| --- | --- | --- | --- |
| Dominio | `src/dominio` | Reglas del negocio: precios, descuentos, totales, validaciones, árbol de almacenes. | Nadie (solo la biblioteca estándar) |
| Aplicación | `src/aplicacion` | Casos de uso que orquestan el dominio. Define los **puertos** (interfaces) que necesita. | Dominio |
| Infraestructura | `src/infraestructura` | Implementa los puertos: repositorios en memoria, archivo de datos y datos iniciales. | Aplicación, Dominio |
| Presentación (consola) | `src/presentacion` | Menú de consola: lee datos, llama casos de uso y muestra DTOs. | Aplicación, Dominio |
| Presentación (escritorio) | `src/presentacion/escritorio` | Ventana de Qt: una clase por pantalla, formularios y estilos. | Aplicación, Dominio, Qt |
| Raíces de composición | `src/Aplicacion.cpp`, `src/escritorio/` | Crean los objetos concretos y los conectan. | Todas |

## Aplicación de escritorio

- `VentanaPrincipal` no conoce ningún caso de uso: recibe las pantallas ya construidas y hace de
  **Mediator**. Cuando una pantalla emite `datosCambiados()`, la ventana guarda el archivo y actualiza
  todas las pantallas (**Observer** con señales de Qt).
- Cada pantalla (`PaginaVenta`, `PaginaHistorial`...) recibe por el constructor **solo** los casos de uso
  que usa. `PaginaVenta`, por ejemplo, no puede crear sub-almacenes aunque quisiera.
- Los errores del negocio se muestran con `ejecutarSeguro()`: el mismo mensaje del dominio aparece en
  un cuadro de diálogo y la ventana sigue funcionando.
- Los números de cliente, pedido, factura, producto y almacén los asigna la capa de aplicación
  (`siguienteId()` en los puertos), así el usuario nunca tiene que inventarlos.
- `ArchivoDatos` escribe un archivo de texto versionado, primero en un temporal que luego reemplaza al
  original: un corte de luz a mitad del guardado no deja el archivo a medias. Si al abrir el archivo
  está dañado, el programa ofrece guardarlo aparte y empezar de nuevo.
- `tests/PruebasEscritorio.cpp` maneja la ventana con clics y teclas simulados (Qt Test) y verifica que
  la venta quede guardada en el archivo.

## Diagrama de clases del dominio

```mermaid
classDiagram
    class Identificable {
        <<interface>>
        +id() int
    }
    class Dinero {
        <<value object>>
        -centavos : int64
        +desdeUnidades(double) Dinero
        +multiplicar(int) Dinero
        +porcentaje(double) Dinero
        +formatear() string
    }
    class PoliticaDescuento {
        <<interface>>
        +aplicar(Dinero) Dinero
        +describir() string
        +clonar() PoliticaDescuento
    }
    class SinDescuento
    class DescuentoFijo
    class DescuentoPorcentual
    class Producto {
        +precioBase() Dinero
        +precioFinal() Dinero
        +cambiarDescuento(PoliticaDescuento)
    }
    class Almacen {
        +agregarProducto(Producto)
        +agregarSubAlmacen(Almacen) Almacen
        +totalProductos() size_t
        +buscarProducto(int) Producto
        +buscarAlmacen(int) Almacen
    }
    class Cliente
    class Pedido {
        +agregarFactura(Factura)
        +total() Dinero
    }
    class Factura {
        +agregarProducto(Producto, int)
        +total() Dinero
        +ahorroTotal() Dinero
    }
    class LineaFactura {
        <<value object>>
        +subtotal() Dinero
        +ahorro() Dinero
    }

    Identificable <|.. Producto
    Identificable <|.. Almacen
    Identificable <|.. Cliente
    Identificable <|.. Pedido
    Identificable <|.. Factura
    PoliticaDescuento <|.. SinDescuento
    PoliticaDescuento <|.. DescuentoFijo
    PoliticaDescuento <|.. DescuentoPorcentual
    Producto *-- PoliticaDescuento : estrategia
    Almacen o-- Producto : comparte
    Almacen *-- Almacen : sub-almacenes
    Pedido *-- Factura : agregado
    Factura *-- LineaFactura
    Pedido --> Cliente : clienteId
```

## Patrones de diseño

| Patrón | Dónde | Qué resuelve |
| --- | --- | --- |
| **Strategy** | `PoliticaDescuento` y sus implementaciones | Antes cada descuento era una subclase de `Producto`. Ahora el descuento es una pieza intercambiable: un tipo nuevo de descuento no obliga a tocar `Producto`. |
| **Null Object** | `SinDescuento` | Un producto siempre tiene política; no hay que preguntar si es nula. |
| **Prototype** | `PoliticaDescuento::clonar()` | Permite copiar un producto con su propio descuento sin conocer el tipo concreto. |
| **Composite** | `Almacen` | Un almacén contiene productos y otros almacenes. Contar, buscar productos o buscar sub-almacenes recorre el árbol completo con la misma interfaz. |
| **Repository** | `RepositorioClientes`, `RepositorioPedidos`, `RepositorioAlmacenes` | Los casos de uso guardan y buscan sin saber dónde viven los datos. |
| **Aggregate Root** (DDD) | `Pedido` | Las facturas solo se agregan a través del pedido, que valida cliente, ids repetidos y facturas vacías. |
| **Value Object** (DDD) | `Dinero`, `LineaFactura` | Valores inmutables comparados por contenido. `LineaFactura` guarda el precio del momento: si el producto cambia después, la factura no. |
| **DTO** | `aplicacion/Dtos.hpp` | La presentación recibe datos planos, nunca entidades que pueda modificar. |
| **Command** | `presentacion/comandos` | Cada opción del menú es un objeto. Reemplaza el `switch` de 150 líneas de la versión anterior. |
| **Factory** | `crearAlmacenConCatalogoInicial()` | Un solo lugar para construir los datos de ejemplo. |
| **Observer** | Señales `datosCambiados()`, `mensaje()` e `irA()` de `Pagina` | Las pantallas no se conocen entre sí; avisan y la ventana reacciona. |
| **Mediator** | `VentanaPrincipal` | Coordina guardar y actualizar todas las pantallas después de cada cambio. |
| **Template Method** | `Pagina::refrescar()` | Todas las pantallas se actualizan de la misma forma desde la ventana. |
| **Inyección de dependencias** | `Aplicacion.cpp`, `AplicacionEscritorio.cpp` | Las clases reciben lo que necesitan por constructor; nadie crea sus propias dependencias. |

## Principios SOLID

| Principio | Cómo se aplica |
| --- | --- |
| **S** — Responsabilidad única | Cada caso de uso es una clase con un solo método `ejecutar`. `Consola` solo lee y valida; `Vistas` solo da formato; `MenuPrincipal` solo despacha. |
| **O** — Abierto/cerrado | Un descuento nuevo es una clase nueva que implementa `PoliticaDescuento`. Una opción de menú nueva es un `Comando` nuevo registrado en `Aplicacion.cpp`; `MenuPrincipal` no cambia. |
| **L** — Sustitución de Liskov | Cualquier `PoliticaDescuento` funciona donde se espera una; cualquier implementación de un repositorio funciona con los casos de uso (las pruebas usan las mismas que el programa). |
| **I** — Segregación de interfaces | Un repositorio por agregado, con solo los métodos que los casos de uso usan. `Identificable` exige únicamente `id()`. Cada comando recibe solo los casos de uso que necesita. |
| **D** — Inversión de dependencias | Los casos de uso dependen de interfaces (`puertos/`), no de `std::map` ni de archivos. La infraestructura depende de esas interfaces, no al revés. |

## Errores

- El dominio lanza excepciones que heredan de `ErrorDominio` (`ValorInvalido`, `EntidadNoEncontrada`,
  `EntidadDuplicada`), con mensajes listos para mostrar.
- La consola lanza `EntradaInvalida` cuando el usuario escribe algo que no se puede interpretar, y
  `FinDeEntrada` cuando se cierra la entrada.
- `MenuPrincipal` atrapa todas: muestra el mensaje y el programa sigue funcionando.
- Las operaciones son atómicas: si una factura tiene un producto inválido, el pedido no se modifica.

## Memoria

El dominio, la aplicación y la infraestructura no usan `new` ni `delete`. Los sub-almacenes pertenecen a
su padre (`std::unique_ptr`), los productos se comparten entre almacenes (`std::shared_ptr<const Producto>`)
y el resto son valores. La CI ejecuta esas pruebas con AddressSanitizer para garantizar que no haya fugas
ni accesos inválidos.

En la ventana, los widgets se crean con `new` como es habitual en Qt: cada uno pertenece a su widget
padre, que lo libera automáticamente.

## Cómo extender

| Quiero... | Hago... |
| --- | --- |
| Un descuento "2x1" | Una clase `DescuentoDosPorUno : PoliticaDescuento`. Nada más cambia. |
| Guardar los datos en SQLite | Clases nuevas que implementen los puertos de `aplicacion/puertos` y cambiarlas en las raíces de composición. Los casos de uso y las pruebas del dominio no cambian. |
| Una interfaz web | Otra capa de presentación que llame a los mismos casos de uso, como ya lo hacen la consola y la ventana. |
| Una opción nueva en el menú de consola | Un `Comando` nuevo y una línea `menu.agregar(...)` en `Aplicacion.cpp`. |
| Una pantalla nueva en la ventana | Una clase que herede de `Pagina` y una línea `agregarPagina(...)` en `AplicacionEscritorio.cpp`. |
