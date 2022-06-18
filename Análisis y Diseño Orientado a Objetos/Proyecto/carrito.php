<?php
require 'sesion.php';

if (!isset($_SESSION['usuario'])) {
    header('location: index.php');
}

$consulta = $conexion->prepare("SELECT * FROM carrito WHERE IdUsuario = :id");
$consulta->execute(
    array(
        ':id' => $_SESSION['usuario']
    )
);
$consulta = $consulta->fetchAll();

$total = 0;
$aux = 0;

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST['comprar']) && !empty($_POST['comprar']) && $_POST['comprar'] == 'ok')
    {
        header ('Location: compra.php');
    }
    else if (isset($_POST['actualizar']) && !empty($_POST['actualizar']))
    {
        $piezas = $_POST['piezas'];
        $piezas = trim($piezas);
        $actualizar = $_POST['actualizar'];
        $actualizar = trim($actualizar);

        $actualizar_carrito = $conexion->prepare("UPDATE carrito SET Cantidad = :cantidad WHERE IdProducto = :producto AND IdUsuario = :id");
        $actualizar_carrito->execute(
            array(
                ':cantidad' => $piezas,
                ':producto' => $actualizar,
                ':id' => $_SESSION['usuario']
            )
        );
        $actualizar_carrito = $actualizar_carrito->rowCount();

        if ($actualizar_carrito > 0)
        {
            echo "<script>alert('Carrito actualizado.');window.location.href='carrito.php';</script>";
        }
    }
    else if (isset($_POST['producto_eliminado']) && !empty($_POST['producto_eliminado']))
    {
        $producto_eliminado = $_POST['producto_eliminado'];
        $producto_eliminado = trim($producto_eliminado);

        $eliminar_producto = $conexion->prepare("DELETE FROM carrito WHERE IdProducto = :producto AND IdUsuario = :id");
        $eliminar_producto->execute(
            array(
                ':producto' => $producto_eliminado,
                ':id' => $_SESSION['usuario']
            )
        );
        $eliminar_producto = $eliminar_producto->rowCount();

        if ($eliminar_producto > 0) {
            echo "<script>alert('Producto eliminado.');window.location.href='carrito.php';</script>";
        }
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
    <title>Mercury | Inicio</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/carrito.css">
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
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="carrito">
                    <a href="./carrito.php" class="boton_carrito"><i class="fas fa-shopping-cart"></i></a>
                </div>
            <?php else : ?>
                <div class="ingresar">
                    <a href="./ingresar.php" class="boton_ingresar">Ingresar</a>
                </div>
            <?php endif; ?>
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="cuenta">
                    <a href="./gestionar_cuenta.php" class="boton_cuenta">
                        <p><?php echo $datos['Nombre'] . " " . substr($datos['Apellido_P'], 0, 1) . "."; ?></p>
                    </a>
                    <div class="imagen_cuenta">
                        <img src="./img/user.png" alt="Imagen usuario">
                    </div>
                </div>
            <?php else : ?>
                <div class="registro">
                    <a href="./registrarse.php" class="boton_registro">Registrarse</a>
                </div>
            <?php endif; ?>
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="cerrar_sesion">
                    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                        <button type="submit" name="cerrar_sesion" value="ok">Cerrar Sesión</button>
                    </form>
                </div>
            <?php endif; ?>
        </header>
        <div class="contenido">
            <div class="titulo">
                <h1>CARRITO DE COMPRAS</h1>
            </div>
            <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <?php foreach ($consulta as $producto) :
                    $productos = $conexion->prepare("SELECT * FROM producto WHERE IdProducto = :id LIMIT 1");
                    $productos->execute(
                        array(
                            ':id' => $producto['IdProducto']
                        )
                    );
                    $productos = $productos->fetch();
                    $aux = $productos['Precio'] * $producto['Cantidad'];
                    $total += $aux;
                ?>
                    <div class="tarjeta_producto">
                        <a href="producto.php?id=<?php echo $productos['IdProducto']; ?>">
                            <div class="contenedor_datos">
                                <div class="imagen_producto">
                                    <img src="./img/<?php echo $productos['Imagen']; ?>" alt="<?php echo $productos['Nombre']; ?>">
                                </div>
                                <p><?php echo $productos['Nombre']; ?></p>
                            </div>
                        </a>
                        <div class="contenedor_piezas_precio">
                            <div class="input_piezas">
                                <label for="piezas">Piezas: </label>
                                <input type="number" name="piezas" id="piezas" min="1" max="<?php echo $productos['Existencias']; ?>" value="<?php echo $producto['Cantidad']; ?>">
                            </div>
                            <p class="precio">$<?php echo $productos['Precio']; ?>.00</p>
                        </div>
                        <button type="submit" name="actualizar" value="<?php echo $productos['IdProducto']; ?>"><i class="fas fa-sync-alt"></i></button>
                        <button type="submit" name="producto_eliminado" value="<?php echo $productos['IdProducto']; ?>"><i class="fas fa-trash-alt"></i></button>
                    </div>
                <?php endforeach; ?>
                <button type="submit" name="comprar" value="ok">COMPRAR</button>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>

</html>