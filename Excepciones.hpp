#ifndef EXCEPCIONES_HPP
#define EXCEPCIONES_HPP

#include <exception>
#include <string>

class ClienteNoCreadoException : public std::exception {
public:
	const char* what() const noexcept override {
		return "Debe crear un cliente primero.";
	}
};

class PedidoNoCreadoException : public std::exception {
public:
	const char* what() const noexcept override {
		return "Debe crear un cliente y un pedido primero.";
	}
};

class EntradaInvalidaException : public std::exception {
private:
	std::string message_;
public:
	EntradaInvalidaException(const std::string& message) : message_(message) {}
	
	const char* what() const noexcept override {
		return message_.c_str();
	}
};

#endif
