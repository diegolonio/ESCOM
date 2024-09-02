namespace ServicioWeb
{
    public class ArticuloCarrito
    {
        public int ID { get; set; }
        public string nombre { get; set; }
        public string descripcion { get; set; }
        public float precio { get; set; }
        public byte[] foto { get; set; }
        public int cantidad { get; set; }
    }
}
