<?php 
    require 'sesion.php';
    
    if(!isset($_SESSION['usuario']))
    {
        header('location: index.php');
    }
    
    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $tipo = $_POST['tipo_tarjeta'];
        $tipo = trim($tipo);
        $tarjeta = $_POST['numero_tarjeta'];
        $tarjeta = trim($tarjeta);
        $cvv = $_POST['cvv'];
        $cvv = trim($cvv);
        $contrasena = $_POST['contrasenia_actual'];
        $contrasena = trim($contrasena);

        if (isset ($contrasena) && !empty ($contrasena))
        {
            $consulta = $conexion->prepare("SELECT IdUsuario FROM usuario WHERE IdUsuario = :IdUsuario AND Contrasena = :contrasenia LIMIT 1");
            $consulta->execute(
                array(
                    ':IdUsuario' => $_SESSION['usuario'],
                    ':contrasenia' => $contrasena
                )
            );
            $IdUsuario = $consulta->fetch();

            if (isset ($IdUsuario) && !empty ($IdUsuario) && $IdUsuario[0] == $_SESSION['usuario'])
            {
                if (isset ($tipo) && !empty ($tipo))
                {
                    $modificar_tipo = $conexion->prepare("UPDATE banco SET Tipo = :tipo WHERE IdUsuario = :id");
			        $modificar_tipo->execute(
                        array(
                            ':tipo' => $tipo,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_tipo = $modificar_tipo->rowCount();
                }

                if (isset ($tarjeta) && !empty ($tarjeta))
                {
                    $modificar_tarjeta = $conexion->prepare("UPDATE banco SET NoTarjeta = :tarjeta WHERE IdUsuario = :id");
			        $modificar_tarjeta->execute(
                        array(
                            ':tarjeta' => $tarjeta,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_tarjeta = $modificar_tarjeta->rowCount();
                }

                if (isset ($cvv) && !empty ($cvv))
                {
                    $modificar_cvv = $conexion->prepare("UPDATE banco SET CVV = :cvv WHERE IdUsuario = :id");
			        $modificar_cvv->execute(
                        array(
                            ':cvv' => $cvv,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_cvv = $modificar_cvv->rowCount();
                }

                echo "<script>alert('Modificación de datos exitosa.');window.location.href='modificar_datos_bancarios.php';</script>";
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
    <link rel="stylesheet" href="./css/modificar_datos_bancarios.css">
    
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
                <h1>Cambiar datos bancarios</h1>
            </div>
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <div class="contenedor_tipo_tarjeta">
                    <label for="tipo_tarjeta">Tipo de tarjeta:</label>
                    <select name="tipo_tarjeta" id="tipo_tarjeta" data-validetta="required">
                        <option value="">--Tipo de tarjeta--</option>
                        <option value="1" >Débito</option>
                        <option value="0">Crédito</option>
                    </select>
                </div>
                <div class="contenedor_numero_tarjeta">
                    <label for="numero_tarjeta">Número de tarjeta:</label>
                    <input type="text" name="numero_tarjeta" id="numero_tarjeta" data-validetta="required,creditCard,number">
                </div>
                <div class="contenedor_cvv">
                    <label for="cvv">CVV:</label>
                    <input type="text" name="cvv" id="cvv" data-validetta="required,number,minLenght[3],maxLenght[3]">
                </div>
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