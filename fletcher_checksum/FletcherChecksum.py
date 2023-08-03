'''
 * Universidad del Valle de Guatemala
 * Redes - Sección 10
 * Laboratorio 2
 * 
 * Python (Receptor)
 * 
 * Autores: [
 *      Cristian Laynez,
 *      Mario de León
 * ]
'''


# Receptor.py
def verificar_checksum(mensaje_emisor):
    # Obtener el checksum del mensaje
    checksum_emisor = mensaje_emisor[-8:].replace(" ", "")
    print("Checksum recibido:", checksum_emisor)

    # Obtener el mensaje original sin el checksum y sin espacios en blanco entre bloques de 8
    mensaje_original = mensaje_emisor[:-8].replace(" ", "")
    print("Mensaje original:", mensaje_original)

    # Dividir la cadena en grupos de 8 bits
    binary_blocks = [mensaje_original[i:i + 8]
                     for i in range(0, len(mensaje_original), 8)]

    # Calcular el checksum del mensaje recibido
    new_string = "00000000"

    for binary_block in binary_blocks:
        integer = int(binary_block, 2)
        total_int_value = int(new_string, 2)
        bit_sum = integer + total_int_value

        if bit_sum > 255:
            bit_sum = (bit_sum & 255) + 1

        # Realizar la suma directamente en binario con longitud fija de 8 bits
        binary_total_sum = format(bit_sum, '08b')
        new_string = binary_total_sum

    # Realizar el complemento a uno de la suma final usando XOR (^)
    new_string = "".join("1" if bit == "0" else "0" for bit in new_string)

    # Comparar los checksums
    return checksum_emisor == new_string


def obtener_mensaje_original(mensaje_emisor):
    # Obtener el mensaje original sin el checksum
    return mensaje_emisor[:-8].replace(" ", "")


def main():
    print("\n***RECEPTOR***")
    # Leer el mensaje del archivo de texto
    with open("mensaje_emisor.txt", "r") as file:
        mensaje_emisor = file.read()

    if verificar_checksum(mensaje_emisor):
        print("El mensaje es valido.")
        print("Mensaje Original:", obtener_mensaje_original(mensaje_emisor))
    else:
        print("El mensaje ha sido alterado durante la transmision.")


if __name__ == "__main__":
    main()
