public class Practica1
{
    public static void main(String[] args) {
        Cuenta usuario = new Cuenta(124, "Diego", "Apolonio", "Av. de la Suerte 590A");
        GUI interfaz = new GUI();
        int opcion = 0;
        
        while (opcion != 4) {
            opcion = interfaz.menu(usuario);
            
            switch (opcion) {
                case 1:    
                    interfaz.ver_saldo(usuario);
                    break;
                case 2:
                    interfaz.depositar(usuario);
                    break;
                case 3:
                    interfaz.retirar(usuario);
                    break;
                case 4:
                    if (!interfaz.confirmacion_salir()) {
                        opcion = 0;
                    }
                    break;
                default:
                    interfaz.opcion_no_valida();
                    break;
            }
        }
    }
}
