/**
 * @file MiniTest.hpp
 * @brief Marco de pruebas mínimo, sin dependencias externas.
 * @author Santiago Caicedo
 *
 * Cada prueba se declara con `PRUEBA(nombre) { ... }` y se registra sola.
 * `main` (en `main_pruebas.cpp`) las ejecuta todas y devuelve 1 si alguna falla,
 * así CTest y GitHub Actions detectan el error.
 */

#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace minitest {

/** @brief Una prueba registrada. */
struct Prueba {
    std::string nombre;            ///< Nombre para el reporte.
    std::function<void()> cuerpo;  ///< Código de la prueba.
};

/** @brief Falla de una verificación. */
struct Falla {
    std::string mensaje;  ///< Descripción de lo que falló.
};

/** @return El registro global de pruebas. */
inline std::vector<Prueba>& registro() {
    static std::vector<Prueba> pruebas;
    return pruebas;
}

/** @brief Registra una prueba al construirse (se usa desde la macro PRUEBA). */
struct Registrador {
    Registrador(const char* nombre, std::function<void()> cuerpo) { registro().push_back({nombre, std::move(cuerpo)}); }
};

/**
 * @brief Ejecuta todas las pruebas registradas.
 * @return 0 si todas pasaron, 1 si alguna falló.
 */
inline int ejecutarTodas() {
    int fallidas = 0;
    for (const auto& prueba : registro()) {
        try {
            prueba.cuerpo();
            std::cout << "[ OK ] " << prueba.nombre << '\n';
        } catch (const Falla& falla) {
            ++fallidas;
            std::cout << "[FALLA] " << prueba.nombre << ": " << falla.mensaje << '\n';
        } catch (const std::exception& error) {
            ++fallidas;
            std::cout << "[FALLA] " << prueba.nombre << ": excepcion inesperada: " << error.what() << '\n';
        }
    }
    std::cout << '\n'
              << registro().size() - static_cast<std::size_t>(fallidas) << " de " << registro().size()
              << " pruebas pasaron.\n";
    return fallidas == 0 ? 0 : 1;
}

}  // namespace minitest

#define MINITEST_CONCAT_(a, b) a##b
#define MINITEST_CONCAT(a, b) MINITEST_CONCAT_(a, b)

/** @brief Declara y registra una prueba. */
#define PRUEBA(nombre)                                                                       \
    static void nombre();                                                                    \
    static const minitest::Registrador MINITEST_CONCAT(registro_, nombre)(#nombre, &nombre); \
    static void nombre()

/** @brief Verifica que una condición sea verdadera. */
#define VERIFICAR(condicion)                                                                                \
    do {                                                                                                    \
        if (!(condicion)) {                                                                                 \
            throw minitest::Falla{std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " #condicion}; \
        }                                                                                                   \
    } while (false)

/** @brief Verifica que dos valores sean iguales y muestra ambos si no lo son. */
#define VERIFICAR_IGUAL(esperado, obtenido)                                                                    \
    do {                                                                                                       \
        const auto esperado_ = (esperado);                                                                     \
        const auto obtenido_ = (obtenido);                                                                     \
        if (!(esperado_ == obtenido_)) {                                                                       \
            std::ostringstream mensaje_;                                                                       \
            mensaje_ << __FILE__ << ":" << __LINE__ << " se esperaba " #esperado " pero se obtuvo " #obtenido; \
            throw minitest::Falla{mensaje_.str()};                                                             \
        }                                                                                                      \
    } while (false)

/** @brief Verifica que una expresión lance una excepción del tipo indicado. */
#define VERIFICAR_LANZA(expresion, TipoExcepcion)                                            \
    do {                                                                                     \
        bool lanzo_ = false;                                                                 \
        try {                                                                                \
            (void)(expresion);                                                               \
        } catch (const TipoExcepcion&) {                                                     \
            lanzo_ = true;                                                                   \
        }                                                                                    \
        if (!lanzo_) {                                                                       \
            throw minitest::Falla{std::string(__FILE__) + ":" + std::to_string(__LINE__) +   \
                                  " se esperaba " #TipoExcepcion " al evaluar " #expresion}; \
        }                                                                                    \
    } while (false)
