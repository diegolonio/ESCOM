public class Point
{
    // Atributos    
    private int x;
    private int y;
    
    // Getters
    public int get_x()
    {
        return x;
    }

    public int get_y()
    {
        return y;
    }

    // Setters
    public void set_x(int nuevo_x)
    {
        x = nuevo_x;
    }

    public void set_y(int nuevo_y)
    {
        y = nuevo_y;
    }
    
    public double distancia(Point punto_2)
    {
        return Math.sqrt((punto_2.x - x) * (punto_2.x - x) + (punto_2.y - y) * (punto_2.y - y));
    }
}
