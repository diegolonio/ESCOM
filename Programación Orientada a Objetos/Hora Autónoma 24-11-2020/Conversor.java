import javax.swing.JOptionPane;
import java.text.DecimalFormat;

public class Conversor
{
    public static void main(String[] args) {
        String opcion = "", dato = "";
        DecimalFormat formato = new DecimalFormat("#.00");
        float celsius, fahrenheit, pesos, dolares, centimetros, pulgadas;
        
        while (!opcion.equals("4")) {
            opcion = JOptionPane.showInputDialog("Elige una de las siguientes opciones:\n1 - Convertir grados Fahrenheit a Celsius\n2 - Convertir MXN a USD\n3 - Convertir Pulgadas a Centímetros\n4 - Salir");
            
            if (opcion == null) {
                break;
            }
            
            opcion = opcion.trim();
            
            switch (opcion) {
                case "1":
                    while (dato.equals("")) {
                        dato = JOptionPane.showInputDialog("Ingresa los grados Celsius que quieres convertir:\n");
                        
                        if (dato == null) {
                            break;
                        }
                    }
                    
                    if (dato == null) {
                        dato = "";
                        break;
                    }
                    
                    dato = dato.trim();
                    
                    celsius = Float.parseFloat(dato);
                    fahrenheit = celsius * 9 / 5 + 32;
                    JOptionPane.showMessageDialog(null, formato.format(celsius) + "° Celsius = " + formato.format(fahrenheit) + "° Fahrenheit");
                    dato = "";
                    break;
                case "2":
                    while (dato.equals("")) {
                        dato = JOptionPane.showInputDialog("Ingresa el monto que quieres convertir:\n");
                        
                        if (dato == null) {
                            break;
                        }
                    }
                    
                    if (dato == null) {
                        dato = "";
                        break;
                    }
                    
                    dato = dato.trim();
                    
                    pesos = Float.parseFloat(dato);
                    dolares = pesos / 20.03f;
                    JOptionPane.showMessageDialog(null, formato.format(pesos) + " MXN = " + formato.format(dolares) + " USD\n");
                    dato = "";
                    break;
                case "3":
                    while (dato.equals("")) {
                        dato = JOptionPane.showInputDialog("Ingresa la distancia que quieres convertir:\n");
                        
                        if (dato == null) {
                            break;
                        }
                    }
                    
                    if (dato == null) {
                        dato = "";
                        break;
                    }
                    
                    dato = dato.trim();
                    
                    centimetros = Float.parseFloat(dato);
                    pulgadas = centimetros / 2.54f;
                    JOptionPane.showMessageDialog(null, formato.format(centimetros) + " cm = " + formato.format(pulgadas) + " in\n");
                    dato = "";
                    break;
                case "4":
                    break;
                default:
                    JOptionPane.showMessageDialog(null, "Opción " + opcion + " no válida, vuelve a intentarlo\n");
                    break;
            }    
        }
    }
}
