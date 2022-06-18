public class Fonda
{
    public static void main(String[] args)
    {
        Entrada entrada = new Entrada();
        Guisado guisado = new Guisado();
        Postre postre = new Postre();
        
        String Entradas[] = {
            "Espagueti con Crema y Queso",
            "Sopa Aguada de Fideos",
            "Consomé de Pollo"
        };
        String Guisados[] = {
            "Albóndigas en Chipotle",
            "Pechuga de Pollo con Verduras",
            "Calabacitas en Salsa Verde con Carne de Puerco",
            "Bistec en Chile \"Guasilla\""
        };
        String Postres[] = {
            "Fresas con Crema",
            "Duraznos en Almíbar",
            "Helado de Sabores",
            "Pie de Limón"
        };
        int opcion = 0;
        
        while (true) {
            opcion = 0;
            
            while ((opcion < 1 || opcion > 3) && opcion != -1978) {
                opcion = Menu.Entradas(Entradas);
                
                if ((opcion < 1 || opcion > 3) && opcion != -1978) {
                    Menu.entrada_no_valida();
                }
            }
            
            if (opcion != -1978) {
                entrada.set_descripcion(Entradas[opcion-1]);
                opcion = 0;
            
                while ((opcion < 1 || opcion > 4) && opcion != -1978) {
                    opcion = Menu.Guisados(Guisados);
                    
                    if ((opcion < 1 || opcion > 4) && opcion != -1978) {
                        Menu.entrada_no_valida();
                    }
                }
            }
            
            if (opcion != -1978) {
                guisado.set_descripcion(Guisados[opcion-1]);
                opcion = 0;
                
                while ((opcion < 1 || opcion > 4) && opcion != -1978) {
                    opcion = Menu.Postres(Postres);
                    
                    if ((opcion < 1 || opcion > 4) && opcion != -1978) {
                        Menu.entrada_no_valida();
                    }
                }
            }
            
            if (opcion != -1978) {
                postre.set_descripcion(Postres[opcion-1]);
                Cocina.preparar(entrada);
                Cocina.preparar(guisado);
                Cocina.preparar(postre);
            }
            
            if (opcion == -1978) {
                if (Menu.confirmacion_salir("MENU")) {
                    break;
                }
            }
            
            if (!Menu.hacer_otra_orden()) {
                break;
            }
        }
    }
}
