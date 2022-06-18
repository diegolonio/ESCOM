<?php 

    require 'sesion.php';
    
    if(!isset($_SESSION['usuario']))
    {
        header('location: index.php');
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $contrasenia_actual = $_POST['contrasenia_actual'];

        if (isset ($contrasenia_actual) && !empty ($contrasenia_actual))
        {
            $consulta = $conexion->prepare("SELECT IdUsuario FROM usuario WHERE IdUsuario = :IdUsuario AND Contrasena = :contrasenia LIMIT 1");
            $consulta->execute(
                array(
                    ':IdUsuario' => $_SESSION['usuario'],
                    ':contrasenia' => $contrasenia_actual
                )
            );
            $IdUsuario = $consulta->fetch();

            if (isset ($IdUsuario) && !empty ($IdUsuario) && $IdUsuario[0] == $_SESSION['usuario'])
            {
                $correo_nuevo = $_POST['correo_nuevo'];
                $correo_nuevo = trim ($correo_nuevo);
                $contrasenia_nueva = $_POST['contrasenia_nueva'];
                $contrasenia_nueva = trim ($contrasenia_nueva);
                $nuevo_num_tel = $_POST['nuevo_num_tel'];
                $nuevo_num_tel = trim ($nuevo_num_tel);

                if (isset ($correo_nuevo) && !empty ($correo_nuevo))
                {
                    $modificar_correo = $conexion->prepare("UPDATE usuario SET Correo = :correo_nuevo WHERE IdUsuario = :correo_actual");
			        $modificar_correo->execute(
                        array(
                            ':correo_nuevo' => $correo_nuevo,
                            ':correo_actual' => $_SESSION['usuario']
                        )
                    );
                    $modificar_correo = $modificar_correo->rowCount();
                }
                
                if (isset ($contrasenia_nueva) && !empty ($contrasenia_nueva))
                {
                    $modificar_contrasenia = $conexion->prepare("UPDATE usuario SET Contrasena = :contrasenia_nueva WHERE IdUsuario = :correo_actual");
			        $modificar_contrasenia->execute(
                        array(
                            ':contrasenia_nueva' => $contrasenia_nueva,
                            ':correo_actual' => $_SESSION['usuario']
                        )
                    );
                    $modificar_contrasenia = $modificar_contrasenia->rowCount();
                }
                
                if (isset ($nuevo_num_tel) && !empty ($nuevo_num_tel))
                {
                    $modificar_telefono = $conexion->prepare("UPDATE usuario SET Telefono = :telefono_nuevo WHERE IdUsuario = :correo_actual");
			        $modificar_telefono->execute(
                        array(
                            ':telefono_nuevo' => $nuevo_num_tel,
                            ':correo_actual' => $_SESSION['usuario']
                        )
                    );
                    $modificar_telefono = $modificar_telefono->rowCount();
                }

                echo "<script>alert('Modificación de datos exitosa.');window.location.href='modificar_datos_inicio_sesion.php';</script>";
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
    <title>Mercury | Modificar datos de inicio de sesión</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/modificar_datos_inicio_sesion.css">
    
    <link href="./css/validetta.css" rel="stylesheet" type="text/css" media="screen">
    <script src="./js/jquery-3.6.0.min.js"></script>
    <script type="text/javascript" src="http://code.jquery.com/jquery-latest.min.js"></script>
    <script type="text/javascript" src="./js/validetta.js"></script>
    <script type="text/javascript" src="./js/validettaLang-es-ES.js"></script>
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>

    <script>
        $(document).ready(function () {
            $("#registro").validetta({
                bubblePosition: 'bottom',
                bubbleGapTop: 10,
                bubbleGapLeft: -5,
                validators: {
                regExp: {
                regemail:
                {
                    pattern: /^(\W|^)[\w.\-]{0,25}@(hotmail|gmail)\.com(\W|$)$/,
                    errorMessage: 'Correo electrónico no válido'
                },
                regpass:
                {
                    pattern: /^(?=.*\d)(?=.*[\u0021-\u002b\u003c-\u0040])(?=.*[A-Z])(?=.*[a-z])\S{8,16}$/,
                    errorMessage: 'La contraseña debe tener al entre 8 y 16 caracteres, al menos un dígito, al menos una minúscula, al menos una mayúscula y al menos un caracter no alfanumérico'
                }
            }
        }
            });
        });
    </script>
    
</head>
<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="./index.php"><img src="./img/mercury_logo.png" alt="Mercury Logo" width="80%" height="80%"></a>
            </div>
            <div class="botones_cuenta">
                <div class="cuenta">
                    <a href="./gestionar_cuenta.php" class="boton_cuenta"><p><?php echo $datos['Nombre']." ".substr ($datos['Apellido_P'], 0, 1)."."; ?></p></a>
                    <div class="imagen_cuenta">
                        <img src="./img/user.png" alt="Imagen usuario">
                    </div>
                </div>
                <div class="cerrar_sesion">
                    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                        <button type="submit" name="cerrar_sesion" value="ok">Cerrar Sesión</button>
                    </form>
                </div>
            </div>
        </header>
        <div class="contenido">
            <div class="titulo">
                <h1>Cambiar datos de inicio de sesión</h1>
            </div>
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST" autocomplete="off">
                <div class="nuevos_datos">
                    <div class="contenedor_correo">
                        <label for="correo_nuevo">Correo nuevo</label>
                        <input type="text" name="correo_nuevo" id="correo_nuevo" data-validetta="regExp[regemail]">
                    </div>
                    <div class="contenedor_contrasenia">
                        <label for="contrasenia_nueva">Contraseña nueva</label>
                        <input type="password" name="contrasenia_nueva" id="contrasenia_nueva" data-validetta="minLength[8],maxLength[16],regExp[regpass]">
                    </div>
                    <div class="contenedor_tel">
                        <label for="nuevo_num_tel">Número de teléfono</label>
                        <input type="text" name="nuevo_num_tel" id="nuevo_num_tel" data-validetta="number,minLength[10],maxLength[10]">
                    </div>
                </div>
                <?php if (isset ($IdUsuario) && (empty ($IdUsuario) || $IdUsuario[0] != $_SESSION['usuario'])): ?>
                <div class="mensaje_error">
                    <p>Contraseña incorrecta.</p>
                </div>
                <?php endif; ?>
                <div class="confirmacion">
                    <div class="contrasenia_actual">
                        <label for="contrasenia_actual">Contraseña actual</label>
                        <input type="password" name="contrasenia_actual" id="contrasenia_actual" data-validetta="required,minLength[8],maxLength[16]">
                    </div>
                    <button type="submit">Aceptar</button>
                    <a href="gestionar_cuenta.php">Cancelar</a>
                </div>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>