public class Guisado
{
    // Atributos    
    private String descripcion;
    
    // Constructor
    public Guisado()
    {
        this.descripcion = "No has elegido tu guisado";
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
