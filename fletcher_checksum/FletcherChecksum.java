/*************************************************************
 * <h1>Universidad del Valle de Guatemala</h1>
 * <h2>Redes - Seccion 10</h2>
 * <h3>Laboratorio 2</h3>
 * 
 * Java (Fletcher Checksum)
 * 
 * @author: [
 *      Cristian Laynez,
 *      Mario de Leon
 * ]
 *************************************************************/

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

class Emisor {
    private static final int binarySize = 8;
    private String trama;
    private String checksum;
    private ArrayList<byte[]> bloquesDe8Bits;

    // Constructor de la clase Emisor
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

    // Obtener el checksum calculado
    public String getChecksum() {
        return checksum;
    }

    // Obtener el mensaje final con bloques y checksum
    public String getMensajeFinal() {
        StringBuilder bloques_finales = new StringBuilder();
        for (byte[] bloque : bloquesDe8Bits) {
            bloques_finales.append(byteToBinaryString(bloque)).append(" "); // Agregamos el espacio aqui
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

    // Funcion para simular ruido en el mensaje
    public static String simularRuido(String mensaje, double probabilidad) {
        StringBuilder mensajeConRuido = new StringBuilder();
        Random random = new Random();

        for (char c : mensaje.toCharArray()) {
            if (random.nextDouble() < probabilidad) {
                // Cambiar el bit si se cumple la probabilidad
                mensajeConRuido.append((c == '0') ? '1' : '0');
            } else {
                mensajeConRuido.append(c);
            }
        }

        return mensajeConRuido.toString();
    }

    // Calcular el checksum de Fletcher
    public static String calcularFletcherChecksum(ArrayList<byte[]> bloquesDe8Bits) {
        StringBuilder bloquesConcatenados = new StringBuilder();
        for (byte[] bloque : bloquesDe8Bits) {
            bloquesConcatenados.append(byteToBinaryString(bloque));
        }

        // Eliminar espacios de la cadena concatenada
        String bloquesSinEspacios = bloquesConcatenados.toString().replaceAll(" ", "");

        // Dividir la cadena en grupos de 8 bits usando split()
        String[] binary_blocks = bloquesSinEspacios.split("(?<=\\G.{8})");

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

    // Convertir un byte a una cadena binaria
    public static String byteToBinaryString(byte[] dataBytes) {
        StringBuilder binaryString = new StringBuilder();
        for (byte dataByte : dataBytes) {
            for (int i = 7; i >= 0; i--) {
                binaryString.append((dataByte >> i) & 1);
            }
        }
        return binaryString.toString();
    }

    // Verificar si una trama es binaria valida
    public static boolean esTramaBinariaValida(String trama) {
        // Expresion regular para verificar que la trama contiene solo 0s y 1s
        String regex = "^[01]+$";

        // Verificar si la trama coincide con la expresion regular
        return trama.matches(regex);
    }

    // Escribir el mensaje final en un archivo
    public void escribirMensajeFinalEnArchivo(String rutaArchivo) {
        try {
            // Abrir el archivo en modo escritura
            FileWriter writer = new FileWriter(rutaArchivo);

            // Obtener el mensaje final del emisor
            String mensajeFinal = getMensajeFinal();

            // Agregar espacios cada 8 caracteres
            StringBuilder mensajeConEspacios = new StringBuilder();
            for (int i = 0; i < mensajeFinal.length(); i += 8) {
                String grupo = mensajeFinal.substring(i, Math.min(i + 8, mensajeFinal.length()));
                mensajeConEspacios.append(grupo).append(" ");
            }

            // Escribir el mensaje final con espacios en el archivo
            writer.write(mensajeConEspacios.toString().trim());

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
        // *************** Capa aplicacion ***************
        int numMensajes = 10000; // Cantidad de mensajes a enviar
        int longitudMensajeMin = 10; // Longitud mínima de los mensajes
        int longitudMensajeMax = 100; // Longitud máxima de los mensajes

        for (int i = 0; i < numMensajes; i++) {
            // Generar un mensaje aleatorio de longitud variable
            int longitudMensaje = getRandomNumberInRange(longitudMensajeMin, longitudMensajeMax);
            String mensaje = generateRandomMessage(longitudMensaje);

            // *************** Capa presentacion ***************
            StringBuilder tramaBinaria = new StringBuilder();
            for (char c : mensaje.toCharArray()) {
                String asciiBinary = String.format("%8s", Integer.toBinaryString(c)).replace(' ', '0');
                tramaBinaria.append(asciiBinary);
            }

            System.out.println("\n***EMISOR***");
            Emisor emisor = new Emisor(tramaBinaria.toString());

            // *************** Capa enlace ***************
            System.out.println("Checksum Fletcher: " + emisor.getChecksum());
            System.out.println("Mensaje Final: " + emisor.getMensajeFinal());

            // Escribir el mensaje final en un archivo de texto
            String rutaArchivo = "mensaje_emisor.txt";
            emisor.escribirMensajeFinalEnArchivo(rutaArchivo);

            // *************** Capa ruido ***************
            // Simular ruido agregando cambios aleatorios en algunos bits
            String mensajeConRuido = Emisor.simularRuido(emisor.getMensajeFinal(), 0.01); // 10% de probabilidad de cambio

            // *************** Capa transmision ***************
            // Crear un socket del cliente para conectarse al receptor
            String host = "localhost"; // Receptor
            int port = 12345; // Puerto para la comunicacion

            try (Socket socket = new Socket(host, port)) {
                OutputStream output = socket.getOutputStream();
                output.write(mensajeConRuido.getBytes());
                System.out.println("Mensaje con ruido enviado al receptor.");
            } catch (IOException e) {
                System.out.println("Error al enviar el mensaje: " + e.getMessage());
            }
        }
    }

    // Genera un número aleatorio en un rango dado
    private static int getRandomNumberInRange(int min, int max) {
        Random r = new Random();
        return r.nextInt((max - min) + 1) + min;
    }

    // Genera un mensaje aleatorio de longitud dada
    private static String generateRandomMessage(int length) {
        Random r = new Random();
        StringBuilder message = new StringBuilder();
        for (int i = 0; i < length; i++) {
            char c = (char) (r.nextInt(26) + 'a');
            message.append(c);
        }
        System.out.println(message.toString());
        return message.toString();
    }
}
