var URL = "/api";

var foto = null;  // por default la foto es nula

function get(id) {
    return document.getElementById(id);
}

function muestra(id) {
    get(id).style.display = "block";
}

function oculta(id) {
    get(id).style.display = "none";
}

function muestra_pantalla(id) {
    oculta("menu");
    muestra(id);
}

function oculta_pantalla(id) {
    oculta(id);
    muestra("menu");
}

function ocultar_toast(id) {
    get(id).style.display = "none";
    get('toast').style.display = "none";

    if (id == "eliminar_articulo") {
        get(id).innerHTML = "\
                    <div class='imagen'><img id='imagen'></div>\
                        <div class='datos'>\
                            <p id='nombre_toast'></p>\
                            <p id='descripcion_toast'></p>\
                            <p id='cantidad_toast'></p>\
                        </div>\
                        <div class='confirmacion'>\
                            <p>¿Está seguro de que quiere eliminar este producto de su carrito?</p>\
                            <div class='botones'>\
                            <button type='button' id='eliminar'>Sí</button>\
                            <button type='button' onclick='ocultar_toast(\"eliminar_articulo\")'>No</button>\
                        </div>\
                    </div>";
    }
}

function mostrar_toast(toast, id) {
    get('toast').style.display = "flex";
    get(toast).style.display = "flex";

    var cliente = new WSClient(URL);

    if (toast == "eliminar_articulo") {
        get('eliminar').setAttribute("onclick", "eliminar_articulo(" + id + ")");

        cliente.postJson("articulo_toast", {
            ID: id
        },
            function (code, result) {
                if (code == 200) {
                    let articulo = JSON.parse(JSON.stringify(result));
                    crear_articulo_toast(articulo);
                } else {
                    alert(JSON.stringify(result));
                }
            }
        );
    }
}

function readSingleFile(files, imagen) {
    var file = files[0];
    if (!file) return;
    var reader = new FileReader();
    reader.onload = function (e) {
        imagen.src = reader.result;
        // reader.result incluye al principio: "data:image/jpeg;base64,"
        foto = reader.result.split(',')[1];
    };
    reader.readAsDataURL(file);
}

function inicializar() {
    get('resultados').innerHTML = "\
                <div class='aviso aviso_compra'>\
                    <div class='imagen_aviso'><img src='./Servidor?nombre=carrito.png'></div>\
                    <p class='caveat-aviso'>¡Utiliza la barra de búsqueda para encontrar tus productos favoritos!</p>\
                </div>";
    get('articulos').innerHTML = "\
                <div class='aviso aviso_carrito'>\
                    <div class='imagen_aviso'><img src='./Servidor?nombre=carrito.png'></div>\
                    <p class='caveat-aviso'>¡Aún no tienes productos en tu carrito!</p>\
                </div>";
    get('busqueda').value = "";
    get('ordenar').selectedIndex = 0;
    get('total').textContent = "";
}

function limpiar_captura() {
    get('nombre').value = "";
    get('descripcion').value = "";
    get('precio').value = "";
    get('existencia').value = "";
    get('relevancia').value = "";
    get('alta_imagen').src = "./Servidor?nombre=paquete.jpg";
    get('imagen_file').value = "";
}

function insertar_articulo() {
    var cliente = new WSClient(URL);

    let articulo = {
        nombre: get('nombre').value,
        descripcion: get('descripcion').value,
        precio: get('precio').value,
        existencia: get('existencia').value,
        relevancia: get('relevancia').value,
        foto: foto
    };

    cliente.postJson("insertar_articulo", {
        articulo: articulo
    },
        function (code, result) {
            if (code == 200) {
                alert("OK");
                limpiar_captura();
            } else {
                alert(JSON.stringify(result));
            }
        }
    );
}

function busqueda() {
    let cliente = new WSClient(URL);
    let consulta = {
        busqueda: get('busqueda').value,
        orden: get('ordenar').value
    };

    cliente.postJson("busqueda_articulo",
        {
            busqueda: consulta
        },
        function (code, result) {
            if (code == 200) {
                let articulos = JSON.parse(JSON.stringify(result));

                get('resultados').innerHTML = "";

                for (let i = 0; i < articulos.length; i++)
                    crear_articulo(articulos[i]);
            } else {
                alert("¡Lo sentimos! El producto que estás intentando buscar no existe.");
            }
        }
    );
}

function agregar_carrito(id) {
    let cliente = new WSClient(URL);
    let carrito = {
        ID: id,
        cantidad: get(id).value
    };

    cliente.postJson("agregar_carrito",
        {
            carrito: carrito
        },
        function (code, result) {
            if (code == 200) {
                alert("Producto agregado al carrito.");
                busqueda();
            } else {
                alert(JSON.parse(JSON.stringify(result)).error);
            }
        }
    );
}

function obtener_carrito() {
    let cliente = new WSClient(URL);

    cliente.postJson("obtener_carrito", null,
        function (code, result) {
            if (code == 200) {
                let articulos = JSON.parse(JSON.stringify(result));
                let total = 0;

                get('articulos').innerHTML = "";

                for (let i = 0; i < articulos.length; i++) {
                    crear_articulo_carrito(articulos[i]);
                    total += (articulos[i].precio * articulos[i].cantidad);
                }

                get('total').textContent = "Total: $" + total.toFixed(2).replace(/\B(?=(\d{3})+(?!\d))/g, ",");
            } else {
                inicializar();
            }
        }
    );
}

function eliminar_articulo(id) {
    let cliente = new WSClient(URL);

    cliente.postJson("eliminar_articulo", {
        ID: id
    },
        function (code, result) {
            ocultar_toast('eliminar_articulo');

            if (code == 200) {
                carrito = obtener_carrito();
                alert("El artículo ha sido eliminado de tu carrito con éxito.");
            } else {
                alert(JSON.stringify(result));
            }
        }
    );
}

function eliminar_carrito() {
    let cliente = new WSClient(URL);

    cliente.postJson("eliminar_carrito", null,
        function (code, result) {

            if (code == 200) {
                alert("Carrito eliminado con éxito.");
            } else {
                alert(JSON.stringify(result));
            }

            inicializar();
            ocultar_toast('eliminar_carrito');
        }
    );
}

function crear_articulo(articulo) {
    let resultados = get('resultados'),
        resultado = null,
        izquierda = null,
        imagen = null,
        centro = null,
        nombre_producto = null,
        descripcion_producto = null,
        derecha = null,
        precio_producto = null,
        existencia_producto = null,
        div = null,
        cantidad = null,
        input = null,
        button = null;

    resultado = document.createElement('div');
    resultado.setAttribute("class", "resultado");

    izquierda = document.createElement('div');
    izquierda.setAttribute("class", "izquierda");

    imagen = document.createElement("img");
    imagen.setAttribute("src", "data:image/jpeg;base64," + articulo.foto);

    izquierda.appendChild(imagen);
    resultado.appendChild(izquierda);

    centro = document.createElement('div');
    centro.setAttribute("class", "centro");

    nombre_producto = document.createElement("p");
    nombre_producto.setAttribute("class", "nombre_producto");
    nombre_producto.textContent = articulo.nombre;

    descripcion_producto = document.createElement("p");
    descripcion_producto.setAttribute("class", "descripcion_producto");
    descripcion_producto.textContent = articulo.descripcion;

    centro.appendChild(nombre_producto);
    centro.appendChild(descripcion_producto);
    resultado.appendChild(centro);

    derecha = document.createElement("div");
    derecha.setAttribute("class", "derecha");

    precio_producto = document.createElement("p");
    precio_producto.setAttribute("class", "precio_producto");
    precio_producto.textContent = "$" + articulo.precio.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");

    existencia_producto = document.createElement("p");
    existencia_producto.setAttribute("class", "existencia_producto");
    existencia_producto.textContent = "Existencia: " + articulo.existencia;

    div = document.createElement("div");

    cantidad = document.createElement("p");
    cantidad.textContent = "Cantidad: ";

    input = document.createElement("input");
    input.setAttribute("type", "number");
    input.setAttribute("name", "cantidad");
    input.setAttribute("id", articulo.ID);
    input.setAttribute("value", 1);
    input.setAttribute("min", 1);
    input.setAttribute("step", 1);

    button = document.createElement("button");
    button.setAttribute("type", "button");
    button.setAttribute("onclick", "agregar_carrito(" + articulo.ID + ")");
    button.textContent = "Agregar al Carrito";

    div.appendChild(cantidad);
    div.appendChild(input);

    derecha.appendChild(precio_producto);
    derecha.appendChild(existencia_producto);
    derecha.appendChild(div);
    derecha.appendChild(button);
    resultado.appendChild(derecha);

    resultados.appendChild(resultado);
}

function crear_articulo_carrito(articulo) {
    let articulos = get('articulos'),
        resultado = null,
        izquierda = null,
        imagen = null,
        centro = null,
        nombre_producto = null,
        descripcion_producto = null,
        derecha = null,
        precio_producto = null,
        cantidad_producto = null,
        costo_producto = null,
        input = null,
        button = null;

    resultado = document.createElement('div');
    resultado.setAttribute("class", "resultado");

    izquierda = document.createElement('div');
    izquierda.setAttribute("class", "izquierda");

    imagen = document.createElement("img");
    imagen.setAttribute("src", "data:image/jpeg;base64," + articulo.foto);

    izquierda.appendChild(imagen);
    resultado.appendChild(izquierda);

    centro = document.createElement('div');
    centro.setAttribute("class", "centro");

    nombre_producto = document.createElement("p");
    nombre_producto.setAttribute("class", "nombre_producto");
    nombre_producto.textContent = articulo.nombre;

    descripcion_producto = document.createElement("p");
    descripcion_producto.setAttribute("class", "descripcion_producto");
    descripcion_producto.textContent = articulo.descripcion;

    centro.appendChild(nombre_producto);
    centro.appendChild(descripcion_producto);
    resultado.appendChild(centro);

    derecha = document.createElement("div");
    derecha.setAttribute("class", "derecha");

    precio_producto = document.createElement("p");
    precio_producto.setAttribute("class", "precio_producto");
    precio_producto.textContent = "Precio: $" + articulo.precio.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");

    cantidad_producto = document.createElement("p");
    cantidad_producto.setAttribute("class", "cantidad_producto");
    cantidad_producto.textContent = "Cantidad: " + articulo.cantidad;

    costo_producto = document.createElement("p");
    costo_producto.setAttribute("class", "costo_producto");
    costo_producto.textContent = "Costo: $" + (articulo.precio * articulo.cantidad).toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");

    button = document.createElement("button");
    button.setAttribute("type", "button");
    button.setAttribute("onclick", "mostrar_toast('eliminar_articulo', " + articulo.ID + ")");
    button.textContent = "Eliminar del Carrito";

    derecha.appendChild(precio_producto);
    derecha.appendChild(cantidad_producto);
    derecha.appendChild(costo_producto);
    derecha.appendChild(button);
    resultado.appendChild(derecha);

    articulos.appendChild(resultado);
}

function crear_articulo_toast(articulo) {
    get('imagen').src = "data:image/jpeg;base64," + articulo.foto;
    get('nombre_toast').textContent = articulo.nombre;
    get('descripcion_toast').textContent = articulo.descripcion;
    get('cantidad_toast').textContent = "Cantidad en Carrito: " + articulo.cantidad;
}
