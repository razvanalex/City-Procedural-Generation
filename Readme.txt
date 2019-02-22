------------------------------------------------------------------------------
				Tema 3 - EGC
------------------------------------------------------------------------------

Autor: Smadu Razvan-Alexandru 335CB

README
In scena am randat 3 tipuri de cladiri, drumuri, copaci, masini si ground-ul.
Cele 3 tipuri de cladiri sunt:
- Tower = in care se genereaza cub peste cub astfel incat se genereaza o cladire.
Fiecare nivel are o grosime mai mica decat nivelul anterior. Intre nivele exista 
despartitoare care pot sa fie de acelasi nivel cu nivelul anterior, sau mai mari.
Acoperisul consta in mai multe paralelipipede una peste alta, ca o piramida. 
pe acoperis exista niste paralelipipede pt ventilatie.
-Clasic = este o cladire formata din mai multe paralelipipede care se intersecteaza.
Algoritmul presupune ca fiecare parte din cladire sa reprezinte un paralelipiped care
contine centrul cladirii. Fiecare parte are o inaltime aleatoriu aleasa. Acoperisul este 
la fel ca la Tower, doar ca se aplica doar pe "cladirea" principala.
- Cilindru = este asemanator cu tower, dar are un cilindru cu 30 de laturi drept model.

Gramaticile care descriu forma cladirilor sunt de forma celor regulate (S -> aB), deoarece
la fiecare iteratie se pune o componenta si se trece la urmatoarea.

Pe cladiri se afla cel mult 4 lumini de tip spot care se afla in jurul cladirii. Rotatia 
in jurul cladirii este aleator aleasa, iar directia de proiectare este varful cladirii.
De asemenea, si culorile sunt alese aleator. Gramatica pentru plasarea luminilor este 
urmatoarea: S -> 1A | eA; A -> 2B | eB; B -> 3C | eC; C -> 4e | e; unde 1,2,3,4 sunt 
luminile, iar e -> sirul vid. 

Textura cladirilor este una repetitiva.

Generarea strazilor:
Aceasta se realizeaza prin completarea unei matrici care va contine niste caractere ce 
pot desemna:
'h' -> drum orizontal
'c' -> intersectie
'v' -> drum vertical
'.' -> cladire sau orice alt ceva

Algoritmul este urmatorul:
Se alege aleatoriu directia. Apoi se genereaza o lungime aleatorie si se pozitioneaza
strada. Daca incape pe harta, este plasata conform conventiilor de mai sus (h, c, v).
Daca nu incape, se incearca din nou repetarea pasului. Daca nu merge, se opresete algoritmul.
Daca drumul e prea apropiat de alt drum, la fel, se repeta pasul. Pozitia noului drum va fi
centrata intr-un punct aleatoriu de pe vechiul drum. Astfel se asigura conectivitatea 
retelei de drumuri. Dezavantajul acestui algoritm este ca exista sansa sa se termine prea 
repede si sa nu genereze foarte multe drumuri.

Copacii sunt pozitionati in acelasi timp cu cladirile. Atunci cand o cladire nu 
incape in celula rezultata dintre drumuri, se plaseaza copacii. Acestia pot sa 
apara pe pozitia care este calculata, sau nu.

Masinile se afla pe drumuri. Acestea sunt generate in acelasi moment cu drumurile.
Se calculeaza toate pozitiile, insa se aleg doar unele, in mod aleatoriu. 
In caz de trafic maxim, masinile sunt una dupa alta, iar in trafic liber, nu e nicio
masina, sau numerul este mic. 

Luminile sunt plasate in functie de tipul drumului, in dreapta si in stanga sa.
In celulele de tip 'c' (intersectie), nu se plaseaza lumini.

Ground-ul este un panou, pe care este aplicata o textura repetitiva de zapada.

Nota: Programul poate sa consume memorie de ordinul GB.
