/**
 * @file Consola.hpp
 * @brief Lectura y escritura validada por consola.
 * @author Santiago Caicedo
 */

#pragma once

#include <iosfwd>
#include <stdexcept>
#include <string>

namespace almacen::presentacion {

/**
 * @brief El usuario escribió algo que no se pudo interpretar.
 */
class EntradaInvalida : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/**
 * @brief Se terminó la entrada (Ctrl+D / Ctrl+Z o fin de archivo).
 */
class FinDeEntrada : public std::runtime_error {
public:
    FinDeEntrada() : std::runtime_error("Fin de la entrada.") {}
};

/**
 * @brief Envoltorio de flujos de entrada y salida.
 *
 * Recibe los flujos por el constructor en lugar de usar `std::cin` y
 * `std::cout` directamente. Así la interfaz se puede probar con
 * `std::istringstream` sin un usuario real.
 *
 * Lee línea por línea: los nombres con espacios funcionan y una entrada
 * mala no deja basura para la siguiente pregunta.
 */
class Consola {
public:
    /**
     * @param entrada Flujo desde el que se leen las respuestas.
     * @param salida Flujo donde se escriben los mensajes.
     */
    Consola(std::istream& entrada, std::ostream& salida) : entrada_(entrada), salida_(salida) {}

    /**
     * @brief Pregunta por un número entero.
     * @param pregunta Texto que se muestra antes de leer.
     * @return El número leído.
     * @throws EntradaInvalida si la respuesta no es un entero.
     * @throws FinDeEntrada si ya no hay más entrada.
     */
    int leerEntero(const std::string& pregunta);

    /**
     * @brief Pregunta por un texto no vacío.
     * @param pregunta Texto que se muestra antes de leer.
     * @return La línea leída, sin espacios en los extremos.
     * @throws EntradaInvalida si la respuesta está vacía.
     * @throws FinDeEntrada si ya no hay más entrada.
     */
    std::string leerTexto(const std::string& pregunta);

    /** @return El flujo de salida, para escribir con `<<`. */
    std::ostream& salida() noexcept { return salida_; }

private:
    std::string leerLinea(const std::string& pregunta);

    std::istream& entrada_;
    std::ostream& salida_;
};

}  // namespace almacen::presentacion
