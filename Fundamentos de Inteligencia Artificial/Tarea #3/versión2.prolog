% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #3: Problemas tipo cebra.
%
%   - Resuelva en Prolog los tres problemas recortados de tipo cebraque
%   se describen en las láminas #43 y #44 de la presentación #15
%   Razonamiento por unificación.
%
%   - Entregue cada problema en un archivo diferente (suba 3 archivos
%   como solución a esta tarea).
%
%   - Puede, si así lo desea, resolver en SWISH y entregar sólo la liga
%   a los archivos originales.
%
%   VERSIÓN #2:
%       3 casas, 4 atributos y 6 pistas.
%
%       1. El brasileño NO vive en la segunda casa.
%       2. El dueño de perros juega baloncesto.
%       3. Hay una casa intermedia entre la del que juega fútbol y la
%       casa roja.
%       4. El dueño de peces vive junto al dueño de gatos.
%       5. El dueño de perros vive junto a la casa verde.
%       6. Un alemán vive en la tercera casa.
%
%   En esta versión, una casa es modelada con el predicado casa/4:
%
%           casa(<color>, <nacionalidad>, <deporte>, <animal>)
%
%   Para visualizar las soluciones parciales al problema tipo cebra de
%   esta versión, utilice el predicado visualiza_vecindario/1.
%
%               visualiza_vecindario(<lista de casas>).
% =====================================================================

% =====================================================================
%   una_casa_entre/3.
%   una_casa_entre(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) con una
%   subsecuencia en la que se encuentra Casa1 seguida de una casa
%   arbitraria, seguida de Casa2.
% =====================================================================
una_casa_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa1, _, Casa2|_], Vecindario).
una_casa_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa2, _, Casa1|_], Vecindario).

% =====================================================================
%   junto/3.
%   junto(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas en la que Casa1 y Casa2 se
%   encuentran una junto a la otra.
% =====================================================================
junto(Casa1, Casa2, Vecindario) :- append(_, [Casa1, Casa2|_], Vecindario).
junto(Casa1, Casa2, Vecindario) :- append(_, [Casa2, Casa1|_], Vecindario).

% =====================================================================
%   visualiza_vecindario/1.
%   visualiza_vecindario(-Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) cumple con
%   las clausulas consecuentes de visualiza_vecindario/1.
% =====================================================================
visualiza_vecindario(V) :-
    V = [_, _, _],
    (V = [casa(_, brasil, _, _), _, _]; V = [_, _, casa(_, brasil, _, _)]),
    member(casa(_, _, baloncesto, perro), V),
    una_casa_entre(casa(_, _, fútbol, _), casa(rojo, _, _, _), V),
    junto(casa(_, _, _, pez), casa(_, _, _, gato), V),
    junto(casa(_, _, _, perro), casa(verde, _, _, _), V),
    V = [_, _, casa(_, alemania, _, _)].
