public class Orden
{
    // Atributos
    private String entrada;
    private String guisado;
    private String postre;
    
    // Constructor
    public Orden()
    {
        this.entrada = "No has seleccionado tu entrada";
        this.guisado = "No has seleccionado tu guisado.";
        this.postre = "No has seleccionado tu postre.";
    }
    
    // Setters
    public void set_entrada(Entrada entrada)
    {
        this.entrada = entrada.get_descripcion();
    }
    
    public void set_guisado(Guisado guisado)
    {
        this.guisado = guisado.get_descripcion();
    }
    
    public void set_postre(Postre postre)
    {
        this.postre = postre.get_descripcion();
    }
    
    // Guetter
    public String get_entrada()
    {
        return this.entrada;
    }
    
    public String get_guisado()
    {
        return this.guisado;
    }
    
    public String get_postre()
    {
        return this.postre;
    }
}
