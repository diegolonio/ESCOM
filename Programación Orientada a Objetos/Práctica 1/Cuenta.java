public class Cuenta
{
    // Atributos
    private int ID;
    private String nombre;
    private String apellido;
    private String direccion;
    private float saldo;
    
    // Constructor
    public Cuenta(int ID, String nombre, String apellido, String direccion) {
        this.ID = ID;
        this.nombre = nombre;
        this.apellido = apellido;
        this.direccion = direccion;
        this.saldo = 1500.00f;
    }
    
    // Métodos
    
    // Getters
    public int get_ID() {
        return ID;
    }
    
    public String get_nombre() {
        return nombre;
    }
    
    public String get_apellido() {
        return apellido;
    }
    
    public String get_direccion() {
        return direccion;
    }
    
    public float get_saldo() {
        return saldo;
    }
    
    // Setters
    public void set_nombre(String nuevo_nombre) {
        nombre = nuevo_nombre;
    }
    
    public void set_apellido(String nuevo_apellido) {
        apellido = nuevo_apellido;
    }
    
    public void set_direccion(String nueva_direccion) {
        direccion = nueva_direccion;
    }
    
    public void set_saldo(float nuevo_saldo) {
        saldo = nuevo_saldo;
    }
}
