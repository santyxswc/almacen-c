/**
 * @file PruebasConsola.cpp
 * @brief Pruebas de punta a punta del menú con entrada simulada.
 * @author Santiago Caicedo
 */

#include <sstream>
#include <string>

#include "Aplicacion.hpp"
#include "MiniTest.hpp"
#include "presentacion/Consola.hpp"

namespace {

std::string ejecutarCon(const std::string& entrada) {
    std::istringstream in(entrada);
    std::ostringstream out;
    almacen::ejecutarAplicacion(in, out);
    return out.str();
}

bool contiene(const std::string& texto, const std::string& fragmento) {
    return texto.find(fragmento) != std::string::npos;
}

}  // namespace

PRUEBA(consola_rechaza_texto_donde_se_espera_un_numero) {
    std::istringstream in("abc\n");
    std::ostringstream out;
    almacen::presentacion::Consola consola(in, out);
    VERIFICAR_LANZA(consola.leerEntero("> "), almacen::presentacion::EntradaInvalida);
}

PRUEBA(menu_crea_cliente_pedido_y_factura) {
    const std::string salida = ejecutarCon(
        "1\n1\nAna Maria\n"            // crear cliente
        "3\n10\n1\n"                   // crear pedido
        "4\n100\n10\n1\n2\n2\n1\n0\n"  // factura: 2 camisas y 1 pantalon
        "5\n1\n"                       // resumen
        "0\n");
    VERIFICAR(contiene(salida, "Cliente 1 (Ana Maria) creado con exito."));
    VERIFICAR(contiene(salida, "Pedido 10 creado con exito."));
    VERIFICAR(contiene(salida, "Total factura: $340.00"));
    VERIFICAR(contiene(salida, "Total a pagar: $340.00"));
    VERIFICAR(contiene(salida, "Hasta pronto."));
}

PRUEBA(menu_muestra_errores_y_sigue_funcionando) {
    const std::string salida = ejecutarCon(
        "hola\n"     // opción que no es número
        "3\n1\n5\n"  // pedido para un cliente que no existe
        "99\n"       // opción fuera de rango
        "0\n");
    VERIFICAR(contiene(salida, "Entrada invalida"));
    VERIFICAR(contiene(salida, "Error: Cliente con ID 5 no existe."));
    VERIFICAR(contiene(salida, "Opcion invalida."));
    VERIFICAR(contiene(salida, "Hasta pronto."));
}

PRUEBA(menu_termina_si_se_acaba_la_entrada) {
    VERIFICAR(contiene(ejecutarCon("1\n"), "Hasta pronto."));
}
