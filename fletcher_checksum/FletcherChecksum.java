/*************************************************************
 * <h1>Universidad del Valle de Guatemala</h1>
 * <h2>Redes - Sección 10</h2>
 * <h3>Laboratorio 2</h3>
 * 
 * Java (Fletcher Checksum)
 * 
 * @author: [
 *      Cristian Laynez,
 *      Mario de León
 * ]
 *************************************************************/

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

class Emisor {
    private static final int binarySize = 8;
    private String trama;
    private String checksum;
    private ArrayList<byte[]> bloquesDe8Bits;

    public Emisor(String trama) {
        if (!esTramaBinariaValida(trama)) {
            throw new IllegalArgumentException("Error: La trama debe contener solo 0s y 1s.");
        }

        // Añadir ceros si es necesario
        int left_zeros = binarySize - (trama.length() % binarySize);
        if (left_zeros > 0 && left_zeros < binarySize) {
            StringBuilder tramaBuilder = new StringBuilder(trama);
            for (int i = 0; i < left_zeros; i++) {
                tramaBuilder.insert(0, '0');
            }
            this.trama = tramaBuilder.toString();
        } else {
            this.trama = trama;
        }

        // Convertir la trama binaria a un arreglo de bytes
        byte[] bytes = new byte[this.trama.length() / binarySize];
        for (int i = 0; i < this.trama.length(); i += binarySize) {
            String byteStr = this.trama.substring(i, i + binarySize);
            bytes[i / binarySize] = (byte) Integer.parseInt(byteStr, 2);
        }

        // Almacenar los bloques de 8 bits en un ArrayList
        bloquesDe8Bits = new ArrayList<>();
        for (int i = 0; i < bytes.length; i++) {
            byte[] bloque = new byte[1];
            bloque[0] = bytes[i];
            bloquesDe8Bits.add(bloque);
        }

        // Calcular el checksum Fletcher
        this.checksum = calcularFletcherChecksum(bloquesDe8Bits);
    }

    public String getChecksum() {
        return checksum;
    }

    public String getMensajeFinal() {
        StringBuilder bloques_finales = new StringBuilder();
        for (byte[] bloque : bloquesDe8Bits) {
            bloques_finales.append(byteToBinaryString(bloque)).append(" "); // Agregamos el espacio aquí
        }
        bloques_finales.append(checksum);

        String mensaje = bloques_finales.toString();
        StringBuilder final_mensaje = new StringBuilder();
        // Concatenar los grupos de 8 caracteres al StringBuilder
        for (int i = 0; i < mensaje.length(); i += 9) { // Incrementamos en 9 para saltar el espacio
            String grupo = mensaje.substring(i, Math.min(i + 8, mensaje.length()));
            final_mensaje.append(grupo);
        }
        return final_mensaje.toString();
    }



    public static String calcularFletcherChecksum(ArrayList<byte[]> bloquesDe8Bits) {
        StringBuilder bloquesConcatenados = new StringBuilder();
        for (byte[] bloque : bloquesDe8Bits) {
            bloquesConcatenados.append(byteToBinaryString(bloque));
        }
        System.out.println("Bloques Concatenados: " + bloquesConcatenados);

        // Eliminar espacios de la cadena concatenada
        String bloquesSinEspacios = bloquesConcatenados.toString().replaceAll(" ", "");

        // Dividir la cadena en grupos de 8 bits usando split()
        String[] binary_blocks = bloquesSinEspacios.split("(?<=\\G.{8})");

        // Mostrar los grupos de 8 bits almacenados en el arreglo String[]
        for (int i = 0; i < binary_blocks.length; i++) {
            System.out.println("Bloque " + i + ": " + binary_blocks[i]);
        }

        StringBuilder new_string = new StringBuilder("00000000"); // Initialize the sum to all zeros

        for (String binary_block : binary_blocks) {
            int integer = Integer.parseInt(binary_block, 2); 
            int total_int_value = Integer.parseInt(new_string.toString(), 2); 
            int bit_sum = integer + total_int_value;
            
            if (bit_sum > 255) {
            	bit_sum = (bit_sum & 255) + 1;
            }

         // Realiza la suma directamente en binario con longitud fija de 8 bits
            String binary_total_sum = String.format("%8s", Integer.toBinaryString(bit_sum)).replace(' ', '0');
            new_string = new StringBuilder(binary_total_sum);
        }

        // Realiza el complemento a uno de la suma final usando XOR (^)
        for (int i = 0; i < new_string.length(); i++) {
            char bit = new_string.charAt(i);
            new_string.setCharAt(i, bit == '0' ? '1' : '0');
        }
        
        return new_string.toString();
    }

    // Function to convert byte to binary string
    public static String byteToBinaryString(byte[] dataBytes) {
        StringBuilder binaryString = new StringBuilder();
        for (byte dataByte : dataBytes) {
            for (int i = 7; i >= 0; i--) {
                binaryString.append((dataByte >> i) & 1);
            }
        }
        return binaryString.toString();
    }

    public static boolean esTramaBinariaValida(String trama) {
        // Expresion regular para verificar que la trama contiene solo 0s y 1s
        String regex = "^[01]+$";

        // Verificar si la trama coincide con la expresion regular
        return trama.matches(regex);
    }
    
    public void escribirMensajeFinalEnArchivo(String rutaArchivo) {
        try {
            // Abrir el archivo en modo escritura
            FileWriter writer = new FileWriter(rutaArchivo);

            // Obtener el mensaje final del emisor
            String mensajeFinal = getMensajeFinal();

            // Escribir el mensaje final en el archivo
            writer.write(mensajeFinal);

            // Cerrar el archivo
            writer.close();

            System.out.println("El mensaje final se ha escrito en el archivo: " + rutaArchivo);
        } catch (IOException e) {
            System.out.println("Error al escribir el archivo: " + e.getMessage());
        }
    }

}

public class FletcherChecksum {
    public static void main(String[] args) {
        String trama = "1010100100111001";
        System.out.println("\n***EMISOR***");
        Emisor emisor = new Emisor(trama);

        System.out.println("Checksum Fletcher: " + emisor.getChecksum());
        System.out.println("Mensaje Final: " + emisor.getMensajeFinal());
        
        // Escribir el mensaje final en un archivo de texto
        String rutaArchivo = "mensaje_emisor.txt";
        emisor.escribirMensajeFinalEnArchivo(rutaArchivo);
    }
}

