<?php
include_once './conexion.php';
include_once './sesion.php';

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

require '../PHPMailer/Exception.php';
require '../PHPMailer/PHPMailer.php';
require '../PHPMailer/SMTP.php';

$IdUsuario = $_SESSION['IdUsuario'];

$mail = new PHPMailer(true);

try {
    //Server settings
    //DATOS DEL CORREO.
    $mail->SMTPDebug = 0;                      //Enable verbose debug output
    $mail->isSMTP();                                            //Send using SMTP
    $mail->Host       = 'smtp.gmail.com';                     //Set the SMTP server to send through
    $mail->SMTPAuth   = true;                                   //Enable SMTP authentication
    $mail->Username   = 'cafeteriaescom1@gmail.com';                     //SMTP username
    $mail->Password   = '1234567a$';                               //SMTP password
    $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;         //Enable TLS encryption; `PHPMailer::ENCRYPTION_SMTPS` encouraged
    $mail->Port       = 587;
    $mail->CharSet = 'UTF-8';                                   //TCP port to connect to, use 465 for `PHPMailer::ENCRYPTION_SMTPS` above

    //CUENTA DE USUARIO DESDE LA QUE SE ENVIA EL CORREO
    $mail->setFrom('cafeteriaescom1@gmail.com', 'Mercury');
    //DESTINO DEL CORREO
    $mail->addAddress($Correo, $Usuario);     //Add a recipient
    // $mail->addAddress('ellen@example.com');               //Name is optional

    //Attachments
    /* $mail->addAttachment('/var/tmp/file.tar.gz');         //Add attachments
    $mail->addAttachment('/tmp/image.jpg', 'new.jpg');    //Optional name
*/

    $stmt = $pdo->prepare("SELECT * FROM Compra c JOIN Producto p ON c.IdProducto = p.IdProducto where IdVenta= :IdVenta");
    $stmt->bindParam(':IdVenta', $IdVenta);
    $stmt->execute();

    $consulta = $stmt->fetchAll();
    $Mensaje = '<table>
    <thead>
        <th>Producto</th>
        <th>Cantidad</th>
        <th>Precio</th>
    </thead>
    <tbody>';
    $Aux;
    foreach ($consulta as $Dato) :
        $Producto = $Dato["Nombre"];
        $Cantidad = $Dato["Cantidad"];
        $Precio = $Dato["Precio"];

        $Aux = '<tr>
        <td>' . $Producto . '</td>
        <td>' . $Cantidad . '</td>
        <td>' . $Precio . '</td>
        </tr>';

        $Mensaje = $Mensaje . $Aux;
    endforeach;
    $Mensaje =  $Mensaje . '</tbody>
    </table></br> <b>Total:' . $Total . ' </b> ';

    //MENSAJE
    $mail->isHTML(true);                                  //Set email format to HTML
    $mail->Subject = 'Mercury - Validar usuario';
    $mail->Body = 'Hola ' . $Usuario . '. Tu compra se ha realizado con éxito en Cafetería ESCOM. <br>En caso de que un producto de tu carrito no se haya reflejado en tu comprobante, significa que estaba agotado y no se te realizó ningún cargo .<br>Tus productos son: <br>' . $Mensaje.'<br>-- ESCOM --';

    //ACCION EN CASO DE QUE SE ENVIE CORRECTAMENTE EL CORREO
    $mail->send();
    //ACCION EN CASO DE QUE EL CORREO NO SE ENVIE CORRECTAMENTE
} catch (Exception $e) {
    $respAX_JSON["mensaje"] = "error";
}

        //SMPT OUTLOOK smtp-mail.outlook.com