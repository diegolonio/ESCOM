# HIGH-ORDER CALCULATOR
La High-Order Calculator (HOC) es la implementación de un lenguaje de programación interpretado utilizando ```bison``` y el lenguaje C, descrita en seis etapas en el libro *The Unix Programming Environment* (Kernighan y Pike, 1984) para ejemplificar el proceso de desarrollo de programas grandes.

Las seis etapas son:

- **HOC 1:**
    Una calculadora capaz de realizar las cuatro operaciones aritméticas elementales (suma, resta, multiplicación y división) sobre números en coma flotante; permite utilizar paréntesis para modificar la precedencia de los operadores. Una expresión es ingresada y el resultado es impreso inmediatamente.

- **HOC 2:**
    Variables cuyo nombre puede ser solo una letra de la *a* a la *z*. Se incluye el operador menos (-) unario y algunas defensas contra errores.

- **HOC 3:**
    Nombres de variables con cualquier cantidad de caracteres. Funciones predefinidas para el seno, coseno, la exponencial, etc., constantes útiles como $\pi$, y el operador de exponenciación.

- **HOC 4:**
    Cambios en su funcionamiento interno: para cada sentencia se genera código que posteriormente es interpretado, en lugar de ser evaluado en tiempo real.

- **HOC 5:**
    Control de flujo: ```if-else``` y ```while```, agrupamiento de sentencias con ```{``` y ```}```, y operadores relacionales como ```<```, ```>=```, etc.

- **HOC 6:**
    Funciones y procedimientos recursivos, con argumentos. Se agregan sentencias para entrada y salida de cadenas y números. 