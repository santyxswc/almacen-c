/**
 * @file ArchivoDatos.cpp
 * @brief Implementación de la persistencia en archivo de texto.
 * @author Santiago Caicedo
 */

#include "infraestructura/ArchivoDatos.hpp"

#include <filesystem>
#include <fstream>
#include <locale>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <utility>

#include "dominio/Excepciones.hpp"
#include "dominio/PoliticaDescuento.hpp"

namespace almacen::infraestructura {

namespace {

constexpr const char* kEncabezado = "ALMACEN-DATOS";
constexpr int kVersion = 1;

std::string escapar(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        switch (c) {
            case '\\':
                resultado += "\\\\";
                break;
            case '\t':
                resultado += "\\t";
                break;
            case '\n':
                resultado += "\\n";
                break;
            case '\r':
                break;
            default:
                resultado += c;
        }
    }
    return resultado;
}

std::string desescapar(const std::string& texto) {
    std::string resultado;
    for (std::size_t i = 0; i < texto.size(); ++i) {
        if (texto[i] == '\\' && i + 1 < texto.size()) {
            const char siguiente = texto[++i];
            resultado += siguiente == 't' ? '\t' : siguiente == 'n' ? '\n' : siguiente;
        } else {
            resultado += texto[i];
        }
    }
    return resultado;
}

std::vector<std::string> dividir(const std::string& linea) {
    std::vector<std::string> campos;
    std::string campo;
    std::istringstream entrada(linea);
    while (std::getline(entrada, campo, '\t')) {
        campos.push_back(desescapar(campo));
    }
    if (!linea.empty() && linea.back() == '\t') {
        campos.emplace_back();
    }
    return campos;
}

std::string numeroDecimal(double valor) {
    std::ostringstream texto;
    texto.imbue(std::locale::classic());
    texto.precision(17);
    texto << valor;
    return texto.str();
}

/** @brief Lee los campos de una línea con validación y mensajes claros. */
class Registro {
public:
    Registro(std::vector<std::string> campos, int numeroLinea)
        : campos_(std::move(campos)), numeroLinea_(numeroLinea) {}

    const std::string& tipo() const { return campos_.front(); }

    void exigirCampos(std::size_t cantidad) const {
        if (campos_.size() != cantidad) {
            fallar("se esperaban " + std::to_string(cantidad) + " campos");
        }
    }

    const std::string& texto(std::size_t i) const { return campos_.at(i); }

    long long entero(std::size_t i) const {
        try {
            std::size_t usados = 0;
            const long long valor = std::stoll(campos_.at(i), &usados);
            if (usados != campos_.at(i).size()) {
                fallar("numero invalido");
            }
            return valor;
        } catch (const std::logic_error&) {
            fallar("numero invalido");
        }
    }

    int id(std::size_t i) const { return static_cast<int>(entero(i)); }

    double decimal(std::size_t i) const {
        std::istringstream entrada(campos_.at(i));
        entrada.imbue(std::locale::classic());
        double valor = 0;
        if (!(entrada >> valor) || !entrada.eof()) {
            fallar("numero decimal invalido");
        }
        return valor;
    }

    [[noreturn]] void fallar(const std::string& motivo) const {
        throw ErrorArchivo("Archivo de datos danado (linea " + std::to_string(numeroLinea_) + "): " + motivo + ".");
    }

private:
    std::vector<std::string> campos_;
    int numeroLinea_;
};

std::unique_ptr<dominio::PoliticaDescuento> leerDescuento(const Registro& registro) {
    const std::string& tipo = registro.texto(4);
    if (tipo == "NINGUNO") {
        return std::make_unique<dominio::SinDescuento>();
    }
    if (tipo == "FIJO") {
        return std::make_unique<dominio::DescuentoFijo>(dominio::Dinero::desdeCentavos(registro.entero(5)));
    }
    if (tipo == "PORCENTAJE") {
        return std::make_unique<dominio::DescuentoPorcentual>(registro.decimal(5));
    }
    registro.fallar("tipo de descuento desconocido '" + tipo + "'");
}

void escribirDescuento(std::ostream& salida, const dominio::PoliticaDescuento& descuento) {
    if (const auto* fijo = dynamic_cast<const dominio::DescuentoFijo*>(&descuento)) {
        salida << "FIJO\t" << fijo->monto().centavos();
    } else if (const auto* porcentual = dynamic_cast<const dominio::DescuentoPorcentual*>(&descuento)) {
        salida << "PORCENTAJE\t" << numeroDecimal(porcentual->porcentaje());
    } else {
        salida << "NINGUNO\t0";
    }
}

void escribirAlmacen(std::ostream& salida, const dominio::Almacen& almacen, int padreId,
                     std::set<int>& productosEscritos, std::ostringstream& ubicaciones) {
    if (padreId != 0) {
        salida << "ALMACEN\t" << almacen.id() << '\t' << escapar(almacen.nombre()) << '\t' << padreId << '\n';
    }
    for (const auto& producto : almacen.productos()) {
        if (productosEscritos.insert(producto->id()).second) {
            salida << "PRODUCTO\t" << producto->id() << '\t' << escapar(producto->nombre()) << '\t'
                   << producto->precioBase().centavos() << '\t';
            escribirDescuento(salida, producto->descuento());
            salida << '\n';
        }
        ubicaciones << "UBICACION\t" << almacen.id() << '\t' << producto->id() << '\n';
    }
    for (const auto* sub : almacen.subAlmacenes()) {
        escribirAlmacen(salida, *sub, almacen.id(), productosEscritos, ubicaciones);
    }
}

}  // namespace

bool ArchivoDatos::existe() const {
    std::error_code error;
    return std::filesystem::exists(ruta_, error);
}

void ArchivoDatos::guardar(const aplicacion::RepositorioClientes& clientes,
                           const aplicacion::RepositorioPedidos& pedidos,
                           const aplicacion::RepositorioAlmacenes& almacenes) const {
    std::ostringstream contenido;
    const dominio::Almacen& principal = almacenes.principal();
    contenido << kEncabezado << '\t' << kVersion << '\n';
    contenido << "PRINCIPAL\t" << principal.id() << '\t' << escapar(principal.nombre()) << '\n';

    std::set<int> productosEscritos;
    std::ostringstream ubicaciones;
    escribirAlmacen(contenido, principal, 0, productosEscritos, ubicaciones);
    contenido << ubicaciones.str();

    for (const auto& cliente : clientes.listar()) {
        contenido << "CLIENTE\t" << cliente.id() << '\t' << escapar(cliente.nombre()) << '\n';
    }
    for (const auto& pedido : pedidos.listar()) {
        contenido << "PEDIDO\t" << pedido.id() << '\t' << pedido.clienteId() << '\n';
        for (const auto& factura : pedido.facturas()) {
            contenido << "FACTURA\t" << factura.id() << '\t' << pedido.id() << '\n';
            for (const auto& linea : factura.lineas()) {
                contenido << "LINEA\t" << factura.id() << '\t' << linea.productoId() << '\t'
                          << escapar(linea.nombreProducto()) << '\t' << linea.cantidad() << '\t'
                          << linea.precioBase().centavos() << '\t' << linea.precioUnitario().centavos() << '\n';
            }
        }
    }

    const std::filesystem::path destino(ruta_);
    std::error_code error;
    if (destino.has_parent_path()) {
        std::filesystem::create_directories(destino.parent_path(), error);
    }
    const std::filesystem::path temporal = destino.string() + ".tmp";
    {
        std::ofstream salida(temporal, std::ios::binary | std::ios::trunc);
        salida << contenido.str();
        if (!salida) {
            throw ErrorArchivo("No se pudo escribir el archivo de datos en " + temporal.string() + ".");
        }
    }
    std::filesystem::rename(temporal, destino, error);
    if (error) {
        throw ErrorArchivo("No se pudo reemplazar el archivo de datos: " + error.message() + ".");
    }
}

DatosAlmacen ArchivoDatos::cargar() const {
    std::ifstream entrada(ruta_, std::ios::binary);
    if (!entrada) {
        throw ErrorArchivo("No se pudo abrir el archivo de datos " + ruta_ + ".");
    }

    std::vector<Registro> registros;
    std::string linea;
    int numero = 0;
    while (std::getline(entrada, linea)) {
        ++numero;
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        if (linea.empty()) {
            continue;
        }
        registros.emplace_back(dividir(linea), numero);
    }
    if (registros.empty() || registros.front().tipo() != kEncabezado) {
        throw ErrorArchivo("El archivo " + ruta_ + " no es un archivo de datos del almacen.");
    }
    registros.front().exigirCampos(2);
    if (registros.front().entero(1) != kVersion) {
        throw ErrorArchivo("El archivo de datos es de una version distinta del programa.");
    }

    try {
        std::unique_ptr<dominio::Almacen> principal;
        std::map<int, std::shared_ptr<const dominio::Producto>> productos;
        std::vector<dominio::Cliente> clientes;
        std::map<int, dominio::Pedido> pedidos;
        std::map<int, dominio::Factura> facturas;
        std::map<int, int> pedidoDeFactura;
        std::vector<int> ordenFacturas;

        for (std::size_t i = 1; i < registros.size(); ++i) {
            const Registro& r = registros[i];
            const std::string& tipo = r.tipo();
            if (tipo == "PRINCIPAL") {
                r.exigirCampos(3);
                principal = std::make_unique<dominio::Almacen>(r.id(1), r.texto(2));
            } else if (principal == nullptr) {
                r.fallar("falta el almacen principal al inicio");
            } else if (tipo == "PRODUCTO") {
                r.exigirCampos(6);
                productos[r.id(1)] = std::make_shared<const dominio::Producto>(
                    r.id(1), r.texto(2), dominio::Dinero::desdeCentavos(r.entero(3)), leerDescuento(r));
            } else if (tipo == "ALMACEN") {
                r.exigirCampos(4);
                dominio::Almacen* padre = principal->buscarAlmacen(r.id(3));
                if (padre == nullptr) {
                    r.fallar("el almacen padre no existe");
                }
                padre->agregarSubAlmacen(std::make_unique<dominio::Almacen>(r.id(1), r.texto(2)));
            } else if (tipo == "UBICACION") {
                r.exigirCampos(3);
                dominio::Almacen* almacen = principal->buscarAlmacen(r.id(1));
                const auto producto = productos.find(r.id(2));
                if (almacen == nullptr || producto == productos.end()) {
                    r.fallar("ubicacion con almacen o producto inexistente");
                }
                almacen->agregarProducto(producto->second);
            } else if (tipo == "CLIENTE") {
                r.exigirCampos(3);
                clientes.emplace_back(r.id(1), r.texto(2));
            } else if (tipo == "PEDIDO") {
                r.exigirCampos(3);
                pedidos.emplace(r.id(1), dominio::Pedido(r.id(1), r.id(2)));
            } else if (tipo == "FACTURA") {
                r.exigirCampos(3);
                const auto pedido = pedidos.find(r.id(2));
                if (pedido == pedidos.end()) {
                    r.fallar("factura de un pedido inexistente");
                }
                facturas.emplace(r.id(1), dominio::Factura(r.id(1), pedido->second.clienteId()));
                pedidoDeFactura[r.id(1)] = r.id(2);
                ordenFacturas.push_back(r.id(1));
            } else if (tipo == "LINEA") {
                r.exigirCampos(7);
                const auto factura = facturas.find(r.id(1));
                if (factura == facturas.end()) {
                    r.fallar("linea de una factura inexistente");
                }
                factura->second.agregarLinea(dominio::LineaFactura(r.id(2), r.texto(3), r.id(4),
                                                                   dominio::Dinero::desdeCentavos(r.entero(5)),
                                                                   dominio::Dinero::desdeCentavos(r.entero(6))));
            } else {
                r.fallar("tipo de registro desconocido '" + tipo + "'");
            }
        }
        if (principal == nullptr) {
            throw ErrorArchivo("El archivo de datos no tiene almacen principal.");
        }
        for (int facturaId : ordenFacturas) {
            pedidos.at(pedidoDeFactura.at(facturaId)).agregarFactura(facturas.at(facturaId));
        }

        DatosAlmacen datos{std::move(*principal), std::move(clientes), {}};
        for (auto& [id, pedido] : pedidos) {
            datos.pedidos.push_back(std::move(pedido));
        }
        return datos;
    } catch (const dominio::ErrorDominio& error) {
        throw ErrorArchivo(std::string("Archivo de datos con informacion invalida: ") + error.what());
    }
}

}  // namespace almacen::infraestructura
