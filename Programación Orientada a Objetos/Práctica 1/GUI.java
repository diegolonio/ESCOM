import javax.swing.JOptionPane;

public class GUI
{
    public int menu(Cuenta usuario) {
        String input = "";
        int opcion = 0;
        
        while (input.equals("")) {
            input = JOptionPane.showInputDialog("Buenos días, " + usuario.get_nombre() + "! Por favor elige la operación que deseas realizar:\n\n1. Ver saldo\n2. Depositar una cantidad\n3. Retirar una cantidad\n4. Salir\n\n");
            
            if (input == null) {
                input = "4";
                opcion = Integer.parseInt(input);
            } else if (input.length() == 0) {
                input = "";
            } else {
                input = input.trim();
                
                try {
                    opcion = Integer.parseInt(input);
                } catch (NumberFormatException e) {
                    this.entrada_no_valida();
                    input = "";
                }
            }
        }
        
        return opcion;
    }

    public void ver_saldo(Cuenta usuario) {
        if (usuario.get_saldo() < 1000.0f) {
            if (usuario.get_saldo() - 30.50f < 0.0f) {
                usuario.set_saldo(usuario.get_saldo() - usuario.get_saldo());
            } else {
                usuario.set_saldo(usuario.get_saldo() - 30.50f);
            }
        }
        
        JOptionPane.showMessageDialog(null, "Tu saldo es de $" + usuario.get_saldo());
    }
    
    public void depositar(Cuenta usuario) {
        String input = "";
        float monto = 0.0f;
        
        while (input.equals("")) {
            input = JOptionPane.showInputDialog("Ingresa el monto que deseas depositar:\n\n");
            
            if (input == null) {
                if (this.confirmacion_cancelar()) {
                    input = " ";
                } else {
                    input = "";
                }
            } else if (input.length() == 0) {
                input = "";
            } else {
                input = input.trim();
                
                try {
                    monto = Float.parseFloat(input);
                    usuario.set_saldo(usuario.get_saldo() + monto);
                    this.operacion_exitosa();
                } catch (NumberFormatException e) {
                    this.entrada_no_valida();
                    input = "";
                }
            }
        }
    }

    public void retirar (Cuenta usuario) {
        String input = "";
        float monto = 0.0f;
            
        while (input.equals("")) {
            input = JOptionPane.showInputDialog("Ingresa el monto que deseas retirar:\n\n");
            
            if (input == null) {
                if (this.confirmacion_cancelar()) {
                    input = " ";
                } else {
                    input = "";
                }
            } else if (input.length() == 0) {
                input = "";
            } else {
                input = input.trim();
                
                try {
                    monto = Float.parseFloat(input);
                    
                    if (monto < 0) {
                        this.entrada_no_valida();
                        input = "";
                    } else if (monto > usuario.get_saldo()) {
                        this.fondos_insuficientes();
                        input = "";
                    } else {
                        if (usuario.get_saldo() < 1000.0f) {
                            if (usuario.get_saldo() - 30.50f < 0.0f) {
                                usuario.set_saldo(usuario.get_saldo() - usuario.get_saldo());
                            } else {
                                usuario.set_saldo(usuario.get_saldo() - 30.50f);
                            }
                        }
                        
                        usuario.set_saldo(usuario.get_saldo() - monto);
                        this.operacion_exitosa();
                    }
                } catch (NumberFormatException e) {
                    this.entrada_no_valida();
                    input = "";
                }
            }
        }
    }

    public void opcion_no_valida() {
        JOptionPane.showMessageDialog(null, "Opción no válida, por favor vuelve a intentarlo.");
    }

    public void entrada_no_valida() {
        JOptionPane.showMessageDialog(null, "Entrada no válida,por favor vuelve a intentarlo.");
    }

    public void fondos_insuficientes() {
        JOptionPane.showMessageDialog(null, "No tienes los fondos suficientes para realizar esta operación.");
    }

    public void operacion_exitosa() {
        JOptionPane.showMessageDialog(null, "Operación realizada con éxito.");
    }

    public boolean confirmacion_cancelar() {
        boolean opcion = false;
        int nosi;
        
        nosi = JOptionPane.showConfirmDialog(null,"¿Estás seguro que quieres cancelar esta operación?", null, JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
        
        if (nosi == JOptionPane.YES_OPTION) {
            opcion = true;
        }
        
        return opcion;
    }
    
    public boolean confirmacion_salir() {
        boolean opcion = false;
        int nosi;
        
        nosi = JOptionPane.showConfirmDialog(null,"¿Estás seguro que quieres salir?", null, JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
        
        if (nosi == JOptionPane.YES_OPTION) {
            opcion = true;
        }
        
        return opcion;
    }
}
