/*************************************************************
 * Universidad del Valle de Guatemala
 * Redes - Sección 10
 * Laboratorio 2
 * 
 * C++ (Receptor)
 * 
 * Autores: [
 *      Cristian Laynez,
 *      Mario de León
 * ]
 *************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Función para verificar el checksum del mensaje recibido
bool verificar_checksum(const std::string &mensaje_emisor) {
    // Obtener el checksum del mensaje
    std::string checksum_emisor = mensaje_emisor.substr(mensaje_emisor.length() - 8);

    // Obtener el mensaje original sin el checksum y sin espacios en blanco entre bloques de 8
    std::string mensaje_original = mensaje_emisor.substr(0, mensaje_emisor.length() - 8);
    mensaje_original.erase(std::remove_if(mensaje_original.begin(), mensaje_original.end(), ::isspace), mensaje_original.end());

    // Dividir la cadena en grupos de 8 bits
    std::vector<std::string> binary_blocks;
    for (size_t i = 0; i < mensaje_original.length(); i += 8) {
        binary_blocks.push_back(mensaje_original.substr(i, 8));
    }

    // Calcular el checksum del mensaje recibido
    std::string new_string = "00000000";
    for (const std::string &binary_block : binary_blocks) {
        int integer = std::stoi(binary_block, nullptr, 2);
        int total_int_value = std::stoi(new_string, nullptr, 2);
        int bit_sum = integer + total_int_value;

        if (bit_sum > 255) {
            bit_sum = (bit_sum & 255) + 1;
        }

        // Realizar la suma directamente en binario con longitud fija de 8 bits
        std::string binary_total_sum = std::bitset<8>(bit_sum).to_string();
        new_string = binary_total_sum;
    }

    // Realizar el complemento a uno de la suma final usando XOR (^)
    for (char &bit : new_string) {
        bit = (bit == '0') ? '1' : '0';
    }

    // Comparar los checksums
    return checksum_emisor == new_string;
}

// Función para obtener el mensaje original sin el checksum
std::string obtener_mensaje_original(const std::string &mensaje_emisor) {
    return mensaje_emisor.substr(0, mensaje_emisor.length() - 8);
}

// Función para decodificar el mensaje original y presentar la información
void decodificar_mensaje(const std::string &mensaje_original) {
    // *************** Capa presentacion ***************
    std::string mensaje_decodificado = "";
    // Dividir el mensaje original en grupos de 8 bits y decodificar
    for (size_t i = 0; i < mensaje_original.length(); i += 8) {
        std::string bloque = mensaje_original.substr(i, 8);
        int decimal_value = std::stoi(bloque, nullptr, 2);
        char caracter = static_cast<char>(decimal_value);
        mensaje_decodificado += caracter;
    }

    // *************** Capa aplicacion ***************
    std::cout << "Mensaje Decodificado: " << mensaje_decodificado << std::endl;
}

int main() {
    // *************** Capa transmision ***************
    std::cout << "\n***RECEPTOR***" << std::endl;
    // Configuración del socket del servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    bind(server_socket, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address));
    listen(server_socket, 1);

    std::cout << "Esperando conexiones del emisor..." << std::endl;

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_length = sizeof(client_address);
        int client_socket = accept(server_socket, reinterpret_cast<struct sockaddr *>(&client_address), &client_address_length);

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        std::string mensaje_emisor = buffer;
        std::cout << "Mensaje recibido del emisor: " << mensaje_emisor << std::endl;

        // *************** Capa enlace ***************
        // Verificar el checksum
        if (verificar_checksum(mensaje_emisor)) {
            std::cout << "El mensaje es válido." << std::endl;
            std::string mensaje_original = obtener_mensaje_original(mensaje_emisor);

            // Llamar a la función de decodificación
            decodificar_mensaje(mensaje_original);
        } else {
            std::cout << "El mensaje ha sido alterado durante la transmisión." << std::endl;
        }

        // Cerrar el socket del cliente
        close(client_socket);
    }

    // Cerrar el socket del servidor (esto solo se ejecutará si el bucle se detiene)
    close(server_socket);

    return 0;
}
