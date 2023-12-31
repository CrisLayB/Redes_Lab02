/*************************************************************
 * <h1>Universidad del Valle de Guatemala</h1>
 * <h2>Redes - Sección 10</h2>
 * <h3>Laboratorio 2</h3>
 * 
 * Java (Receptor)
 * 
 * @author: [
 *      Cristian Laynez,
 *      Mario de León
 * ]
 *************************************************************/

package hamming;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

class Colors {
    public static final String RESET = "\033[0m";  // Text Reset
    public static final String RED = "\033[0;31m";     // RED
    public static final String GREEN = "\033[0;32m";   // GREEN
}

public class Hamming {
    private static int PORT = 65432;

    public static String checkPlotBits(String plotBits) {
        int r = 0;
        while(Math.pow(2, r) <= plotBits.length()) r++;
        
        int[] bits = new int[r];
        
        for (int i = 0; i < r; i++) {
            int exp = (int)Math.pow(2, i);
            int controller = 0, bit = -1;
            boolean isOne = false;        
            
            for (int j = plotBits.length() - 1; j >= 0; j--) {
                controller++;
                if(controller >= exp){
                    isOne = !isOne;
                    controller = 0;
                }
                
                if(isOne){
                    int numBit = Integer.parseInt(plotBits.charAt(j) + "");
                    bit = (bit == -1) ? numBit : bit ^ numBit;
                }
            }

            bits[i] = bit;
        }

        String strBinary = "";
        for (int i = bits.length - 1; i >= 0 ; i--) strBinary += bits[i];

        int decimal = Integer.parseInt(strBinary, 2);
        
        if(decimal == 0){
            System.out.println("Todo ok");
            return plotBits;
        }

        if(decimal > plotBits.length()){
            System.out.println("Este error no es posible de corregir");
            return "-1";
        }        
                
        return fixPlot(plotBits, decimal);
    }

    private static String fixPlot(String plotBits, int decimal){
        System.out.println("Errores encontrados");
        StringBuilder strBuilder = new StringBuilder(plotBits);
        strBuilder.reverse();
        
        int pos = decimal - 1;

        String indicate = "";
        boolean stop = false;
        int aux = 0;
        for (int i = plotBits.length() - 1; i >= 0; i--) {
            char bit = plotBits.charAt(aux);
            String strBit = (i == pos) ? Colors.RED + bit + Colors.RESET : bit + "";
            System.out.print(strBit);
            if(i == pos) {
                indicate += "|";
                stop = true;
            }
            if(!stop) indicate += " ";
            aux++;
        }

        System.out.println();
        System.out.println(indicate);
        
        char flipBit = (strBuilder.charAt(pos) == '0') ? '1' : '0';
        strBuilder.setCharAt(pos, flipBit);

        System.out.println("Trama de bits Arreglado:");
        return new String(strBuilder.reverse());
    }
    
    public static void main(String[] args) {

        try {
            ServerSocket serverSocket = new ServerSocket(PORT);
            System.out.println("Server listening on PORT " + PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("\n===========================================");
                System.out.println("Client connected on the host: " + clientSocket.getInetAddress());

                PrintWriter output = new PrintWriter(clientSocket.getOutputStream(), true);
                BufferedReader input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                String plot = input.readLine();
                System.out.println("Input: " + plot);
                String plotChecked = checkPlotBits(plot);
                System.out.println("Output: " + plotChecked);

                output.println(plotChecked);
                output.flush();

                clientSocket.close();                
                System.out.println("===========================================\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}