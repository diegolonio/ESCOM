<?php 
    require 'sesion.php';
    
    if(!isset($_SESSION['usuario']))
    {
        header('location: index.php');
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $ciudad = $_POST['ciudad'];
        $ciudad = trim($ciudad);
        $cp = $_POST['cp'];
        $cp = trim($cp);
        $direccion = $_POST['direccion'];
        $direccion = trim($direccion);
        $referencias = $_POST['referencias'];
        $referencias = trim($referencias);
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
                if (isset ($ciudad) && !empty ($ciudad))
                {
                    $modificar_ciudad = $conexion->prepare("UPDATE direccion SET Ciudad = :ciudad WHERE IdUsuario = :id");
			        $modificar_ciudad->execute(
                        array(
                            ':ciudad' => $ciudad,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_ciudad = $modificar_ciudad->rowCount();
                }

                if (isset ($cp) && !empty ($cp))
                {
                    $modificar_cp = $conexion->prepare("UPDATE direccion SET CP = :cp WHERE IdUsuario = :id");
			        $modificar_cp->execute(
                        array(
                            ':cp' => $cp,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_cp = $modificar_cp->rowCount();
                }

                if (isset ($direccion) && !empty ($direccion))
                {
                    $modificar_direccion = $conexion->prepare("UPDATE direccion SET Direccion = :direccion WHERE IdUsuario = :id");
			        $modificar_direccion->execute(
                        array(
                            ':direccion' => $direccion,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_direccion = $modificar_direccion->rowCount();
                }

                if (isset ($referencias) && !empty ($referencias))
                {
                    $modificar_referencias = $conexion->prepare("UPDATE direccion SET Referencias = :referencias WHERE IdUsuario = :id");
			        $modificar_referencias->execute(
                        array(
                            ':referencias' => $referencias,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $modificar_referencias = $modificar_referencias->rowCount();
                }

                echo "<script>alert('Modificación de datos exitosa.');window.location.href='modificar_domicilio.php';</script>";
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
    <link rel="stylesheet" href="./css/modificar_domicilio.css">
    
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
                regdir: {
                    pattern: /^[a-zA-Z]{1,45}$/,
                    errorMessage: 'Solo se permiten letras del abecedario: Mayúsculas y minúsculas'
                },
                regCP:
                {
                    pattern: /^[0-1]{1,1}\d{4,4}$/,
                    errorMessage: 'El código postal no es de la CDMX'
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
                <h1>Cambiar datos de domicilio</h1>
            </div>
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <div class="datos_ciudad">
                    <div class="contenedor_ciudad">
                        <label for="ciudad">Ciudad:</label>
                        <select name="ciudad" id="ciudad" data-validetta="required">
                            <option value="">-- Elige tu ciudad --</option>
                            <option value="cdmx">CDMX</option>
                        </select>
                    </div>
                    <div class="contenedor_cp">
                        <label for="cp">Código postal:</label>
                        <input type="text" name="cp" id="cp" data-validetta="required,number,minLength[5],maxLength[5],regExp[regCP]">
                    </div>
                </div>
                <div class="contenedor_direccion">
                    <label for="direccion">Dirección:</label>
                    <input type="text" name="direccion" id="direccion" data-validetta="required,minLength[10],maxLength[100]">
                </div>
                <div class="contenedor_referencias">
                    <label for="referencias">Referencias:</label>
                    <input type="text" name="referencias" id="referencias" data-validetta="minLength[10],maxLength[100]">
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