namespace ServicioWeb
{
    public class Articulo
    {
        public int ID { get; set; }
        public string nombre { get; set; }
        public string descripcion { get; set; }
        public float precio { get; set; }
        public int existencia { get; set; }
        public int relevancia { get; set; }
        public byte[] foto { get; set; }
    }
}
