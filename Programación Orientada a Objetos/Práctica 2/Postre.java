public class Postre
{
    // Atributos
    private String descripcion;
    
    // Constructor
    public Postre()
    {
        this.descripcion = "No has elegido tu postre";
    }
    
    // Setters
    public void set_descripcion(String descripcion)
    {
        this.descripcion = descripcion;
    }
    
    // Getters
    public String get_descripcion()
    {
        return this.descripcion;
    }
}
