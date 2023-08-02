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

import java.util.ArrayList;

public class Hamming {

    private static void checkPlotBits(String plotBits, int parityBits) {
        System.out.println("Input: " + plotBits + " and " + parityBits);

        ArrayList<Integer> errorsFound = new ArrayList<Integer>();
        
        for (int i = 0; i < parityBits; i++) {
            int exp = (int)Math.pow(2, i);
            int controller = 0;
            boolean isOne = false;
            int bit = -1;
            System.out.println("2^" + i + " : " + exp);
            
            for (int j = plotBits.length() - 1; j >= 0; j--) {
                controller++;
                if(controller >= exp){
                    isOne = !isOne;
                    controller = 0;
                }

                if(bit == -1) bit = (int)plotBits.charAt(j);
                
                if(isOne){
                    bit = bit ^ (int)plotBits.charAt(j);
                    System.out.println("--> " + plotBits.charAt(j));
                }
            }

            System.out.println("RESULT: " + bit + " and " + (char)bit);
            if(bit == 49){
                errorsFound.add(exp);
            }
        }

        if(!errorsFound.isEmpty()){
            System.out.println("Output: Errores encontrados");
            for (int iterable_element : errorsFound) {
                System.out.println("==> " + iterable_element + " bit: " + plotBits.charAt(iterable_element));                  
            }
            return;
        }
        
        System.out.println("Output: Todo bien " + plotBits);
    }
    
    public static void main(String[] args)
    {
        if(args.length != 2){
            System.out.println("No ingresaste el trama de bits esperado");
            System.out.println("\nEjemplo de input esperado:");
            System.out.println("java hamming/Hamming plot_bits 10101001110 4");
            return;
        }
        
        if(!args[1].chars().allMatch( Character::isDigit )){
            System.out.println("El segundo argumento de numero de paridad no es int");
            return;
        }
        
        checkPlotBits(args[0], Integer.parseInt(args[1]));
    }
}
