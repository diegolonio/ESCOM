import javax.swing.JOptionPane;

public class Cocina
{
    public static void preparar(Entrada entrada)
    {
        JOptionPane.showMessageDialog(null, "Preparando " + entrada.get_descripcion() + "...", null, JOptionPane.PLAIN_MESSAGE);
    }
    
    public static void preparar(Guisado guisado)
    {
        JOptionPane.showMessageDialog(null, "Preparando " + guisado.get_descripcion() + "...", null, JOptionPane.PLAIN_MESSAGE);
    }
    
    public static void preparar(Postre postre)
    {
        JOptionPane.showMessageDialog(null, "Preparando " + postre.get_descripcion() + "...", null, JOptionPane.PLAIN_MESSAGE);
    }
}
