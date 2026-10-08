/**
 * @file Consola.cpp
 * @brief Implementación de Consola.
 * @author Santiago Caicedo
 */

#include "presentacion/Consola.hpp"

#include <cerrno>
#include <cstdlib>
#include <istream>
#include <ostream>

namespace almacen::presentacion {

namespace {

std::string recortar(const std::string& texto) {
    const auto inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return {};
    }
    const auto fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fin - inicio + 1);
}

}  // namespace

std::string Consola::leerLinea(const std::string& pregunta) {
    salida_ << pregunta << std::flush;
    std::string linea;
    if (!std::getline(entrada_, linea)) {
        throw FinDeEntrada();
    }
    return recortar(linea);
}

int Consola::leerEntero(const std::string& pregunta) {
    const std::string texto = leerLinea(pregunta);
    if (texto.empty()) {
        throw EntradaInvalida("Debe escribir un numero entero.");
    }
    errno = 0;
    char* fin = nullptr;
    const long valor = std::strtol(texto.c_str(), &fin, 10);
    if (*fin != '\0' || errno == ERANGE || valor < -2147483647L || valor > 2147483647L) {
        throw EntradaInvalida("\"" + texto + "\" no es un numero entero valido.");
    }
    return static_cast<int>(valor);
}

std::string Consola::leerTexto(const std::string& pregunta) {
    std::string texto = leerLinea(pregunta);
    if (texto.empty()) {
        throw EntradaInvalida("La respuesta no puede estar vacia.");
    }
    return texto;
}

}  // namespace almacen::presentacion
