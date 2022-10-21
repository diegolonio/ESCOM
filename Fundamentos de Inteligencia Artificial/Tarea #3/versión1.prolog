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
%   VERSIÓN #1:
%       Sólo 3 casas con 2 atributos y 3 pistas.
%
%       1. El español vive junto a la casa roja.
%       2. El noruego vive en la casa azul.
%       3. Un italiano vive en la segunda casa.
%
%   En esta versión, una casa es modelada con el predicado casa/2:
%
%                   casa(<color>, <nacionalidad>).
%
%   Para visualizar las soluciones parciales al problema tipo cebra de
%   esta versión, utilice el predicado visualiza_vecindario/1.
%
%               visualiza_vecindario(<lista de casas>).
% =====================================================================

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
    junto(casa(_, españa), casa(rojo, _), V),
    member(casa(azul, noruega), V),
    V = [_, casa(_, italia), _].
