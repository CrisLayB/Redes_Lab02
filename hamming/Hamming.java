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

class Colors {
    public static final String RESET = "\033[0m";  // Text Reset
    public static final String RED = "\033[0;31m";     // RED
    public static final String GREEN = "\033[0;32m";   // GREEN
}

public class Hamming {

    public static String checkPlotBits(String plotBits) {
        ArrayList<Integer> parityNums = new ArrayList<Integer>();

        int r = 0;
        while(Math.pow(2, r) <= plotBits.length()) {
            parityNums.add(0, (int)Math.pow(2, r) - 1);
            r++;
        }
        
        System.out.println("Input: " + plotBits + ", r = " + r);

        ArrayList<Integer> errorsFound = new ArrayList<Integer>();
        
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

                if(bit == -1) bit = (int)plotBits.charAt(j);
                
                if(isOne) bit = bit ^ (int)plotBits.charAt(j);
            }

            if(bit == 49) errorsFound.add(exp);
        }
        
        if(!errorsFound.isEmpty()){
            return fixPlot(plotBits, errorsFound, parityNums);
        }
        
        System.out.println("Output: Todo bien " + plotBits);
        originalPlot(plotBits, parityNums);
        return plotBits;
    }

    private static String fixPlot(String plotBits, ArrayList<Integer> errorsFound, ArrayList<Integer> parityNums){
        StringBuilder bitsResultBuilder = new StringBuilder(plotBits);            
        bitsResultBuilder.reverse();

        System.out.println("Output: Errores encontrados");
        ArrayList<String> errors = new ArrayList<String>();
        ArrayList<String> markErrors = new ArrayList<String>();
        for (int i = 0; i < bitsResultBuilder.length(); i++) {
            if(errorsFound.contains(i + 1)){
                errors.add(Colors.RED + bitsResultBuilder.charAt(i) + Colors.RESET);
                markErrors.add("|");
                continue;
            }                
            errors.add(bitsResultBuilder.charAt(i) + "");
            markErrors.add(" ");
        }
                
        for (int i = errors.size() - 1; i >= 0; i--) System.out.print(errors.get(i));
        System.out.println();
        for (int i = markErrors.size() - 1; i >= 0; i--) System.out.print(markErrors.get(i));
        
        for (int iterable_element : errorsFound) { // Fixing Code
            int num = iterable_element - 1;                
            char flipNum = bitsResultBuilder.charAt(num) == '1' ? '0' : '1';
            bitsResultBuilder.setCharAt(num, flipNum);
        }

        bitsResultBuilder.reverse();
        System.out.println("\nTrama de bits Arreglado: " + bitsResultBuilder);
        originalPlot(new String(bitsResultBuilder), parityNums);
        return new String(bitsResultBuilder);
    }

    private static void originalPlot(String plotToTransform, ArrayList<Integer> parityNums){
        StringBuilder strTemp = new StringBuilder(plotToTransform);
        strTemp.reverse();

        StringBuilder result = new StringBuilder();
        for (int i = 0; i < strTemp.length(); i++) {
            if(!parityNums.contains(i)) result.append(strTemp.charAt(i));
        }        
        
        System.out.println("Original: " + result.reverse());
    }
    
    public static void main(String[] args) {
        if(args.length != 2) {
            System.out.println("No ingresaste el trama de bits esperado");
            System.out.println("\nEjemplo de input esperado:");
            System.out.println("java hamming/Hamming 10101001110 10101001110");
            return;
        }        
        
        String plot = args[0];
        String plotTrue = args[1];

        String plotChecked = checkPlotBits(plot);

        String result = (plotChecked.equals(plotTrue)) ? "CORRECTO" : "INCORRECTO";
        System.out.println(result);
    }
}
