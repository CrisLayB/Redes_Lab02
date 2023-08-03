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
************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>

std::string obtenerChecksum(std::string mensaje) {
    // Obtener el checksum del mensaje
    std::string checksumEmisor = mensaje.substr(mensaje.length() - 8);
    std::cout << "Checksum recibido: " << checksumEmisor << std::endl;

    // Obtener el mensaje original sin el checksum y sin espacios en blanco entre bloques de 8
    std::string mensajeOriginal = mensaje.substr(0, mensaje.length() - 8);
    mensajeOriginal.erase(std::remove_if(mensajeOriginal.begin(), mensajeOriginal.end(), ::isspace), mensajeOriginal.end());
    std::cout << "Mensaje original: " << mensajeOriginal << std::endl;

    // Dividir la cadena en grupos de 8 bits
    std::vector<std::string> binary_blocks;
    for (int i = 0; i < mensajeOriginal.length(); i += 8) {
        std::string bloque = mensajeOriginal.substr(i, 8);
        binary_blocks.push_back(bloque);
    }

    // Calcular el checksum del mensaje recibido
    std::string new_string = "00000000";

    for (const std::string& binary_block : binary_blocks) {
        int integer = std::stoi(binary_block, nullptr, 2); 
        int total_int_value = std::stoi(new_string, nullptr, 2); 
        int bit_sum = integer + total_int_value;
        
        if (bit_sum > 255) {
            bit_sum = (bit_sum & 255) + 1;
        }

        // Realiza la suma directamente en binario con longitud fija de 8 bits
        std::string binary_total_sum = std::bitset<8>(bit_sum).to_string();
        new_string = binary_total_sum;
    }

    // Realiza el complemento a uno de la suma final usando XOR (^)
    for (char& bit : new_string) {
        bit = bit == '0' ? '1' : '0';
    }

    return new_string;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "No ingresaste el nombre del archivo de texto esperado" << std::endl;
        std::cout << "\nEjemplo de input esperado:" << std::endl;
        std::cout << "./receptor mensaje_emisor.txt" << std::endl;
        return 0;
    }

    std::string rutaArchivo = argv[1];
    std::ifstream archivo(rutaArchivo);

    if (!archivo) {
        std::cout << "Error al abrir el archivo: " << rutaArchivo << std::endl;
        return 0;
    }

    // Leer el mensaje final del archivo
    std::string mensajeFinal;
    std::getline(archivo, mensajeFinal);

    std::cout << "\n***RECEPTOR***" << std::endl;

    std::string checksumRecibido = obtenerChecksum(mensajeFinal);
    std::string checksumCalculado = checksumRecibido;

    if (checksumRecibido == mensajeFinal.substr(mensajeFinal.length() - 8)) {
        std::cout << "El mensaje es valido." << std::endl;
        std::cout << "Mensaje Original: " << mensajeFinal.substr(0, mensajeFinal.length() - 8) << std::endl;
    } else {
        std::cout << "El mensaje ha sido alterado durante la transmisión." << std::endl;
    }

    archivo.close();
    return 0;
}