import javax.swing.JOptionPane;

public class Menu
{
    public static int Entradas(String Entradas[])
    {
        int entrada = 0, index;
        String input = "";
        String mensaje = "Selecciona tu entrada:\n\n";
        
        for (index = 0; index < Entradas.length; index++) {
            mensaje = mensaje + (index+1) + ". " + Entradas[index] + "\n";
        }
        
        mensaje = mensaje + "\n";
        
        while (input.equals ("")) {
            input = JOptionPane.showInputDialog(null, mensaje, "MENU DEL DÍA - ENTRADA", JOptionPane.PLAIN_MESSAGE);
            
            if (input == null) {
                if (confirmacion_salir("ENTRADA")) {
                    input = "-1978";                    
                } else {
                    input = "";
                    continue;
                }
            }
            
            input = input.trim();
                
            try {
                entrada = Integer.parseInt(input);
                
                if (entrada <= 0 && entrada != -1978) {
                    entrada_no_valida();
                    input = "";
                }
            } catch (NumberFormatException e) {
                entrada_no_valida();
                   input = "";
            }
        }
        
        return entrada;
    }
    
    public static int Guisados(String Guisados[])
    {
        int guisado = 0, index;
        String input = "";
        String mensaje = "Selecciona tu guisado:\n\n";
        
        for (index = 0; index < Guisados.length; index++) {
            mensaje = mensaje + (index+1) + ". " + Guisados[index] + "\n";
        }
        
        mensaje = mensaje + "\n";
        
        while (input.equals ("")) {
            input = JOptionPane.showInputDialog(null, mensaje, "MENU DEL DÍA - GUISADO", JOptionPane.PLAIN_MESSAGE);
            
            if (input == null) {
                if (confirmacion_salir("GUISADO")) {
                    input = "-1978";                    
                } else {
                    input = "";
                    continue;
                }
            }
            
            input = input.trim();
                
            try {
                guisado = Integer.parseInt(input);
                
                if (guisado <= 0 && guisado != -1978) {
                    entrada_no_valida();
                    input = "";
                }
            } catch (NumberFormatException e) {
                entrada_no_valida();
                   input = "";
            }
        }
        
        return guisado;
    }
    
    public static int Postres(String Postres[])
    {
        int postre = 0, index;
        String input = "";
        String mensaje = "Selecciona tu postre:\n\n";
        
        for (index = 0; index < Postres.length; index++) {
            mensaje = mensaje + (index+1) + ". " + Postres[index] + "\n";
        }
        
        mensaje = mensaje + "\n";
        
        while (input.equals ("")) {
            input = JOptionPane.showInputDialog(null, mensaje, "MENU DEL DÍA - POSTRE", JOptionPane.PLAIN_MESSAGE);
            
            if (input == null) {
                if (confirmacion_salir("POSTRE")) {
                    input = "-1978";                    
                } else {
                    input = "";
                    continue;
                }
            }
            
            input = input.trim();
                
            try {
                postre = Integer.parseInt(input);
                
                if (postre <= 0 && postre != -1978) {
                    entrada_no_valida();
                    input = "";
                }
            } catch (NumberFormatException e) {
                entrada_no_valida();
                   input = "";
            }
        }
        
        return postre;
    }
    
    public static void entrada_no_valida()
    {
        JOptionPane.showMessageDialog(null, "Entrada no válida, inténtalo de nuevo por favor.");
    }
    
    public static boolean confirmacion_salir(String seccion)
    {
        boolean opcion = false;
        int nosi;
        
        nosi = JOptionPane.showConfirmDialog(null,"¿Estás seguro que quieres salir?", 
                "MENU DEL DÍA - " + seccion, JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
        
        if (nosi == JOptionPane.YES_OPTION) {
            opcion = true;
        }
        
        return opcion;
    }

    public static boolean hacer_otra_orden()
    {
        boolean opcion = false;
        int nosi;
        
        nosi = JOptionPane.showConfirmDialog(null,"¿Quieres pedir otra comida?", 
                null, JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
        
        if (nosi == JOptionPane.YES_OPTION) {
            opcion = true;
        }
        
        return opcion;
    }
}
