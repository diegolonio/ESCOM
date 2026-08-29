% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #7: Agente jugador de Gato 4x4
%
%   - Construya en Prolog un agente jugador de Gato 4x4 siguiendo TODAS
%   las instrucciones que están en las láminas 31 a 39 de la
%   presentación #25 (Estrategia de juego).
%
%   - El predicado  principal  debe  parametrizar el nivel máximo de
%   produndización en el árbol de búsqueda y resolver usando tanto el
%   algoritmo Mini Max (Nega Max) como podas Alpha-Beta.
%
%   Para iniciar un nuevo juego utilice el predicado jugar/1:
%
%                       jugar(<profundidad>).
%
%   El predicado jugar/1 recibe como argumento el nivel máximo de
%   profundización del árbol de búsqueda y resuelve con Nega Max más
%   podas Alpha-Beta, que es la estrategia por omisión.
%
%   Para elegir explícitamente el algoritmo de búsqueda utilice el
%   predicado jugar/2:
%
%                   jugar(<profundidad>, minimax).
%                   jugar(<profundidad>, alfa_beta).
%
%   Los dos algoritmos recorren el mismo árbol y eligen exactamente la
%   misma jugada; la diferencia está en cuántos nodos necesita visitar
%   cada uno, cantidad que se reporta al terminar cada turno de la
%   máquina.
%
%   Para configurar el simbolo que utilizará cualquiera de los dos
%   jugadores, utilice el predicado modificar_símbolo/2:
%
%           modificar_símbolo(<jugador>, <nuevo_símbolo>).
% =====================================================================
:- dynamic(símbolo/2).
:- retractall(símbolo(_, _)).

% =====================================================================
%   símbolo/2.
%   símbolo(+Jugador, -Símbolo).
%   Verdadero si Símbolo es el símbolo de Jugador. El jugador 1 es
%   siempre el humano y el jugador 2 el agente artificial.
% =====================================================================
símbolo(1, x).
símbolo(2, o).

% =====================================================================
%   siguiente_turno/2.
%   siguiente_turno(+JugadorActual, -SiguienteJugador).
%   SiguienteJugador es el jugador del turno que sucede a
%   JugadorActual.
% =====================================================================
siguiente_turno(1,2).
siguiente_turno(2,1).

% =====================================================================
%   obtener_fila/2.
%   obtener_fila(+Letra, -NúmeroFila).
%   Realiza un mapeo de las letras [A, B, C, D] a los números
%   [1, 2, 3, 4] que serán los números de fila NúmeroFila del tablero.
% =====================================================================
obtener_fila('A', 1).
obtener_fila('B', 2).
obtener_fila('C', 3).
obtener_fila('D', 4).

% =====================================================================
%   victoria/1.
%   victoria(-Cota).
%   Cota es la magnitud que se asigna a un estado ganado. Es mayor que
%   cualquier valor que pueda devolver la evaluación heurística (que
%   está acotada por el número de líneas del tablero, 10), de modo que
%   ganar siempre sea preferible a cualquier posición sin resolver.
% =====================================================================
victoria(1000).

% =====================================================================
%   infinito/1.
%   infinito(-Cota).
%   Cota es el valor que se usa como infinito para inicializar la
%   ventana [Alfa, Beta] de la búsqueda.
% =====================================================================
infinito(100000).

% =====================================================================
%   modificar_símbolo/2.
%   modificar_símbolo(+Jugador, +NuevoSímbolo).
%   Permite reemplazar el símbolo con el que se identificará al jugador
%   Jugador en el tablero por NuevoSímbolo. El nuevo símbolo no puede
%   ser el guión (que representa a una celda vacía) ni el símbolo que
%   ya está usando el otro jugador.
% =====================================================================
modificar_símbolo(_, -) :-
    !,
    write('El guión representa a las celdas vacías, elige otro símbolo.'), nl,
    fail.
modificar_símbolo(Jugador, Símbolo) :-
    siguiente_turno(Jugador, Oponente),
    símbolo(Oponente, Símbolo),
    !,
    write('Ese símbolo ya lo está usando el otro jugador.'), nl,
    fail.
modificar_símbolo(Jugador, Símbolo) :-
    retract(símbolo(Jugador, _)),
    assert(símbolo(Jugador, Símbolo)).

% =====================================================================
%   sustituir/4.
%   sustituir(+Índice, +Lista, +Elemento, -NuevaLista).
%   Elemento sustituye al elemento en el índice Índice de Lista, la
%   nueva lista es devuelta en NuevaLista.
% =====================================================================
sustituir(Índice, Lista, NuevoElemento, NuevaLista) :-
    nth1(Índice, Lista, _, Resultado),
    nth1(Índice, NuevaLista, NuevoElemento, Resultado).

% =====================================================================
%   empate(+Tablero).
%   Verdadero si no existen celdas vacías (-) en Tablero.
% =====================================================================
empate([A,B,C,D]) :-
    \+ member(-,A),
    \+ member(-,B),
    \+ member(-,C),
    \+ member(-,D).

% =====================================================================
%   jugada_posible/2.
%   jugada_posible(+Estado, -NuevoEstado).
%   Verdadero sí NuevoEstado es descendiente de Estado tras haber
%   hecho una jugada el jugador en turno.
% =====================================================================
jugada_posible((Tablero, Jugador), (NuevoTablero, SiguienteJugador)) :-
    append(FilaSuperior, [Fila|FilaInferior], Tablero),
    append(ColumnaIzquierda, [-|ColumnaDerecha], Fila),
    símbolo(Jugador, Símbolo),
    append(ColumnaIzquierda, [Símbolo|ColumnaDerecha], NuevaFila),
    append(FilaSuperior, [NuevaFila|FilaInferior], NuevoTablero),
    siguiente_turno(Jugador, SiguienteJugador).

% =====================================================================
%   obtener_jugadas_posibles/2.
%   obtener_jugadas_posibles(+Estado, -JugadasPosibles).
%   JugadasPosibles es una lista con todos los posibles estados que
%   descienden de Estado tras haber realizado todas las jugadas
%   posibles (una para cada nuevo estado) el jugador en turno.
% =====================================================================
obtener_jugadas_posibles(Estado, JugadasPosibles) :-
    findall((NuevoTablero, SiguienteJugador), jugada_posible(Estado, (NuevoTablero, SiguienteJugador)), JugadasPosibles).

% =====================================================================
%   jugada_ganadora/2.
%   jugada_ganadora(+Símbolo, +Tablero).
%   Verdadero si existe una fila, columna o diagonal del tablero con
%   todas sus celdas del mismo símbolo Símbolo.
% =====================================================================
jugada_ganadora(Símbolo, Tablero) :-
    append(_, [[Símbolo, Símbolo, Símbolo, Símbolo]|_], Tablero);
    (Tablero = [A, B, C, D],
    append(DerechaA, [Símbolo|_], A),
    append(DerechaB, [Símbolo|_], B),
    append(DerechaC, [Símbolo|_], C),
    append(DerechaD, [Símbolo|_], D),
    length(DerechaA, Longitud),
    length(DerechaB, Longitud),
    length(DerechaC, Longitud),
    length(DerechaD, Longitud));
    Tablero = [[Símbolo, _, _, _], [_, Símbolo, _, _], [_, _, Símbolo, _], [_, _, _, Símbolo]];
    Tablero = [[_, _, _, Símbolo], [_, _, Símbolo, _], [_, Símbolo, _, _], [Símbolo, _, _, _]].

% =====================================================================
%   posibilidades_ganadoras/2.
%   posibilidades_ganadoras(+Símbolo, +Tablero).
%   Verdadero si existe alguna fila, columna o diagonal del tablero
%   cuyas celdas estén vacías o tengan el símbolo Símbolo. 
% =====================================================================
posibilidades_ganadoras(Símbolo, Tablero) :-
    (append(_, [[Celda1, Celda2, Celda3, Celda4]|_], Tablero);
    (Tablero = [A, B, C, D],
    append(Derecha1, [Celda1|_], A),
    append(Derecha2, [Celda2|_], B),
    append(Derecha3, [Celda3|_], C),
    append(Derecha4, [Celda4|_], D),
    length(Derecha1, Longitud),
    length(Derecha2, Longitud),
    length(Derecha3, Longitud),
    length(Derecha4, Longitud));
    Tablero = [[Celda1, _, _, _], [_, Celda2, _, _], [_, _, Celda3, _], [_, _, _, Celda4]];
    Tablero = [[_, _, _, Celda1], [_, _, Celda2, _], [_, Celda3, _, _], [Celda4, _, _, _]]),
    (Celda1 = Símbolo; Celda1 = (-)),
    (Celda2 = Símbolo; Celda2 = (-)),
    (Celda3 = Símbolo; Celda3 = (-)),
    (Celda4 = Símbolo; Celda4 = (-)).

% =====================================================================
%   evaluación_heurística/2.
%   evaluación_heurística(+Estado, -Evaluación).
%   Implementación declarativa de la función de evaluación heurística:
%
%                           f(e) = G - P
%
%   Donde Evaluación es f(e) y Estado es el estado que se va a evaluar.
%   El valor se mide siempre desde el punto de vista del agente
%   artificial (jugador 2): entre más grande, mejor para la máquina.
% =====================================================================
evaluación_heurística((Tablero, _), Evaluación) :-
    símbolo(2, SímboloAgente),
    símbolo(1, SímboloHumano),
    aggregate_all(count, posibilidades_ganadoras(SímboloAgente, Tablero), G),
    aggregate_all(count, posibilidades_ganadoras(SímboloHumano, Tablero), P),
    Evaluación is G - P.

% =====================================================================
%   evaluación_relativa/2.
%   evaluación_relativa(+Estado, -Evaluación).
%   Evaluación es f(e) expresada desde el punto de vista del jugador
%   que tiene el turno en Estado. Nega Max necesita esta versión de la
%   heurística porque en él ambos jugadores maximizan: lo que cambia de
%   un nivel al siguiente es el signo con el que se lee el tablero.
% =====================================================================
evaluación_relativa((Tablero, 2), Evaluación) :-
    !,
    evaluación_heurística((Tablero, 2), Evaluación).
evaluación_relativa((Tablero, 1), Evaluación) :-
    evaluación_heurística((Tablero, 1), Agente),
    Evaluación is -Agente.

% =====================================================================
%   estado_terminal/3.
%   estado_terminal(+Estado, +Profundidad, -Evaluación).
%   Verdadero si Estado es un nodo terminal del árbol de búsqueda, es
%   decir, si la partida ya se acabó en él. Evaluación es su valor
%   visto por el jugador en turno.
%
%   Sólo hay que revisar si ganó el oponente: el único que acaba de
%   mover es él, así que es imposible llegar a un estado en el que el
%   jugador en turno tenga cuatro en línea.
%
%   Al valor de una derrota se le suma la profundidad que quedaba por
%   explorar para que, entre dos derrotas, el agente prefiera la más
%   lejana, y entre dos victorias, la más rápida.
% =====================================================================
estado_terminal((Tablero, Jugador), Profundidad, Evaluación) :-
    siguiente_turno(Jugador, Oponente),
    símbolo(Oponente, SímboloOponente),
    jugada_ganadora(SímboloOponente, Tablero),
    !,
    victoria(Victoria),
    Evaluación is -(Victoria + Profundidad).
estado_terminal((Tablero, _), _, 0) :-
    empate(Tablero),
    !.

% =====================================================================
%   nodos_explorados/1, reiniciar_contador/0, contar_nodo/0.
%   Llevan la cuenta de los nodos que visita la búsqueda. Sirven para
%   comparar el trabajo que hace Nega Max contra el que hace la versión
%   con podas Alpha-Beta sobre el mismo árbol.
% =====================================================================
:- nb_setval(nodos, 0).

reiniciar_contador :-
    nb_setval(nodos, 0).

contar_nodo :-
    nb_getval(nodos, Nodos),
    Siguiente is Nodos + 1,
    nb_setval(nodos, Siguiente).

nodos_explorados(Nodos) :-
    nb_getval(nodos, Nodos).

% =====================================================================
%   minimax/4.
%   minimax(+Estado, +Profundidad, -SiguienteJugada, -Evaluación).
%   Implementación declarativa del algoritmo Mini Max en su forma Nega
%   Max, con nivel de profundización máximo Profundidad.
%
%   En Nega Max no hay un nivel "mini" y otro "max": los dos jugadores
%   maximizan, y el valor de un nodo es el negativo del mejor valor que
%   consigue el oponente en sus descendientes,
%
%                   f(e) = max { -f(h) : h hijo de e }
%
%   lo que evita tener que escribir dos recursiones distintas.
% =====================================================================
minimax(Estado, Profundidad, SiguienteJugada, Evaluación) :-
    contar_nodo,
    (   estado_terminal(Estado, Profundidad, Valor)
    ->  Evaluación = Valor
    ;   Profundidad =< 0
    ->  evaluación_relativa(Estado, Evaluación)
    ;   obtener_jugadas_posibles(Estado, [Primera|Resto]),
        M is Profundidad - 1,
        infinito(Infinito),
        MenosInfinito is -Infinito,
        obtener_mejor_jugada([Primera|Resto], M, Primera, MenosInfinito, SiguienteJugada, Evaluación)
    ).

% =====================================================================
%   obtener_mejor_jugada/6.
%   obtener_mejor_jugada(+Jugadas, +Profundidad, +MejorParcial,
%                        +EvaluaciónParcial, -MejorJugada, -MejorEvaluación).
%   Recorre la lista de estados Jugadas evaluando cada uno con Nega Max
%   a lo más Profundidad niveles más abajo. MejorParcial y
%   EvaluaciónParcial son la mejor jugada encontrada hasta el momento y
%   su valor; al agotarse la lista se devuelven en MejorJugada y
%   MejorEvaluación.
% =====================================================================
obtener_mejor_jugada([], _, MejorParcial, EvaluaciónParcial, MejorParcial, EvaluaciónParcial).
obtener_mejor_jugada([Jugada|Resto], Profundidad, MejorParcial, EvaluaciónParcial, MejorJugada, MejorEvaluación) :-
    minimax(Jugada, Profundidad, _, EvaluaciónHija),
    Evaluación is -EvaluaciónHija,
    (   Evaluación > EvaluaciónParcial
    ->  obtener_mejor_jugada(Resto, Profundidad, Jugada, Evaluación, MejorJugada, MejorEvaluación)
    ;   obtener_mejor_jugada(Resto, Profundidad, MejorParcial, EvaluaciónParcial, MejorJugada, MejorEvaluación)
    ).

% =====================================================================
%   alfa_beta/4.
%   alfa_beta(+Estado, +Profundidad, -SiguienteJugada, -Evaluación).
%   Punto de entrada del algoritmo Nega Max con podas Alpha-Beta. Abre
%   la búsqueda con la ventana más amplia posible, (-inf, +inf).
% =====================================================================
alfa_beta(Estado, Profundidad, SiguienteJugada, Evaluación) :-
    infinito(Beta),
    Alfa is -Beta,
    alfa_beta(Estado, Profundidad, Alfa, Beta, SiguienteJugada, Evaluación).

% =====================================================================
%   alfa_beta/6.
%   alfa_beta(+Estado, +Profundidad, +Alfa, +Beta, -SiguienteJugada,
%             -Evaluación).
%   Nega Max con podas Alpha-Beta. Alfa es el valor que el jugador en
%   turno ya tiene asegurado y Beta el máximo que el oponente le va a
%   permitir; en cuanto un descendiente alcanza Beta, el resto de los
%   hermanos ya no se explora porque el oponente nunca dejaría que la
%   partida llegara hasta ahí.
%
%   Al bajar un nivel la ventana se invierte y se niega, (-Beta, -Alfa),
%   porque el hijo mide el tablero con el signo contrario.
% =====================================================================
alfa_beta(Estado, Profundidad, Alfa, Beta, SiguienteJugada, Evaluación) :-
    contar_nodo,
    (   estado_terminal(Estado, Profundidad, Valor)
    ->  Evaluación = Valor
    ;   Profundidad =< 0
    ->  evaluación_relativa(Estado, Evaluación)
    ;   obtener_jugadas_posibles(Estado, [Primera|Resto]),
        M is Profundidad - 1,
        podar([Primera|Resto], M, Alfa, Beta, Primera, SiguienteJugada, Evaluación)
    ).

% =====================================================================
%   podar/7.
%   podar(+Jugadas, +Profundidad, +Alfa, +Beta, +MejorParcial,
%         -MejorJugada, -MejorEvaluación).
%   Recorre los descendientes Jugadas manteniendo en Alfa el valor de
%   la mejor jugada encontrada hasta el momento (MejorParcial). En
%   cuanto Alfa alcanza a Beta corta el recorrido: esa es la poda.
% =====================================================================
podar([], _, Alfa, _, MejorParcial, MejorParcial, Alfa).
podar([Jugada|Resto], Profundidad, Alfa, Beta, MejorParcial, MejorJugada, MejorEvaluación) :-
    AlfaHija is -Beta,
    BetaHija is -Alfa,
    alfa_beta(Jugada, Profundidad, AlfaHija, BetaHija, _, EvaluaciónHija),
    Evaluación is -EvaluaciónHija,
    (   Evaluación > Alfa
    ->  NuevaAlfa = Evaluación, NuevaMejor = Jugada
    ;   NuevaAlfa = Alfa, NuevaMejor = MejorParcial
    ),
    (   NuevaAlfa >= Beta
    ->  MejorJugada = NuevaMejor, MejorEvaluación = NuevaAlfa
    ;   podar(Resto, Profundidad, NuevaAlfa, Beta, NuevaMejor, MejorJugada, MejorEvaluación)
    ).

% =====================================================================
%   buscar_jugada/5.
%   buscar_jugada(+Algoritmo, +Estado, +Profundidad, -SiguienteJugada,
%                 -Evaluación).
%   Selecciona con cuál de los dos algoritmos de búsqueda va a razonar
%   el agente artificial.
% =====================================================================
buscar_jugada(minimax, Estado, Profundidad, SiguienteJugada, Evaluación) :-
    minimax(Estado, Profundidad, SiguienteJugada, Evaluación).
buscar_jugada(alfa_beta, Estado, Profundidad, SiguienteJugada, Evaluación) :-
    alfa_beta(Estado, Profundidad, SiguienteJugada, Evaluación).

% =====================================================================
%   celda_válida/2.
%   celda_válida(+Tablero, [+Fila, +Columna]).
%   Verdadero si la celda en la intersección de Fila y Columna de
%   Tablero corresponde a una celda vacía (-).
% =====================================================================
celda_válida(Tablero, [Y, X]) :-
    nth1(Y, Tablero, Fila),
    nth1(X, Fila, Celda),
    Celda = (-).

% =====================================================================
%   jugada_humano/3.
%   jugada_humano(+Estado, [+Fila, +Columna], -NuevoEstado).
%   Llena la celda en la intersección de Fila y Columna del tablero de
%   Estado con el símbolo del jugador en turno y devuelve el nuevo
%   estado en NuevoEstado.
% =====================================================================
jugada_humano((Tablero, 1), [Y, X], (NuevoTablero, 2)) :-
    símbolo(1, SímboloHumano),
    nth1(Y, Tablero, Fila),
    sustituir(X, Fila, SímboloHumano, NuevaFila),
    sustituir(Y, Tablero, NuevaFila, NuevoTablero).

% =====================================================================
%   normalizar_fila/2.
%   normalizar_fila(+Entrada, -Letra).
%   Letra es el átomo en mayúscula que corresponde a Entrada, que puede
%   venir como átomo ('a') o como cadena ("A"). SWI-Prolog lee entre
%   comillas dobles una cadena y entre comillas simples un átomo, así
%   que hay que aceptar las dos para que el jugador humano pueda anotar
%   su jugada de cualquiera de las dos formas.
% =====================================================================
normalizar_fila(Entrada, Letra) :-
    (   string(Entrada)
    ->  atom_string(Átomo, Entrada)
    ;   atom(Entrada)
    ->  Átomo = Entrada
    ),
    upcase_atom(Átomo, Letra).

% =====================================================================
%   jugada_bien_formada/3.
%   jugada_bien_formada(+Tablero, +Entrada, -[Fila, Columna]).
%   Verdadero si Entrada es una jugada de la forma ["Fila", Columna]
%   que cae dentro del tablero y sobre una celda vacía.
% =====================================================================
jugada_bien_formada(Tablero, Entrada, [Y, X]) :-
    nonvar(Entrada),
    Entrada = [Fila, X],
    nonvar(Fila),
    normalizar_fila(Fila, Letra),
    obtener_fila(Letra, Y),
    integer(X),
    X >= 1,
    X =< 4,
    celda_válida(Tablero, [Y, X]).

% =====================================================================
%   leer_jugada/2.
%   leer_jugada(+Tablero, -[Fila, Columna]).
%   Pide al jugador humano las coordenadas de su jugada y se las vuelve
%   a pedir mientras no correspondan a una celda vacía del tablero.
% =====================================================================
leer_jugada(Tablero, Jugada) :-
    write('Ingresa la posición de tu jugada, por ejemplo ["A", 1]. : '),
    catch(read(Entrada), _, Entrada = ilegible),
    (   Entrada == end_of_file
    ->  nl, write('Se acabó la entrada, partida interrumpida.'), nl,
        throw(partida_interrumpida)
    ;   true
    ),
    (   jugada_bien_formada(Tablero, Entrada, Jugada)
    ->  true
    ;   nl, write('¡Ups! Jugada no válida, por favor vuelve a intentarlo.'), nl, nl,
        leer_jugada(Tablero, Jugada)
    ).

% =====================================================================
%   continuar/0.
%   Hace una pausa hasta que el jugador humano decida seguir.
% =====================================================================
continuar :-
    write('Ingresa cualquier letra (seguida de un punto) para continuar: '),
    catch(read(_), _, true).

% =====================================================================
%   mostrar_tablero/1.
%   mostrar_tablero(+Tablero).
%   Despliega en consola el tablero del Gato 4x4 en una forma amigable
%   para el jugador humano.
% =====================================================================
mostrar_tablero([A,B,C,D]) :-
    format('~t    ~w ~t ~t ~w ~t ~t ~w ~t ~t ~w ~t ~n', [1, 2, 3, 4]),
    format('~t  - ~t-~t - ~t-~t - ~t-~t - ~t-~t - ~n', []),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['A'|A]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n',[]),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['B'|B]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n', []),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['C'|C]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n',[]),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['D'|D]),
    format('~t  - ~t-~t - ~t-~t - ~t-~t - ~t-~t - ~n', []).

% =====================================================================
%   mostrar_análisis/3.
%   mostrar_análisis(+Algoritmo, +Evaluación, +Tiempo).
%   Reporta con qué algoritmo razonó la máquina, en cuántos
%   milisegundos, cuántos nodos del árbol tuvo que visitar y cómo
%   califica el estado al que llegó.
% =====================================================================
mostrar_análisis(Algoritmo, Evaluación, Tiempo) :-
    nodos_explorados(Nodos),
    format('[~w] ~d nodos explorados en ~d ms, f(e) = ~d.~n', [Algoritmo, Nodos, Tiempo, Evaluación]).

% =====================================================================
%   jugar_turno/3.
%   jugar_turno((+Tablero, +Humano), +Profundidad, +Algoritmo).
%   Permite al jugador Humano realizar una jugada en el tablero Tablero
%   para llamar posteriormente a jugar al agente artificial con una
%   profundidad de búsqueda de Profundidad niveles.
% =====================================================================
jugar_turno((Tablero, 1), Profundidad, Algoritmo) :-
    write('¡Es tu turno!'), nl, nl,
    leer_jugada(Tablero, [Y, X]),
    jugada_humano((Tablero, 1), [Y, X], (NuevoTablero, 2)),
    nl, mostrar_tablero(NuevoTablero), nl,
    símbolo(1, SímboloHumano),
    (   jugada_ganadora(SímboloHumano, NuevoTablero)
    ->  write('¡Felicidades, ganaste!'), nl
    ;   empate(NuevoTablero)
    ->  write('¡Empataste con la máquina!'), nl
    ;   continuar,
        jugar_turno((NuevoTablero, 2), Profundidad, Algoritmo)
    ).

% =====================================================================
%   jugar_turno/3.
%   jugar_turno((+Tablero, +Máquina), +Profundidad, +Algoritmo).
%   Permite al agente artificial calcular su siguiente jugada con la
%   ayuda del algoritmo de búsqueda Algoritmo.
% =====================================================================
jugar_turno((Tablero, 2), Profundidad, Algoritmo) :-
    nl, write('¡Ahora va la máquina!'), nl,
    reiniciar_contador,
    statistics(walltime, [Inicio, _]),
    buscar_jugada(Algoritmo, (Tablero, 2), Profundidad, (NuevoTablero, SiguienteJugador), Evaluación),
    statistics(walltime, [Fin, _]),
    Tiempo is Fin - Inicio,
    nl, mostrar_tablero(NuevoTablero), nl,
    mostrar_análisis(Algoritmo, Evaluación, Tiempo), nl,
    símbolo(2, SímboloAgente),
    (   jugada_ganadora(SímboloAgente, NuevoTablero)
    ->  write('¡Ay, no! Ganó la máquina. :C'), nl
    ;   empate(NuevoTablero)
    ->  write('¡Empataste con la máquina!'), nl
    ;   jugar_turno((NuevoTablero, SiguienteJugador), Profundidad, Algoritmo)
    ).

% =====================================================================
%   jugar/2.
%   jugar(+Profundidad, +Algoritmo).
%   Inicia una partida de Gato 4x4 en la que el agente artificial
%   tendrá una profundidad límite en su árbol de búsqueda de
%   Profundidad niveles y razonará con el algoritmo Algoritmo, que
%   puede ser minimax (Nega Max sin podas) o alfa_beta.
% =====================================================================
jugar(Profundidad, Algoritmo) :-
    integer(Profundidad),
    Profundidad > 0,
    member(Algoritmo, [minimax, alfa_beta]),
    !,
    TableroInicial = [[-, -, -, -], [-, -, -, -], [-, -, -, -], [-, -, -, -]],
    nl, mostrar_tablero(TableroInicial), nl,
    catch(jugar_turno((TableroInicial, 1), Profundidad, Algoritmo),
          partida_interrumpida,
          true).
jugar(_, _) :-
    write('Uso: jugar(<profundidad entera positiva>, <minimax | alfa_beta>).'), nl,
    fail.

% =====================================================================
%   jugar/1.
%   jugar(+Profundidad).
%   Inicia una partida de Gato 4x4 en la que el agente artificial
%   tendrá una profundidad límite en su árbol de búsqueda de
%   Profundidad niveles, resolviendo con Nega Max y podas Alpha-Beta.
% =====================================================================
jugar(Profundidad) :-
    jugar(Profundidad, alfa_beta).
