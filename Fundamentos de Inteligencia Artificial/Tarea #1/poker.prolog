% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #1: Modelado de "manos" en Poker
%
%   - Modele  el  dominio Poker  en  una  base  de  conocimiento Prolog
%   y construya  un  programa  para generar  aleatoriamente "manos" de
%   Poker, compararlas y ordenarlas descendentemente.
%
%   - El predicado principal  "juega_Poker/1"  recibe como argumento el
%   número de manos que se deben generar aleatoriamente.
%
%   - El ordenamiento y despliegue del resultado debe coincidir con el
%   ejemplo de la lámina #38 en la presentación #5 (Edición de la base
%   de conocimiento).
%   
%   Para poner en marcha el programa, utilice el predicado
%   juega_Poker/1; el cual se define como:
%
%   		juega_Poker(<número de juegos a generar>).
%
%   Sea N el número de juegos a generar y considerando una baraja
%   tradicional inglesa sin comodines (52 cartas), el proceso para
%   generar aleatoriamente y clasificar los juegos es:
%
%   - Generar el mazo con N // 2 barajas.
%   - Obtener aleatoriamente del mazo N juegos de 5 cartas cada uno.
%   - Identificar la mano de cada juego.
%   - Ordenar y mostrar descendentemente las manos de acuerdo con su
%   clasificación y el valor de la carta más alta entre manos de la
%   misma clasificación.
% 
%   Este proceso es destilado en los predicados:
%
%   - calcular_numero_barajas(<número de juegos a generar, <número de barajas>).
%   - generar_mazo(<número de barajas>, <baraja>, <mazo>).
%   - generar_juegos(<número de juegos a generar>, <mazo>, <juegos>).
%   - identificar_manos(<juegos>, <manos>).
%   - ponderar_manos(<manos>, <ponderación>).
%   - resumen(<número de manos distintas identificadas>, <lugar inicial>, <ponderación>).
%
%   NOTA: Se diferencia un juego de una mano en el hecho de que un
%   juego es un conjunto de 5 cartas tomadas aleatoriamente del mazo
%   y una mano es el subconjunto de cartas de un juego que representa
%   la clasificación más alta de mano encontrada en el mismo.
% =====================================================================

% =====================================================================
%   baraja/1.
%   baraja(-Baraja).
%   El predicado recibe como argumento una variable en la cual
%   devolverá una representación de la baraja inglesa con una lista de
%   listas donde cada una de ellas representa una carta en la forma
%   [Valor, Palo] con 2 <= Valor <= 14 y
%   Palo ∈ {'\u2666', '\u2665', '\u2663', '\u2660'}.
% =====================================================================
baraja(Baraja) :-
    findall([Valor, Palo], (between(2, 14, Valor), member(Palo, ['\u2666', '\u2665', '\u2663', '\u2660'])), Tmp),
    random_permutation(Tmp, Baraja).

% =====================================================================
%   mono/2.
%   mono(+Valor, -Símbolo).
%   Retorna el símbolo correspondiente a los cuatro valores más altos
%   de la baraja inglesa tradicional.
% =====================================================================
mono(11, 'J').
mono(12, 'Q').
mono(13, 'K').
mono(14, 'A').

% =====================================================================
%   carta/2.
%   carta(+Carta, -Cadena).
%   Este predicado recibe como argumento una lista Carta, con
%   Carta ∈ X y baraja(X); y devuelve en Cadena una cadena de
%   caracteres que representa a Carta en forma de texto.
% =====================================================================
carta([Valor, Palo], Cadena) :-
    between(11, 14, Valor),
    mono(Valor, Mono),
    atomic_concat(Mono, Palo, Cadena), !.
carta([Valor, Palo], Cadena) :-
    between(2, 10, Valor),
    atomic_concat(Valor, Palo, Cadena).

% =====================================================================
%   nombre_mano/2.
%   nombre_mano(+PonderacionMano, -NombreMano).
%   Para efectos de esta base de conocimiento, las diferentes manos que
%   se pueden presentar en un juego de Poker son ponderadas en la
%   manera que una Flor Imperial se pondera con el valor 9 y la
%   inexistencia de alguna mano con el valor 0.
%   nombre_mano/2 recibe como argumento la ponderación de una mano y
%   devuelve el nombre asociado a la ponderación de dicha mano.
% =====================================================================
nombre_mano(0, 'NADA').
nombre_mano(1, 'Par').
nombre_mano(2, 'Doble Par').
nombre_mano(3, 'Tercia').
nombre_mano(4, 'Escalera').
nombre_mano(5, 'Color').
nombre_mano(6, 'Full House').
nombre_mano(7, 'Poker').
nombre_mano(8, 'Flor').
nombre_mano(9, 'Flor Imperial').

% =====================================================================
%   par/2.
%   par(+Juego, -Mano).
%   par/2 recibe un juego y determina si juego presenta la mano Par.
%   Devuelve las cartas que conforman dicha mano en la variable Mano.
%   Mano ⊂ Juego y Juego ⊂ X y baraja(X).
% =====================================================================
par([[V, PA], [V, PB], _, _, _], [[V, PA], [V, PB]]).
par([_, [V, PA], [V, PB], _, _], [[V, PA], [V, PB]]).
par([_, _, [V, PA], [V, PB], _], [[V, PA], [V, PB]]).
par([_, _, _, [V, PA], [V, PB]], [[V, PA], [V, PB]]).

% =====================================================================
%   doble_par/2.
%   doble_par(+Juego, -Mano).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Doble Par. Devuelve las cartas que conforman dicha mano en la
%   variable Mano. Mano ⊂ Juego y Juego ⊂ X y baraja(X).
% =====================================================================
doble_par([[V1, PA], [V1, PB], [V2, PC], [V2, PD], _], [[V1, PA], [V1, PB], [V2, PC], [V2, PD]]).
doble_par([[V1, PA], [V1, PB], _, [V2, PC], [V2, PD]], [[V1, PA], [V1, PB], [V2, PC], [V2, PD]]).
doble_par([_, [V1, PA], [V1, PB], [V2, PC], [V2, PD]], [[V1, PA], [V1, PB], [V2, PC], [V2, PD]]).

% =====================================================================
%   tercia/2.
%   tercia(+Juego, -Mano).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Tercia. Devuelve las cartas que conforman dicha mano en la
%   variable Mano. Mano ⊂ Juego y Juego ⊂ X y baraja(X).
% =====================================================================
tercia([[V, PA], [V, PB], [V, PC], _, _], [[V, PA], [V, PB], [V, PC]]).
tercia([_, [V, PA], [V, PB], [V, PC], _], [[V, PA], [V, PB], [V, PC]]).
tercia([_, _, [V, PA], [V, PB], [V, PC]], [[V, PA], [V, PB], [V, PC]]).

% =====================================================================
%   escalera/1.
%   escalera(+Juego).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Escalera. Juego ⊂ X y baraja(X).
% =====================================================================
escalera([[VA, _], [VB, _], [VC, _], [VD, _], [VE, _]]) :-
    VB is VA + 1,
    VC is VA + 2,
    VD is VA + 3,
    VE is VA + 4.

% =====================================================================
%   color/1.
%   color(+Juego).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Color. Juego ⊂ X y baraja(X).
% =====================================================================
color([[_, P], [_, P], [_, P], [_, P], [_, P]]).

% =====================================================================
%   full_house/2.
%   full_house(+Juego, -Mano).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Full House. Devuelve las cartas que conforman dicha mano en la
%   variable Mano. Mano ⊂ Juego y Juego ⊂ X y baraja(X).
% =====================================================================
full_house([[VA, _], [VA, _], [VB, _], [VB, _], [VB, _]]).
full_house([[VA, _], [VA, _], [VA, _], [VB, _], [VB, _]]).

% =====================================================================
%   poker/2.
%   poker(+Juego, -Mano).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Poker. Devuelve las cartas que conforman dicha mano en la
%   variable Mano. Mano ⊂ Juego y Juego ⊂ X y baraja(X).
% =====================================================================
poker([[V, PA], [V, PB], [V, PC], [V, PD], _], [[V, PA], [V, PB], [V, PC], [V, PD]]).
poker([_, [V, PA], [V, PB], [V, PC], [V, PD]], [[V, PA], [V, PB], [V, PC], [V, PD]]).

% =====================================================================
%   flor/1.
%   flor(+Juego).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Flor. Juego ⊂ X y baraja(X).
% =====================================================================
flor([[VA, P], [VB, P], [VC, P], [VD, P], [VE, P]]) :-
    VB is VA + 1,
    VC is VA + 2,
    VD is VA + 3,
    VE is VA + 4.

% =====================================================================
%   flor_imperial/1.
%   flor_imperial(+Juego).
%   Este predicado recibe un juego y determina si juego presenta la
%   mano Flor Imperial. Juego ⊂ X y baraja(X).
% =====================================================================
flor_imperial([[10, P], [11, P], [12, P], [13, P], [14, P]]).

% =====================================================================
%   particion/1.
%   particion(+Lista, +Elemento, -PartIzquierda, -PartDerecha).
%   Este predicado divide a Lista en dos particiones: una a la derecha
%   de Elemento y la otra a su izquierda.
% =====================================================================
particion([X|Resto], Pivote, [X|ParticionIzquierda], ParticionDerecha) :-
    X = _-ManoX,
    Pivote = _-ManoPivote,
    last(ManoX, [ValorX, _]),
    last(ManoPivote, [ValorPivote, _]),
    ValorX =< ValorPivote,
    particion(Resto, Pivote, ParticionIzquierda, ParticionDerecha), !.
particion([X|Resto], Pivote, ParticionIzquierda, [X|ParticionDerecha]) :-
    X = _-ManoX,
    Pivote = _-ManoPivote,
    last(ManoX, [ValorX, _]),
    last(ManoPivote, [ValorPivote, _]),
    ValorX > ValorPivote,
    particion(Resto, Pivote, ParticionIzquierda, ParticionDerecha), !.
particion([], _, [], []).

% =====================================================================
%   quicksort/2.
%   quicksort(+ManosDesordenadas, -ManosOrdenadas).
%   Implementación del algoritmo de ordenamiento Quick Sort con
%   predicados. Recibe una lista de manos de Poker y devuelve la misma
%   lista pero con las manos ordenadas ascendentemente con respecto del
%   valor de la última carta de cada mano.
%   ManosDesordenadas, ManoOrdenada ⊂ X y baraja(X). 
% =====================================================================
quicksort([], []) :- !.
quicksort([Pivote|Resto], ListaOrdenada) :-
    particion(Resto, Pivote, ParticionIzquierda, ParticionDerecha),
    quicksort(ParticionIzquierda, ParticionIzquierdaOrdenada),
    quicksort(ParticionDerecha, ParticionDerechaOrdenada),
    append(ParticionIzquierdaOrdenada, [Pivote|ParticionDerechaOrdenada], ListaOrdenada).

% =====================================================================
%   calcular_numero_barajas/2.
%   calcular_numero_barajas(+NumeroJugadores, -NumeroBarajas).
%   Calcula el número de barajas que debe contener un mazo con base en
%   el valor de NumeroJugadores.
% =====================================================================
calcular_numero_barajas(1, 1) :- !.
calcular_numero_barajas(N, M) :- N > 1, M is N // 2.

% =====================================================================
%   generar_mazo/3.
%   generar_mazo(+NumeroBarajas, +Baraja, -Mazo).
%   Dado el número de barajas que debe contener el mazo, el predicado
%   devuelve en Mazo una lista que contiene los elementos de Baraja
%   repetidos NumeroBarajas veces.
% =====================================================================
generar_mazo(NumeroBarajas, Baraja, Mazo) :-
    NumeroBarajas > 0,
    Baraja \= [],
    length(Tmp1, NumeroBarajas),
    maplist(=(Baraja), Tmp1),
    append(Tmp1, Tmp2),
    random_permutation(Tmp2, Mazo).

% =====================================================================
%   generar_juego/4.
%   generar_juego(+NumeroCartas, +Mazo, -Juego, -NuevoMazo).
%   Genera una lista de NumeroCartas elementos tomados de Mazo, que
%   devuelve en Juego. NuevoMazo es Mazo sin los elementos de Juego.
% =====================================================================
generar_juego(0, Mazo, [], Mazo) :- !.
generar_juego(N, Mazo, [Carta|RestoDestino], NuevoMazo) :-
    N > 0,
    Mazo \= [],
    random_member(Carta, Mazo),
    select(Carta, Mazo, MazoActualizado), !,
    M is N - 1,
    generar_juego(M, MazoActualizado, RestoDestino, NuevoMazo).

% =====================================================================
%   generar_juegos/3.
%   generar_juegos(+NumeroJuegos, +Mazo, -Juegos).
%   Genera NumeroJuegos listas, cada una con dos elementos que
%   corresponden con el número de jugador y una lista con 5 elementos
%   de Mazo. Las listas son devueltas en una sola lista en Juegos.
% =====================================================================
generar_juegos(0, _, []) :- !.
generar_juegos(NumeroJugador, Mazo, [NumeroJugador-JuegoOrdenado|RestoDestino]) :-
    NumeroJugador > 0,
    Mazo \= [],
    generar_juego(5, Mazo, Juego, NuevoMazo),
    msort(Juego, JuegoOrdenado),
    M is NumeroJugador - 1,
    generar_juegos(M, NuevoMazo, RestoDestino).

% =====================================================================
%   identificar_manos/2.
%   identificar_manos(+Juegos, -Manos).
%   Dada una lista Juegos que contiene un determinado número de juegos
%   de Poker, identifica la mayor mano posible en cada juego y asigna
%   su ponderación asociada al juego correspondiente.
% =====================================================================
identificar_manos([], []) :- !.
identificar_manos([NumeroJugador-Juego|JuegosRestantes], Ponderacion) :-
    ((flor_imperial(Juego), !, Ponderacion = [9-(NumeroJugador-Juego)|PonderacionRestante]);
    (flor(Juego), !, Ponderacion = [8-(NumeroJugador-Juego)|PonderacionRestante]);
    (poker(Juego, Mano), !, Ponderacion = [7-(NumeroJugador-Mano)|PonderacionRestante]);
    (full_house(Juego), !, Ponderacion = [6-(NumeroJugador-Juego)|PonderacionRestante]);
    (color(Juego), !, Ponderacion = [5-(NumeroJugador-Juego)|PonderacionRestante]);
    (escalera(Juego), !, Ponderacion = [4-(NumeroJugador-Juego)|PonderacionRestante]);
    (tercia(Juego, Mano), !, Ponderacion = [3-(NumeroJugador-Mano)|PonderacionRestante]);
    (doble_par(Juego, Mano), !, Ponderacion = [2-(NumeroJugador-Mano)|PonderacionRestante]);
    (par(Juego, Mano), !, Ponderacion = [1-(NumeroJugador-Mano)|PonderacionRestante]);
    (Ponderacion = [0-(NumeroJugador-Juego)|PonderacionRestante])),
    identificar_manos(JuegosRestantes, PonderacionRestante).

% =====================================================================
%   ponderar_manos/2.
%   ponderar_manos(+Manos, -ManosOrdenadas).
%   Ordena descendentemente las manos contenidas en Manos de acuerdo
%   con su ponderación y con el valor de la carta más alta de cada
%   mano.
% =====================================================================
ponderar_manos(Manos, ManosOrdenadas) :-
    msort(Manos, Tmp1),
    group_pairs_by_key(Tmp1, Tmp2),
    pairs_keys_values(Tmp2, Ponderaciones, Grupos),
    maplist(quicksort, Grupos, Tmp3),
    maplist(reverse, Tmp3, Tmp4),
    pairs_keys_values(Tmp5, Ponderaciones, Tmp4),
    reverse(Tmp5, ManosOrdenadas).

% =====================================================================
%   imprimir_mano/1.
%   imprimir_mano(+Mano).
%   Imprime las cartas de una mano de Poker representada con listas
%   Prolog como una sucesión de cadenas consultadas con el predicado
%   carta/2.
% =====================================================================
imprimir_mano([]) :- !.
imprimir_mano([Carta|CartasRestantes]) :-
    carta(Carta, Simbolo),
    write(Simbolo),
    write(' '),
    imprimir_mano(CartasRestantes).

% =====================================================================
%   resumen/4.
%   resumen(+NumeroRegistro, +Ponderacion, +Manos, -NumeroSiguienteRegistro).
%   Imprime todas las manos Manos de una ponderación Ponderacion junto
%   con su información: número de registro NumeroRegistro, número de
%   jugador, nombre de la mano y representación de la mano como una
%   cadena. Devuelve en NumeroSiguienteRegistro el número de registro a
%   partir del cual seguirá el conteo de registros para la siguiente
%   ponderación.
% =====================================================================
resumen(NumeroRegistro, _, [], NumeroRegistro) :- !.
resumen(NumeroRegistro, Ponderacion, [Jugador-Mano|ManosRestantes], NumeroSiguienteRegistro) :-
    nombre_mano(Ponderacion, NombreMano),
    format('~1|~d~8|jugador-~d~21|~s~37|', [NumeroRegistro, Jugador, NombreMano]),
    imprimir_mano(Mano), nl,
    M is NumeroRegistro + 1,
    resumen(M, Ponderacion, ManosRestantes, NumeroSiguienteRegistro).

% =====================================================================
%   resumen/2.
%   resumen(+NumeroInicialRegistro, +Ponderacion).
%   Imprime la ponderación de las manos generadas en una tabla de
%   resumen. NumeroInicialRegistro es el número a partir del cual
%   comenzará la enumeración de los registros de una determinada
%   ponderación, NumeroSiguienteRegistro es el número a partir del cual
%   comenzará la enumeración de los registros de la siguiente
%   ponderación. 
% =====================================================================
resumen(_, []) :- !.
resumen(NumeroInicialRegistro, [Ponderacion-Manos|RestoPonderacion]) :-
    resumen(NumeroInicialRegistro, Ponderacion, Manos, NumeroSiguienteRegistro),
    resumen(NumeroSiguienteRegistro, RestoPonderacion).

% =====================================================================
%   juega_Poker/1.
%   juega_Poker(+NumeroJugadores).
%   El predicado principal. Recibe como argumento el número de juegos a
%   generar para clasificarlos y ordenarlos descendentemente con
%   respecto de la mano identificada en cada juego.
% =====================================================================
juega_Poker(NumeroJugadores) :-
    baraja(Baraja),
    calcular_numero_barajas(NumeroJugadores, NumeroBarajas),
    generar_mazo(NumeroBarajas, Baraja, Mazo),
    generar_juegos(NumeroJugadores, Mazo, Juegos),
    identificar_manos(Juegos, Manos),
    ponderar_manos(Manos, Ponderacion),
    write('Generando mano de cada jugador...'), nl,
    write('Lugar   Jugador      Mano            Contenido'), nl,
    write('========================================================='), nl,
    resumen(1, Ponderacion).
