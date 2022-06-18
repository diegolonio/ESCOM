public class Entrada
{
    // Atributos    
    private String descripcion;
    
    // Constructor
    public Entrada()
    {
        this.descripcion = "No has elegido tu entrada";
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
