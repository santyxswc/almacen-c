/**
 * @file CasosUsoClientes.cpp
 * @brief Implementación de los casos de uso de clientes.
 * @author Santiago Caicedo
 */

#include "aplicacion/casos_uso/CasosUsoClientes.hpp"

#include "aplicacion/Mapeo.hpp"
#include "dominio/Excepciones.hpp"

namespace almacen::aplicacion {

ClienteDto CrearCliente::ejecutar(int id, const std::string& nombre) {
    if (clientes_.buscar(id)) {
        throw dominio::EntidadDuplicada("cliente", id);
    }
    const dominio::Cliente cliente(id, nombre);
    clientes_.guardar(cliente);
    return aDto(cliente);
}

ClienteDto CrearCliente::ejecutar(const std::string& nombre) {
    return ejecutar(clientes_.siguienteId(), nombre);
}

std::vector<ClienteDto> ListarClientes::ejecutar() const {
    std::vector<ClienteDto> resultado;
    for (const auto& cliente : clientes_.listar()) {
        resultado.push_back(aDto(cliente));
    }
    return resultado;
}

}  // namespace almacen::aplicacion
