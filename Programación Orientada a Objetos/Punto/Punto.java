public class Punto
{
    public static void main(String[] args)
    {    
        Point p1, p2;
        
        p1 = new Point ();
        p2 = new Point ();

        p1.set_x(9);
        p1.set_y(4);
        
        p2.set_x(10);
        p2.set_y(5);
        
        System.out.println(p2.distancia(p1));
    }
}
