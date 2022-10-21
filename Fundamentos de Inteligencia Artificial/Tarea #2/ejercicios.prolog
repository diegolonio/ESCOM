% =====================================================================
%   Diego Armando Apolonio Villegas
%   
%   TAREA #2: Paquete de ejercicios Prolog
%   
%   - Resuelva los 15 ejercicios de este paquete.
%
%   - Entregue el código fuente de la solución en un solo archivo con
%   extensión .prolog.
%
%   Los predicados solicitados en el paquete de ejercicios son:
%   
%   1. contiene_numero(<lista>).
%   2. inserta_ceros(<lista>, <resultado>).
%   3. rota(<lista>, <n>, <lista rotada n veces>).
%   4. reversa_simple(<lista>, <lista invertida>).
%   5. inserta0_en(<elemento>, <lista>, <posición>, <resultado>).
%   6. promedio_parcial(<lista>, <n>, <promedio>).
%   7. fibonacci(<n>, <n-ésimo término de la serie de Fibonacci>).
%   8. simplifica(<lista>, <lista simplificada>).
%   9. depura(<lista>, <lista depurada>).
%   10. maximo(<lista>, <máximo valor numérico en lista>).
%   11. anti_consonante(<lista>, <resultado>).
%   12. vocales(<lista>, <resultado>).
%   13. cada_dos(<lista>, <resultado>).
%   14. contexto(<lista>, <elemento>, <contexto de elemento>).
%   15. particiona(<lista>, <posición>, <partición izquierda>, <partición derecha>).
%
%   Para probar la solución a cada problema del paquete de ejercicios,
%   utilice el predicado correspondiente de la lista anterior.
% =====================================================================

% =====================================================================
%   1. [ ] contiene_numero/1. Verificar si una lista contiene algún
%   elemento numérico.
%
%                       contiene_numero(<lista>).
%
%   Verdadero si <lista> es una lista que contiene algún elemento
%   numérico, falso en cualquier otro caso. Ejemplos:
%
%                   contiene_numero([a,b,c,d]). -> false.
%                   contiene_numero(23). -> false.
%                   contiene_numero([a,b,3,d,e]). -> true.
% =====================================================================

% =====================================================================
%   contiene_numero/1.
%   contiene_numero(+Lista).
%   Verifica que Lista contiene al menos un elemento numérico.
% =====================================================================
contiene_numero([Elemento|_]) :-
    number(Elemento), !.
contiene_numero([Elemento|Resto]) :-
    \+ number(Elemento),
    contiene_numero(Resto).

% =====================================================================
%   2. [ ] inserta_ceros/2. Intercalar ceros después de cada elemento
%   original.
%
%                   inserta_ceros(<lista>, <respuesta>).
%
%   Verdadero si <respuesta> es una lista con los mismos elementos que
%   <lista>, pero con un cero agregado después de cada elemento
%   original. La lista vacía debe conservarse. Ejemplos:
%
%       inserta_ceros([a,2,c,3], R). R = [a, 0, 2, 0, c, 0, 3, 0].
%       inserta-ceros([], R). R = [].
% =====================================================================

% =====================================================================
%   inserta_ceros/2.
%   inserta_ceros(+Lista, -ListaConCeros).
%   Inserta ceros después de cada elemento de Lista, el resultado es
%   devuelto en ListaConCeros. Si Lista es la lista vacía,
%   también ListaConCeros lo es. 
% =====================================================================
inserta_ceros([], []).
inserta_ceros([Elemento|FuenteResto], [Elemento, 0|DestinoResto]) :-
    inserta_ceros(FuenteResto, DestinoResto).

% =====================================================================
%   3. [sin usar append] rota/3. Rotar los elementos de una lista algún
%   número de posiciones hacia la derecha.
%
%                   rota(<lista>, <n>, <respuesta>).
%
%   Verdadero si <respuesta> es una lista con los mismos elementos que
%   <lista>, pero rotados hacia la derecha <n> posiciones. Ejemplos:
%
%                   rota([a,b,c,d], 1, R). R = [d,a,b,c].
%                   rota([a,b,c,d], 2, R). R = [c,d,a,b].
%                   rota([a,b,c,d], 3, R). R = [b,c,d,a].
%                   rota([a,b,c,d], 4, R). R = [a,b,c,d].
% =====================================================================

% =====================================================================
%   rota/3.
%   rota(+Lista, +N, -ListaRotada).
%   Rota Lista N veces hacia la derecha. La lista resultante es
%   devuelta en ListaRotada.
% =====================================================================
rota(ListaRotada, 0, ListaRotada) :- !.
rota(Lista, N, ListaRotada) :-
    N > 0,
    reverse(Lista, [Ultimo|Tmp]),
    reverse(Tmp, Resto),
    M is N - 1,
    rota([Ultimo|Resto], M, ListaRotada), !.

% =====================================================================
%   4. [sin usar reverse, ni append] reversa_simple/2. Invertir una
%   lista.
%
%               reversa_simple(<lista>, <respuesta>).
%
%   Verdadero si <respuesta> es la inversión de primer nivel de
%   <lista>. Ejemplos:
%
%       reversa_simple([a,b,c,d], R). R = [d,c,b,a].
%       reversa_simple([[1,2],3,[a,b],c], R). R = [c,[a,b],3,[1,2]].   
% =====================================================================

% =====================================================================
%   intercambiar/2.
%   intercambiar(+Lista, -Resultado).
%   Resultado es una lista anidada con los elementos de Lista en orden
%   inverso. Ejemplo:
%
%                       [a, b, c] -> [[[c], b], a]
% =====================================================================
intercambiar([Elemento], [Elemento]).
intercambiar([Elemento|Tmp], [Resto, Elemento]) :-
    intercambiar(Tmp, Resto).

% =====================================================================
%   aplanar/3.
%   aplanar(+Lista, +Cabeza, -ListaAplanada).
%   Lista es una lista de la forma [[[...], Elemento], Elemento].
%   Coloca todos los elementos de Lista en una lista ListaAplanada que
%   no está anidada. Cabeza es el último elemento de ListaAplanada.
% =====================================================================
aplanar([Elemento], Cabeza, [Elemento|Cabeza]).
aplanar([Sublista, Elemento], Cabeza, ListaAplanada) :-
    aplanar(Sublista, [Elemento|Cabeza], ListaAplanada).

% =====================================================================
%   reversa_simple/2.
%   reversa_simple(+Lista, -ListaInvertida).
%   Invierte el orden de los elementos de Lista. La lista resultante es
%   devuelta en ListaInvertida.
% =====================================================================
reversa_simple(Lista, ListaInvertida) :-
    intercambiar(Lista, Tmp),
    aplanar(Tmp, [], ListaInvertida), !.

% =====================================================================
%   5. [sin usar select] inserta0_en/4. Insertar un término arbitrario
%   en alguna posición específica de una lista.
%
%       inserta0_en(<término>, <lista>, <posición>, <resultado>).
%
%   Verdadero si <resultado> es una lista con los mismos elementos que
%   <lista> pero con <término> insertado en la posición <posición>,
%   considerando el inicio de la lista como la posición 0. Ejemplos:
%
%           inserta0_en(x, [a,b,c,d], 2, R). R = [a,b,x,c,d].
%           inserta0_en(3, [a,b,c,d], 1, R). R = [a,3,b,c,d].
%           inserta0_en(, [], 4, R). false.
% =====================================================================

% =====================================================================
%   inserta0_en/4.
%   inserta0_en(+Lista, +Elemento, +Posición, -Resultado).
%   Inserta Elemento en la posición Posición de Lista. La lista
%   resultante es devuelta en Resultado.
% =====================================================================
inserta0_en(Elemento, Lista, Posicion, Resultado) :-
    length(Lista, LongitudLista),
    between(0, LongitudLista, Posicion),
    nth0(Posicion, Resultado, Elemento, Lista), !.

% =====================================================================
%   6. [ ] promedio_parcial/3. Calcular el promedio (media aritmética)
%   de los primeros n elementos de una lista.
%
%               promedio_parcial(<lista>, <n>, <resultado>).
%
%   Verdadero si <resultado> es un número que representa el promedio de
%   los primeros <n> elementos numéricos de <lista> (que puede tener
%   otros elementos no-numéricos). Ejemplos:
%
%               promedio_parcial([2,4,6,8,10], 3, R). R = 4.
%               promedio_parcial([a,4,b,8,4,c,d], 2, R). R = 6.
%               promedio_parcial([1,3,5,7], 4, R). R = 4.
% =====================================================================

% =====================================================================
%   sumatoria/3.
%   sumatoria(+Lista, +N, -Sumatoria).
%   Suma los primeros N elementos numéricos de Lista. El resultado de
%   la suma es devuelto en Sumatoria.
% =====================================================================
sumatoria(_, 0, 0) :- !.
sumatoria([Elemento|Resto], N, SumaTotal) :-
    N > 0,
    number(Elemento),
    M is N - 1,
    sumatoria(Resto, M, SumaParcial),
    SumaTotal is Elemento + SumaParcial, !.
sumatoria([Elemento|Resto], N, SumaTotal) :-
    N > 0,
    \+ number(Elemento),
    sumatoria(Resto, N, SumaTotal).

% =====================================================================
%   promedio_parcial/3.
%   promedio_parcial(+Lista, +N, -Promedio).
%   Calcula el promedio de los primeros N elementos numéricos de Lista.
%   El resultado es devuelto en Promedio.
% =====================================================================
promedio_parcial(Lista, N, Promedio) :-
    N > 0,
    sumatoria(Lista, N, Suma),
    Promedio is Suma / N.

% =====================================================================
%   7. [sin cortes] fibonacci/2. Calcular cada término en la serie de
%   Fibonacci.
%
%                       fibonacci(<n>, <resultado>).
%
%   Verdadero si <resultado> es el número Fibonacci correspondiente a
%   <n>. Ejemplos: Los primeros elementos de la serie son:
%
%       fib(0)= 1. fib(1)= 1. fib(2)= 1. fib(3)= 2. fib(4)= 3.
%       fib(5)= 5. fib(6)= 8. fib(7)= 13. fib(8)= 21.
%       fib(9)= 34. fib(10)= 55.
% =====================================================================

% =====================================================================
%   fibonacci/2.
%   fibonacci(+N, -Término).
%   Calcula el N-ésimo término de la serie de Fibonacci. El término es
%   devuelto en Término.
% =====================================================================
fibonacci(0, 0).
fibonacci(1, 1).
fibonacci(N, Resultado) :-
    N >= 2,
    A is N - 1,
    B is N - 2,
    fibonacci(A, Tmp1),
    fibonacci(B, Tmp2),
    Resultado is Tmp1 + Tmp2.

% =====================================================================
%   8. [sin usar sort ni list_to_set, ni list_to_ord_set] simplifica/2.
%   Eliminar de una lista todos los elementos que se encuentren
%   duplicados.
%
%                   simplifica(<lista>, <resultado>).
%
%   Verdadero si <resultado> es una lista con los mismos elementos que
%   <lista> pero con sólo una instancia de cada elemento. Ejemplos:
%
%           simplifica([a,b,1,2,c,a,2,1,4], R). R = [a,b,c,4].
%           simplifica([1,1,2,2,3,3], R). R = [].
% =====================================================================

% =====================================================================
%   simplifica/3.
%   simplifica(+Referencia, +Lista, -ListaSimplificada).
%   Genera la lista ListaSimplificada con los elementos de Lista que no
%   se encuentran duplicados en Referencia.
% =====================================================================
simplifica(_, [], []) :- !.
simplifica(Lista, [Elemento|RestoFuente], [Elemento|RestoDestino]) :-
    \+ occurrences_of_term(Elemento, Lista, 2),
    simplifica(Lista, RestoFuente, RestoDestino), !.
simplifica(Lista, [_|RestoFuente], RestoDestino) :-
    simplifica(Lista, RestoFuente, RestoDestino).

% =====================================================================
%   simplifica/2.
%   simplifica(+Lista, -ListaSimplificada).
%   Genera una lista ListaSimplificada con los elementos de Lista que
%   no están duplicados.
% =====================================================================
simplifica(Lista, ListaSimplificada) :-
    simplifica(Lista, Lista, ListaSimplificada).

% =====================================================================
%   9. [ ] depura/2. Eliminar de una lista todos los elementos que NO
%   se encuentren duplicados, cuando menos, una vez.
%
%                   depura(<lista>, <resultado>).
%
%   Verdadero si <resultado> es una lista conteniendo sólo una
%   instancia de cada elemento en <lista> que sí tenía repeticiones.
%   Ejemplos:
%
%           depura([a,b,1,2,c,a,2,1,4], R). R = [a,1,2].
%           depura([1,1,2,2,3,3], R). R = [1,2,3].
%           depura([1,2,3,4], R). R = [].
% =====================================================================

% =====================================================================
%   depura/3.
%   depura(+Referencia, +Lista, -ListaDepurada).
%   Genera una lista ListaDepurada con los elementos de Lista que no se
%   encuentran duplicados en Referencia.
% =====================================================================
depura(_, [], []) :- !.
depura(Lista, [Elemento|RestoFuente], [Elemento|RestoDestino]) :-
    occurrences_of_term(Elemento, Lista, 2),
    depura(Lista, RestoFuente, RestoDestino), !.
depura(Lista, [_|RestoFuente], RestoDestino) :-
    depura(Lista, RestoFuente, RestoDestino).

% =====================================================================
%   depura/2.
%   depura(+Lista, -ListaDepurada).
%   Genera una lista ListaDepurada con los elementos no duplicados de
%   Lista.
% =====================================================================
depura(Lista, ListaDepurada) :-
    depura(Lista, Lista, Tmp),
    list_to_set(Tmp, ListaDepurada).

% =====================================================================
%   10. [ ] maximo/2. Identificar el mayor valor de entre aquellos
%   contenidos en una lista.
%
%                   maximo(<lista>, <resultado>).
%
%   Verdadero si <resultado> es el mayor valor numérico contenido en
%   <lista>. No todos los elementos necesitan ser numéricos. Ejemplos:
%
%               maximo([a,b,1,2,c,a,2,1,4], R). R = 2.
%               maximo([1,1,2,2,3,3], R). R = 3.
%               maximo([a,b,c,d], R). R = 0.
% =====================================================================

% =====================================================================
%   maximo/3.
%   maximo(+Lista, +Referencia, -Máximo).
%   Obtiene el elemento numérico con el máximo valor de Lista partiendo
%   de Referencia. Dicho elemento es devuelto en Máximo.
% =====================================================================
maximo([], Referencia, Referencia).
maximo([Elemento|Resto], Referencia, Maximo) :-
    Referencia >= 0,
    number(Elemento),
    (Elemento >= Referencia -> maximo(Resto, Elemento, Maximo); maximo(Resto, Referencia, Maximo)), !.
maximo([Elemento|Resto], Referencia, Maximo) :-
    Referencia >= 0,
    \+ number(Elemento),
    maximo(Resto, Referencia, Maximo).

% =====================================================================
%   maximo/2.
%   maximo(+Lista, -Máximo).
%   Devuelve en Máximo el elemento numérico de Lista con el máximo
%   valor.
% =====================================================================
maximo(Lista, Maximo) :-
    maximo(Lista, 0, Maximo).

% =====================================================================
%   11. [ ] anti_consonante/2. Elimina de una lista todos los elementos
%   que sean consonantes o que tengan longitud mayor a una letra.
%
%               anti_consonante(<lista>, <resultado>).
%
%   Verdadero si <resultado> es una lista con los mismos elementos que
%   <lista> excepto aquellos elementos que sean consonantes del
%   abecedario. Ejemplos:
%
%       anti_consonante([a,b,1,2,c,a,d,e], R). R = [a,1,2,a,e].
%       anti_consonante([f,g,h,i,j], R). R = [].
%       anti_consonante([a,aa,e,eee,i,ii,o,u], R). R = [a,e,i,o,u].
% =====================================================================

% =====================================================================
%   anti_consonante/2.
%   anti_consonante(+Lista, -Resultado).
%   Elimina de Lista los elementos que sean consonantes o que tengan
%   una longitud mayor a 1. La lista resultante es devuelta en
%   Resultado.
% =====================================================================
anti_consonante([], []).
anti_consonante([Elemento|RestoFuente], [Elemento|RestoDestino]) :-
    (number(Elemento);
    (memberchk(Elemento, [a, e, i, o, u]), atom_length(Elemento, 1))),
    anti_consonante(RestoFuente, RestoDestino), !.
anti_consonante([_|RestoFuente], Destino) :-
    anti_consonante(RestoFuente, Destino).

% =====================================================================
%   12. [ ] vocales/2. Identificar el mayor valor de entre aquellos
%   contenidos en una lista.
%
%                   vocales(<lista>, <resultado>).
%
%   Verdadero si <resultado> es una lista conteniendo exclusivamente
%   los elementos de <lista> que son vocales, en el mismo orden y
%   número que en la lista original, pero representadas como
%   mayúsculas. Ejemplos:
%
%           vocales([a,b,c,d,e,f,g], R). R = [A,E].
%           vocales([f,g,h,i,z,i,a,e], R). R = [ I,I,A,E].
%           vocales([r,s,t,u], R). R = [U].
% =====================================================================

% =====================================================================
%   vocales/2.
%   vocales(+Lista, -Resultado).
%   Devuelve en Resultado las vocales de Lista pero en mayúscula.
% =====================================================================
vocales([], []).
vocales([Elemento|RestoFuente], [Valor|RestoDestino]) :-
    memberchk(Elemento, [a, e, i, o, u]),
    nth0(Index, [a, e, i, o, u], Elemento),
    nth0(Index, ['A', 'E', 'I', 'O', 'U'], Valor),
    vocales(RestoFuente, RestoDestino), !.
vocales([_|RestoFuente], Destino) :-
    vocales(RestoFuente, Destino).

% =====================================================================
%   13. [ ] cada_dos/2. Selecciona los elementos intercalados de la
%   lista oringial a partir del tercer elemento.
%
%                   cada_dos(<lista>, <resultado>).
%
%   Verdadero si <resultado> es una lista conteniendo exclusivamente
%   los elementos intercalados de uno en uno, a partir del tercer
%   elemento en <lista>. Ejemplos:
%
%           cada_dos([1,a,2,3,b,c,4,5], R). R = [2,b,4].
%           cada_dos([x, y], R). R = [].
%           cada_dos([0,0,1,0,2,0,3,0,4], R). R = [1,2,3,4].
% =====================================================================

% =====================================================================
%   obtener_posiciones_impares/2.
%   obtener_posiciones_impares(+Lista, -Impares).
%   Considerando que la secuencia de índices de Lista comienza en 1,
%   devuelve en Impares la lista con los elementos de Lista de las
%   posiciones impares.
% =====================================================================
obtener_posiciones_impares([], []).
obtener_posiciones_impares([Elemento], [Elemento]) :- !.
obtener_posiciones_impares([Elemento, _|RestoFuente], [Elemento|RestoDestino]) :-
    obtener_posiciones_impares(RestoFuente, RestoDestino).

% =====================================================================
%   cada_dos/2.
%   cada_dos(+Lista, -Resultado).
%   Devuelve en Resultado los elementos intercalados de Lista partiendo
%   del tercer elemento.
% =====================================================================
cada_dos([_, _|Resto], Resultado) :-
    obtener_posiciones_impares(Resto, Resultado).

% =====================================================================
%   14. [ ] contexto/3. Identifica los elementos contexto de una
%   posición indicada en una lista.
%
%               contexto(<lista>, <elemento>,<resultado>).
%
%   Verdadero si <resultado> es una lista conteniendo exclusivamente el
%   elemento anterior y el posterior, en la lista original, de cada
%   instancia de <elemento>. Ejemplos:
%
%               contexto([1,2,3,4,5,6], 4, R). R = [3,5].
%               contexto([a,b,c], c, R). R = [b, []].
%               contexto([1], 1, R). R = [[], []].
% =====================================================================

% =====================================================================
%   obtener_elemento/3.
%   obtener_elemento(+Lista, +Posición, -Elemento).
%   Devuelve en Elemento el elemento de Lista en la posición
%   Posición. Si posición está fuera del intervalo de índices de Lista,
%   devuelve la lista vacía.
% =====================================================================
obtener_elemento(Lista, Posicion, []) :-
    length(Lista, Longitud),
    (Posicion =< 0; Posicion > Longitud), !.
obtener_elemento(Lista, Posicion, Elemento) :-
    nth1(Posicion, Lista, Elemento).

% =====================================================================
%   contexto/4.
%   contexto(+Lista, +Elemento, +Contexto).
%   Contexto es una lista con los elementos anterior y posterior a
%   Elemento en Lista. Si Elemento es el primer elemento de Lista, su
%   antecesor es la lista vacía. Si Elemento es el último elemento de
%   Lista, su sucesor es la lista vacía.
% =====================================================================
contexto(Lista, Elemento, [Anterior, Siguiente]) :-
    nth1(Posicion, Lista, Elemento),
    PosicionAnterior is Posicion - 1,
    PosicionSiguiente is Posicion + 1,
    obtener_elemento(Lista, PosicionAnterior, Anterior),
    obtener_elemento(Lista, PosicionSiguiente, Siguiente).

% =====================================================================
%   15. [ ] particiona/3. Parte una lista en dos listas a partir de
%   alguna posición en la lista inicial.
%
%           particiona(<lista>, <pos1>,<lista1>,<lista2>).
%
%   Verdadero si <lista1> y <lista2> son listas conteniendo los mismos
%   elementos de <lista> separados a partir de la posición <pos1>
%   considerando el primer elemento como en la posición 1. Ejemplos:
%
%   particiona([1,2,3,4,5,6], 4, L1, L2). L1 = [1,2,3]. L2 = [4,5,6].
%   particiona([a,b,c], 2,L1,L2). L1 = [a]. L2 = [b,c].
%   particiona([1], 3, L1,L2). false.
%   particiona([a,b,c,d], 5,L1,L2). L1 = [a,b,c,d]. L2 = []
% =====================================================================

% =====================================================================
%   particion_izquierda/3.
%   particion_izquierda(+Lista, +Elemento, -Partición).
%   Partición es una lista con los elementos de Lista desde el primero
%   hasta el elemento antes de Elemento.
% =====================================================================
particion_izquierda(_, 1, []) :- !.
particion_izquierda([Elemento|RestoFuente], Contador, [Elemento|RestoDestino]) :-
    Contador > 1,
    Siguiente is Contador - 1,
    particion_izquierda(RestoFuente, Siguiente, RestoDestino).

% =====================================================================
%   particiona/4.
%   particiona(+Lista, +Elemento, +PartIzquierda, +PartDerecha).
%   Parte Lista en dos listas PartIzquierda y PartDerecha en Elemento.
%   PartIzquierda es la lista con los elementos de Lista desde el
%   primero hasta el elemento anterior a Elemento. PartDerecha es la
%   lista con los elementos de Lista desde Elemento hasta el final.
% =====================================================================
particiona(Lista, Divisor, Lista1, Lista2) :-
    particion_izquierda(Lista, Divisor, Lista1),
    append(Lista1, Lista2, Lista).
