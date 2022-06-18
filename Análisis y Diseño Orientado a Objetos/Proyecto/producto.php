<?php 
    require 'sesion.php';

    if ($_SERVER['REQUEST_METHOD'] == 'GET')
    {
        $id = $_GET['id'];
        $id = trim ($id);

        if (isset ($id) && !empty ($id))
        {
            $producto = $conexion->prepare("SELECT * FROM producto WHERE IdProducto = :id LIMIT 1");
            $producto->execute(
                array(
                    ':id' => $id
                )
            );
            $producto = $producto->fetch();

            $categoria = $conexion->prepare("SELECT Nombre FROM categoria WHERE IdCategoria = :id LIMIT 1");
            $categoria->execute(
                array(
                    ':id' => $producto['IdCategoria']
                )
            );
            $categoria = $categoria->fetch();
        }
        else
        {
            header ('Location: index.php');
        }
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        if (isset ($_SESSION['usuario']))
        {
            $num_piezas = $_POST['num_piezas'];
            $num_piezas = trim($num_piezas);
            $id = $_POST['anadir'];
            $id = trim ($id);
            
            if (isset ($num_piezas) && !empty ($num_piezas) && isset ($id) && !empty ($id))
            {
                $anadir_carrito = $conexion->prepare("INSERT INTO carrito (IdUsuario, IdProducto, Cantidad) VALUES(:id, :producto, :cantidad)");
                $anadir_carrito->execute(
                    array(
                        ':id' => $_SESSION['usuario'],
                        ':producto' => $id,
                        ':cantidad' => $num_piezas
                        )
                    );
                    
                echo "<script>alert('Producto agregado al carrito.');window.location.href='carrito.php';</script>";
            }
            else
            {
                echo "<script>alert('Selecciona el número de piezas.');window.location.href='producto.php?id=".$id."';</script>";
            }
        }
        else
        {
            echo "<script>alert('Se requiere iniciar sesión para realizar la operación.');window.location.href='ingresar.php';</script>";
        }
    }
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="./img/favicon.ico" type="image/x-icon">
    <link rel="icon" href="./img/favicon.ico" type="image/x-icon">
    <title>Mercury | <?php echo $producto['Nombre']; ?></title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/producto.css">
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>
</head>
<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="./index.php"><img src="./img/mercury_logo.png" alt="Mercury Logo" width="80%" height="80%"></a>
            </div>
            <nav class="categorias">
                <a href="#" class="boton_categorias">Categorías</a>
                <div class="enlaces">
                    <a href="electronicos.php">Electrónicos</a>
                    <a href="salud_belleza.php">Salud y belleza</a>
                    <a href="alimentos.php">Alimentos</a>
                    <a href="ropa_accesorios.php">Ropa y accesorios</a>
                    <a href="hogar.php">Hogar</a>
                    <a href="juguetes.php">Juguetes</a>
                    <a href="deportes.php">Deportes</a>
                    <a href="entretenimiento.php">Entretenimiento</a>
                </div>
            </nav>
            <div class="barra_navegacion">
                <form action="busqueda.php" method="GET" autocomplete="off">
                    <div class="icono_lupa">
                        <i class="fas fa-search"></i>
                    </div>
                    <input type="text" name="busqueda" autocomplete="off" placeholder="Busca los productos que necesitas">
                </form>
            </div>
            <?php if (isset ($_SESSION['usuario'])): ?>
            <div class="carrito">
                <a href="./carrito.php" class="boton_carrito"><i class="fas fa-shopping-cart"></i></a>
            </div>
            <?php else: ?>
            <div class="ingresar">
                <a href="./ingresar.php" class="boton_ingresar">Ingresar</a>
            </div>
            <?php endif; ?>
            <?php if (isset ($_SESSION['usuario'])): ?>
            <div class="cuenta">
                <a href="./gestionar_cuenta.php" class="boton_cuenta"><p><?php echo $datos['Nombre']." ".substr ($datos['Apellido_P'], 0, 1)."."; ?></p></a>
                <div class="imagen_cuenta">
                    <img src="./img/user.png" alt="Imagen usuario">
                </div>
            </div>
            <?php else: ?>
            <div class="registro">
                <a href="./registrarse.php" class="boton_registro">Registrarse</a>
            </div>
            <?php endif; ?>
            <?php if (isset ($_SESSION['usuario'])): ?>
            <div class="cerrar_sesion">
                <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                    <button type="submit" name="cerrar_sesion" value="ok">Cerrar Sesión</button>
                </form>
            </div>
            <?php endif; ?>
        </header>
        <div class="contenido">
            <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <div class="columna_izq">
                    <h1><?php echo $producto['Nombre']; ?></h1>
                    <p><?php echo $producto['Descripcion']; ?></p>
                    <!-- <button type="submit" name="comprar" value="<?php // echo $producto['IdProducto']; ?>" class="comprar">COMPRAR</button> -->
                    <button type="submit" name="anadir" value="<?php echo $producto['IdProducto']; ?>" class="aniadir">AÑADIR AL CARRITO<br>DE COMPRAS</button>
                </div>
                <div class="columna_cen">
                    <div class="contenedor_imagenes">
                        <div class="imagen_principal">
                            <img src="./img/<?php echo $producto['Imagen']; ?>" alt="<?php echo $producto['Nombre']; ?>">
                        </div>
                        <div class="imagenes_extra">
                            <div class="imagen_extra_1">
                                <img src="./img/producto.jpg" alt="Producto">
                            </div>
                            <div class="imagen_extra_2">
                                <img src="./img/producto.jpg" alt="Producto">
                            </div>
                            <div class="categoria">
                                <p><?php echo $categoria['Nombre']; ?></p>
                            </div>
                        </div>
                    </div>
                    <div class="contenedor_calificacion">
                        <p>Calificación:</p>
                        <div class="estrellas">
                            <?php for ($i = 0; $i < floor ($producto['Valoracion']); $i++): ?>
                                <i class="fas fa-star"></i>
                            <?php endfor; ?>
                            <?php if ($producto['Valoracion'] - (int)$producto['Valoracion'] == 0.5): ?>
                                <i class="fas fa-star-half-alt"></i>
                            <?php endif; ?>
                            <?php for ($i = 0; $i < 5 - ceil ($producto['Valoracion']); $i++): ?>
                                <i class="far fa-star"></i>
                            <?php endfor; ?>
                        </div>
                    </div>
                </div>
                <div class="columna_der">
                    <table>
                        <tr><td><p class="precio">$<?php echo $producto['Precio']; ?>.00</p></td></tr>
                        <tr><td><p>Uds. disponibles: <span><?php echo $producto['Existencias']; ?></span></p></td></tr>
                        <tr><td><p>Marca: <span><?php echo $producto['Marca']; ?></span></p></td></tr>
                        <tr><td><p>Estado: <span><?php echo $producto['Estado']; ?></span></p></td></tr>
                        <tr><td><p>Características: <span><?php echo $producto['Caracteristicas']; ?></span></p></td></tr>
                    </table>
                    <div class="contenedor_piezas">
                        <label for="num_piezas">Piezas: </label>
                        <input type="number" name="num_piezas" id="num_piezas" min="1" max="<?php echo $producto['Existencias']; ?>">
                    </div>
                    <!-- <div class="contenedor_opciones">
                        <select name="opcion" id="opciones">
                            <option value="default">Opciones</option>
                            <option value="negro">Negro</option>
                            <option value="blanco">Blanco</option>
                        </select>
                    </div> -->
                </div>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>