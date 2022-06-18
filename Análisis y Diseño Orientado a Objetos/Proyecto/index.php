<?php 
    require 'sesion.php';

    $productos = $conexion->prepare("SELECT * FROM producto");
    $productos->execute();
    $productos = $productos->fetchAll();

    
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
    <link rel="stylesheet" href="./css/inicio.css">
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
            <section class="mercury">
                <div class="logo_izquierda">
                    <img src="./img/mercury_logo.png" alt="Mercury logo">
                </div>
                <h1>MERCURY</h1>
                <div class="logo_derecha">
                    <img src="./img/mercury_logo.png" alt="Mercury logo">
                </div>
            </section> 
            <section class="leyenda_destacado">
                <div class="llama">
                    <img src="./img/llama_ardiente.png" alt="Llama ardiente">
                </div>
                <p>LO MÁS<br>DESTACADO</p>
            </section>
            <section class="leyenda_descubre">
                <p>Descubre nuestros productos con las<br>mejores calificaciones</p>
                <div class="flecha_abajo">
                    <i class="far fa-arrow-alt-circle-down"></i>
                </div>
            </section>
            <div class="tarjetas_productos">
                <section class="tarjeta_producto">
                    <h2>Xiaomi Redmi 9</h2>
                    <div class="imagen_producto">
                        <img src="./img/Redmi9.png" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Xiaomi Redmi 9 (Global) Dual SIM 32 GB carbon grey 3 GB RAM</h3>
                        <p>$2,668.40</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                <section class="tarjeta_producto">
                    <h2>Nintendo Switch Lite</h2>
                    <div class="imagen_producto">
                        <img src="./img/Nintendo_Switch.jpeg" alt="Nintendo Switch">
                    </div>
                    <div class="datos_producto">
                        <h3>Nintendo Switch Lite 32GB color turquesa</h3>
                        <p>$5,319.00</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star-half-alt"></i>
                        </div>
                    </div>
                </section>
                <section class="tarjeta_producto">
                    <h2>Cubrebocas</h2>
                    <div class="imagen_producto">
                        <img src="./img/Cubreboca.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Cubrebocas Plisado Tricapa Termosellado Fda Y Ce</h3>
                        <p>$191.00</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star-half-alt"></i>
                            <i class="far fa-star"></i>
                        </div>
                    </div>
                </section>
                <section class="tarjeta_producto">
                    <h2>Amazon Echo Dot</h2>
                    <div class="imagen_producto">
                        <img src="./img/Amazon_Doth.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Amazon Echo Dot 4th Gen con asistente virtual Alexa charcoal 110V/240V</h3>
                        <p>$1,010.00</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                <section class="tarjeta_producto">
                    <h2>Silla de escritorio</h2>
                    <div class="imagen_producto">
                        <img src="./img/Silla_ escritorio.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Econosillas Ecochair ergonómica negra con tapizado de mesh y tela microespacial</h3>
                        <p>$1,144.50</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                </section>
                <section class="tarjeta_producto">
                    <h2>Set 4 Bolsos</h2>
                    <div class="imagen_producto">
                        <img src="./img/Set_bolsos.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Modernos Mujer Dama Hombro De Alta Calidad</h3>
                        <p>$269.54</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star-half-alt"></i>
                        </div>
                    </div>
                </section>
                </section>
                <section class="tarjeta_producto">
                    <h2>Bicicleta fija</h2>
                    <div class="imagen_producto">
                        <img src="./img/Bicicleta.jpeg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Spinning Centurfit MKZ-JINYUAN10KG</h3>
                        <p>$3,699.00</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                </section>
                <section class="tarjeta_producto">
                    <h2>Cortadora de cabello.</h2>
                    <div class="imagen_producto">
                        <img src="./img/Cortadora.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Wahl 5 Star Magic Clip Cordless 8148 roja 120V</h3>
                        <p>$2,440.00</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                </section>
                <section class="tarjeta_producto">
                    <h2>Ramen Coreano</h2>
                    <div class="imagen_producto">
                        <img src="./img/Ramen_Coreano.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Picante Carbo 1 Pieza 130g Samyang</h3>
                        <p>$40.50</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                        </div>
                    </div>
                </section>
                </section>
                <section class="tarjeta_producto">
                    <h2>Luz Nocturna</h2>
                    <div class="imagen_producto">
                        <img src="./img/dinosaurio.jpg" alt="Imagen producto">
                    </div>
                    <div class="datos_producto">
                        <h3>Led Rgb Dinosaurio 3d Un Solo Toque.</h3>
                        <p>$200.12</p>
                        <div class="calif">
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star"></i>
                            <i class="fas fa-star-half-alt">
                        </i>
                        </div>
                    </div>
                </section>
            </div>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>
