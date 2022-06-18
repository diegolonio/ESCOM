<?php
require 'sesion.php';

$preposiciones = ['a', 'ante', 'bajo', 'cabe', 'con', 'contra', 'de', 'desde', 'durante', 'en', 'entre', 'hacia', 'hasta', 'mediante', 'para', 'por', 'segun', 'sin', 'so', 'sobre', 'tras', 'versus', 'via'];

if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    $statement = "SELECT * FROM producto WHERE ";
    $busqueda = $_GET['busqueda'];
    if (isset($busqueda) && !empty($busqueda)) {
        $palabras = explode(' ', $busqueda);
        $tamano = sizeof($palabras);

        foreach ($preposiciones as $preposicion) {
            if (!empty(array_search($preposicion, $palabras))) {
                unset($palabras[array_search($preposicion, $palabras)]);
            }
        }

        for ($i = 0; $i <= $tamano; $i++) {
            if (isset($palabras[$i])) {
                $statement .= "Descripcion LIKE '%" . $palabras[$i] . "%' OR Nombre LIKE '%" . $palabras[$i] . "%'";

                if ($i < $tamano - 1) {
                    $statement .= " OR ";
                }
            }
        }

        $productos = $conexion->prepare($statement);
        $productos->execute();
        $productos = $productos->fetchAll();
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
    <title>Mercury | Busqueda: <?php echo $busqueda; ?></title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/busqueda.css">
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
            <?php if (sizeof($productos) > 0) : ?>
                <div class="leyenda">
                    <p><span class="resultados_para">Resultados para:</span> <?php echo $busqueda; ?></p>
                </div>
                <?php foreach ($productos as $producto) : ?>
                    <a href="producto.php?id=<?php echo $producto['IdProducto']; ?>">
                        <div class="tarjeta_producto">
                            <div class="imagen_producto">
                                <img src="./img/<?php echo $producto['Imagen']; ?>" alt="<?php echo $producto['Nombre']; ?>">
                            </div>
                            <div class="datos_producto">
                                <h2><?php echo $producto['Nombre']; ?></h2>
                                <h3><?php echo $producto['Descripcion']; ?></h3>
                            </div>
                            <div class="precio_producto">
                                <p>$<?php echo $producto['Precio']; ?>.00</p>
                            </div>
                            <div class="calificacion_producto">
                                <?php for ($i = 0; $i < floor($producto['Valoracion']); $i++) : ?>
                                    <i class="fas fa-star"></i>
                                <?php endfor; ?>
                                <?php if ($producto['Valoracion'] - (int)$producto['Valoracion'] == 0.5) : ?>
                                    <i class="fas fa-star-half-alt"></i>
                                <?php endif; ?>
                                <?php for ($i = 0; $i < 5 - ceil($producto['Valoracion']); $i++) : ?>
                                    <i class="far fa-star"></i>
                                <?php endfor; ?>
                            </div>
                        </div>
                    </a>
                <?php endforeach; ?>
            <?php else : ?>
                <div class="leyenda">
                    <p><span class="resultados_para">No se encontraron resultados para:</span> <?php echo $busqueda; ?></p>
                </div>
            <?php endif; ?>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>

</html>