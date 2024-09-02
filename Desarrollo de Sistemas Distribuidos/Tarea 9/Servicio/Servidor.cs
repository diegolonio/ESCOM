using Azure.Storage.Blobs;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Configuration;
using Azure.Storage.Blobs.Models;

namespace ServicioWeb
{
    public class Servidor
    {
        private readonly ILogger _logger;
        private readonly IConfiguration _configuration;

        public Servidor(ILoggerFactory loggerFactory, IConfiguration configuration)
        {
            _logger = loggerFactory.CreateLogger<Servidor>();
            _configuration = configuration;
        }

        [Function("Servidor")]
        public async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", Route = null)] HttpRequest req)
        {
            try
            {
                string? path = req.Query["nombre"];
                bool descargar = req.Query["descargar"] == "si";
                string? storageConnectionString = _configuration["AzureWebJobsStorage"];
                string? containerName = "data";

                if (string.IsNullOrEmpty(path))
                {
                    return new BadRequestObjectResult("Please provide a file name in the query string.");
                }

                try
                {
                    BlobServiceClient blobServiceClient = new BlobServiceClient(storageConnectionString);
                    BlobContainerClient containerClient = blobServiceClient.GetBlobContainerClient(containerName);
                    BlobClient blobClient = containerClient.GetBlobClient(path);

                    if (!await blobClient.ExistsAsync())
                    {
                        return new NotFoundResult();
                    }

                    BlobDownloadInfo download = await blobClient.DownloadAsync();
                    byte[] contenido;
                    using (MemoryStream ms = new MemoryStream())
                    {
                        await download.Content.CopyToAsync(ms);
                        contenido = ms.ToArray();
                    }

                    string? nombre = Path.GetFileName(path);

                    if (nombre == null)
                    {
                        return new StatusCodeResult(500);
                    }

                    string? tipo_mime = MimeMapping.GetMimeMapping(nombre);

                    DateTime fecha_modificacion = download.Details.LastModified.UtcDateTime;
                    string? fecha = req.Headers["If-Modified-Since"];

                    if (!string.IsNullOrEmpty(fecha) && DateTime.TryParse(fecha, out DateTime fechaParsed) && fechaParsed >= fecha_modificacion)
                    {
                        return new StatusCodeResult(304);
                    }

                    if (descargar)
                    {
                        return new FileContentResult(contenido, tipo_mime)
                        {
                            FileDownloadName = nombre
                        };
                    }
                    else
                    {
                        return new FileContentResult(contenido, tipo_mime)
                        {
                            LastModified = fecha_modificacion
                        };
                    }
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "Error retrieving blob.");
                    return new StatusCodeResult(500);
                }
            }
            catch (Exception e)
            {
                _logger.LogError(e, "General error.");
                return new BadRequestObjectResult(e.Message);
            }
        }
    }
}
