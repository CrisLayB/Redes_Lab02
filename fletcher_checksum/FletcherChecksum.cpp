/*********************
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
********************/

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>
#include <sstream>

#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

std::string obtenerChecksum(std::string mensaje) {
    std::string checksum_emisor = mensaje.substr(mensaje.length() - 8);
    std::cout << "Checksum recibido: " << checksum_emisor << std::endl;

    std::string mensaje_original = mensaje.substr(0, mensaje.length() - 8);
    mensaje_original.erase(std::remove_if(mensaje_original.begin(), mensaje_original.end(), ::isspace), mensaje_original.end());
    std::cout << "Mensaje original: " << mensaje_original << std::endl;

    std::vector<std::string> binary_blocks;
    for (size_t i = 0; i < mensaje_original.length(); i += 8) {
        std::string bloque = mensaje_original.substr(i, 8);
        binary_blocks.push_back(bloque);
    }

    std::string new_string = "00000000";

    for (const std::string& binary_block : binary_blocks) {
        int integer = std::stoi(binary_block, nullptr, 2); 
        int total_int_value = std::stoi(new_string, nullptr, 2); 
        int bit_sum = integer + total_int_value;
        
        if (bit_sum > 255) {
            bit_sum = (bit_sum & 255) + 1;
        }

        std::string binary_total_sum = std::bitset<8>(bit_sum).to_string();
        new_string = binary_total_sum;
    }

    for (char& bit : new_string) {
        bit = bit == '0' ? '1' : '0';
    }

    return new_string;
}

bool verificarChecksum(const std::string& mensaje_emisor) {
    std::string checksum_emisor = mensaje_emisor.substr(mensaje_emisor.length() - 8);
    std::string mensaje_original = mensaje_emisor.substr(0, mensaje_emisor.length() - 8);
    mensaje_original.erase(std::remove_if(mensaje_original.begin(), mensaje_original.end(), ::isspace), mensaje_original.end());

    std::vector<std::string> binary_blocks;
    for (size_t i = 0; i < mensaje_original.length(); i += 8) {
        std::string bloque = mensaje_original.substr(i, 8);
        binary_blocks.push_back(bloque);
    }

    std::string new_string = "00000000";

    for (const std::string& binary_block : binary_blocks) {
        int integer = std::stoi(binary_block, nullptr, 2); 
        int total_int_value = std::stoi(new_string, nullptr, 2); 
        int bit_sum = integer + total_int_value;
        
        if (bit_sum > 255) {
            bit_sum = (bit_sum & 255) + 1;
        }

        std::string binary_total_sum = std::bitset<8>(bit_sum).to_string();
        new_string = binary_total_sum;
    }

    for (char& bit : new_string) {
        bit = bit == '0' ? '1' : '0';
    }

    return new_string == checksum_emisor;
}

std::string obtenerMensajeOriginal(const std::string& mensaje_emisor) {
    std::string mensaje_original = mensaje_emisor.substr(0, mensaje_emisor.length() - 8);
    mensaje_original.erase(std::remove_if(mensaje_original.begin(), mensaje_original.end(), ::isspace), mensaje_original.end());
    return mensaje_original;
}

void decodificarMensaje(const std::string& mensaje_original) {
    std::string mensaje_decodificado = "";

    for (size_t i = 0; i < mensaje_original.length(); i += 8) {
        std::string bloque = mensaje_original.substr(i, 8);
        int decimal_value = std::stoi(bloque, nullptr, 2);
        char caracter = static_cast<char>(decimal_value);
        mensaje_decodificado += caracter;
    }

    std::cout << "Mensaje Decodificado: " << mensaje_decodificado << std::endl;
}

int main() {
    std::cout << "\n**RECEPTOR**" << std::endl;

    // Configuración del socket del servidor
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Port you used for communication
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Binding failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Listening failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Waiting for connections from the sender..." << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data" << std::endl;
            close(clientSocket);
            continue;
        }

        std::string mensaje_emisor(buffer);

        if (verificarChecksum(mensaje_emisor)) {
            std::cout << "The message is valid." << std::endl;
            std::string mensaje_original = obtenerMensajeOriginal(mensaje_emisor);
            decodificarMensaje(mensaje_original);
        } else {
            std::cout << "The message has been altered during transmission." << std::endl;
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}