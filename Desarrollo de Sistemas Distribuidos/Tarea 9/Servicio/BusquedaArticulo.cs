using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.IO;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using MySql.Data.MySqlClient;
using Newtonsoft.Json;

namespace ServicioWeb
{
    public static class BusquedaArticulo
    {
        [Function("busqueda_articulo")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "busqueda_articulo")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("BusquedaArticulo");

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                ParamBusqueda p = JsonConvert.DeserializeObject<ParamBusqueda>(requestBody);
                Busqueda busqueda = p.busqueda;

                string[] palabras = EliminarPreposiciones(busqueda.busqueda);
                string consultaSQL = GenerarConsultaSQL(palabras, busqueda.orden);

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");
                List<Articulo> articulos = new List<Articulo>();

                await using (MySqlConnection conexion = new MySqlConnection(connectionString))
                {
                    await conexion.OpenAsync();
                    await using (MySqlCommand cmd = new MySqlCommand(consultaSQL, conexion))
                    {
                        await using (DbDataReader reader = await cmd.ExecuteReaderAsync())
                        {
                            while (await reader.ReadAsync())
                            {
                                Articulo articulo = new Articulo
                                {
                                    ID = reader.GetInt32(reader.GetOrdinal("ID")),
                                    nombre = reader.GetString(reader.GetOrdinal("nombre")),
                                    descripcion = reader.GetString(reader.GetOrdinal("descripcion")),
                                    precio = reader.GetFloat(reader.GetOrdinal("precio")),
                                    existencia = reader.GetInt32(reader.GetOrdinal("existencia")),
                                    relevancia = reader.GetInt32(reader.GetOrdinal("relevancia")),
                                    foto = reader["imagen"] as byte[]
                                };
                                articulos.Add(articulo);
                            }
                        }
                    }
                }

                if (articulos.Count == 0)
                {
                    return await ConstructBadRequestResponseAsync(req, "Busqueda sin resultados.");
                }

                var response = req.CreateResponse(HttpStatusCode.OK);
                await response.WriteStringAsync(JsonConvert.SerializeObject(articulos), Encoding.UTF8);
                return response;
            }
            catch (Exception ex)
            {
                logger.LogError($"Error general: {ex.Message}");
                return await ConstructBadRequestResponseAsync(req, ex.Message);
            }
        }

        public static string[] EliminarPreposiciones(string busqueda)
        {
            List<string> preposiciones = new List<string> { "a", "ante", "bajo", "cabe", "con", "contra", "de", "desde", "en", "entre", "hacia", "hasta", "para", "por", "según", "sin", "so", "sobre", "tras" };
            string[] palabras = busqueda.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            List<string> palabrasFiltradas = new List<string>();

            foreach (var palabra in palabras)
            {
                if (!preposiciones.Contains(palabra.ToLower()))
                {
                    palabrasFiltradas.Add(palabra);
                }
            }

            return palabrasFiltradas.ToArray();
        }

        public static string GenerarConsultaSQL(string[] palabras, int orden)
        {
            StringBuilder whereClause = new StringBuilder();

            for (int i = 0; i < palabras.Length; i++)
            {
                if (i > 0)
                {
                    whereClause.Append(" OR ");
                }
                whereClause.Append($"nombre LIKE '%{palabras[i]}%' OR descripcion LIKE '%{palabras[i]}%'");
            }

            string consultaSQL = $"SELECT articulos.*, imagenes.imagen FROM articulos LEFT JOIN imagenes ON articulos.ID = imagenes.ID WHERE ({whereClause})";

            switch (orden)
            {
                case 1:
                    consultaSQL += " ORDER BY precio ASC";
                    break;
                case 2:
                    consultaSQL += " ORDER BY precio DESC";
                    break;
                case 3:
                    consultaSQL += " ORDER BY relevancia DESC";
                    break;
                default:
                    break;
            }

            return consultaSQL;
        }

        private static async Task<HttpResponseData> ConstructBadRequestResponseAsync(HttpRequestData req, string message)
        {
            var response = req.CreateResponse(HttpStatusCode.BadRequest);
            response.Headers.Add("Content-Type", "application/json");
            await response.WriteStringAsync(JsonConvert.SerializeObject(new { error = message }), Encoding.UTF8);
            return response;
        }
    }
}