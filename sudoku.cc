/*
 *   Fichero de implementación sudoku.cc para el trabajo con sudokus de
 *   81 celdas y 9 cuadrantes.
 * 
 *  Copyright (C) 2015, David Abián <da [at] davidabian.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/*
 *  Los ficheros de texto que almacenan sudokus tienen el siguiente
 *  formato:
 * 
 *    <fichero> ::= <fila> <EOL> <fila> <EOL> <fila> <EOL> <fila> <EOL>
 *                  <fila> <EOL> <fila> <EOL> <fila> <EOL> <fila> <EOL>
 *                  <fila> [ <EOL> ]
 *    <fila> ::= <n> " " <n> " " <n> " " <n> " " <n> " " <n> " " <n> " "
 *               <n> " " <n> " "
 *    <n> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
 *
 *  Cada una de las líneas del fichero viene ocupada por una fila del
 *  sudoku, siendo cada una de sus celdas los enteros comprendidos entre
 *  0 y 9 presentes en dicha fila y separados en el fichero mediante un
 *  espacio sencillo, " " (ASCII 0x20).
 * 
 *  <EOL> (end of line) es el especificador de fin de línea en notación
 *  BNF.
 * 
 *  Cada entero contenido en el fichero representa su mismo entero en el
 *  sudoku, a excepción del 0, que representa una celda vacía.
 * 
 *  Un ejemplo de representación de un sudoku válido en un fichero de
 *  texto sería el siguiente.
 * 
 *  0 0 3 0 0 0 0 0 1 
 *  0 2 0 0 0 0 0 3 8 
 *  0 0 0 8 0 0 7 0 0 
 *  0 0 0 0 0 0 0 0 0 
 *  0 0 0 0 0 0 9 0 0 
 *  0 0 1 0 7 0 0 0 0 
 *  1 0 0 0 2 0 0 0 7 
 *  0 0 0 0 3 0 0 0 0 
 *  0 0 0 0 0 0 0 0 0 
 */

#include "sudoku.h"

#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

/*
 * Pre: ---
 * Post: Presenta por pantalla un sangrado de valor TAM_SANGRIA sin
 *       salto de línea.
 */
void sangria () {
    cout << setw(TAM_SANGRIA) << "" << flush;
}

/*
 * Pre: ---
 * Post: Presenta por pantalla un separador horizontal grueso como
 *       +===+===+===+===+===+===+===+===+===+
 *       haciendo uso de SEP_CONVERG y SEP_H_GRUESO, y salta a una nueva
 *       línea.
 */
void separadorGrueso () {
    sangria();
    cout << setw(2) << "" << flush;
    for (int i=0; i<9; ++i) {
        cout << SEP_CONVERG << SEP_H_GRUESO << SEP_H_GRUESO << SEP_H_GRUESO
             << flush;
    }
    cout << SEP_CONVERG << endl;
}

/*
 * Pre: ---
 * Post: Presenta por pantalla un separador horizontal fino como
 *       +---+---+---+---+---+---+---+---+---+
 *       haciendo uso de SEP_CONVERG y SEP_H_FINO, y salta a una nueva
 *       línea.
 */
void separadorFino () {
    sangria();
    cout << setw(2) << "" << flush;
    for (int i=0; i<9; ++i) {
        cout << SEP_CONVERG << SEP_H_FINO << SEP_H_FINO << SEP_H_FINO
             << flush;
    }
    cout << SEP_CONVERG << endl;
}

/*
 * Pre: [fila] y [col] son enteros entre 0 y 8.
 * Post: Devuelve el valor de la celda en la fila [fila] y la columna
 *       [col] del sudoku [s].
 */
int celda (Sudoku s, int fila, int col) {
    return s.celda[fila][col];
}

/*
 * Pre: ---
 * Post: Presenta el sudoku [s] en forma de tabla.
 */
void mostrarSudoku (Sudoku s) {
    sangria();
    cout << setw(1) << "" << flush;
    for (int col=0; col<9; ++col) {
        cout << setw(4) << char('A' + col) << flush;
    }
    cout << endl;
    for (int fila=0; fila<9; ++fila) {
        if (fila % 3 == 0) separadorGrueso();
        else separadorFino();
        sangria();
        cout << char('A' + fila) << flush;
        for (int col=0; col<9; ++col) {
            if (col % 3 == 0) cout << SEP_V_GRUESO << flush;
            else cout << SEP_V_FINO << flush;
            if (celda(s,fila,col) == 0) cout << " " << flush;
            else cout << celda(s,fila,col) << flush;
        }
        cout << SEP_V_GRUESO << endl;
    }
    separadorGrueso();
}

/*
 * Pre: ---
 * Post: Devuelve true solo si ninguna casilla del sudoku [s] está en
 *       blanco.
 */
bool finSudoku (Sudoku s) {
    return s.vacias == 0;
}

/*
 * Pre: [s] es un sudoku válido.
 * Post: Si es lícito anotar el valor [n] en la celda de la fila [fila]
 *       y de la columna [columna] del sudoku [s], según las normas del
 *       juego, lo anota y devuelve true.
 *       Si no es lícito hacerlo, no lo anota y devuelve false.
 */
bool anotacion (Sudoku& s, int fila, int columna, int n) {
    bool legal = true;
    if (n < 1 || n > 9) { // valor fuera de [1,9]
        legal = false;
        cerr << "El valor " << n << " no está comprendido entre 1 y 9." << endl;
    }
    else {
        if (fila < 0 || fila > 8 || columna < 0 || columna > 8) { // dim 9 x 9
            legal = false;
            cerr << "La celda ("
                 << char('A'+fila) << "," << char('A'+columna)
                 << ") no existe." << endl;
        }
        else {
            if (celda(s, fila, columna) != 0) { // valor previo
                legal = false;
                cerr << "Ya existe otro valor en la celda ("
                     << char('A'+fila) << "," << char('A'+columna)
                     << ")." << endl;
            }
            else {
                bool error = false; // para detener iteraciones
                for (int f=0; f<9 && !error; ++f) {
                    if (celda(s, f, columna) == n) { // ya en la columna
                        legal = false; error = true;
                        cerr << "El valor " << n
                             << " ya se encuentra en la columna "
                             << char('A'+columna) << "." << endl;
                    }
                }
                error = false;
                for (int c=0; c<9 && !error; ++c) {
                    if (celda(s, fila, c) == n) { // ya en la fila
                        legal = false; error = true;
                        cerr << "El valor " << n
                             << " ya se encuentra en la fila "
                             << char('A'+fila) << "." << endl;
                    }
                }
                error = false;
                for (int f=fila/3*3; f<fila/3*3+3 && !error; ++f) {
                    for (int c=columna/3*3; c<columna/3*3+3 && !error; ++c) {
                        if (celda(s, f, c) == n) { // ya en el cuadrante
                            legal = false; error = true;
                            cerr << "El valor " << n
                                 << " ya se encuentra en el cuadrante."
                                 << endl;
                        }
                    }
                }
            }
        }
    }
    if (legal) {
        s.celda[fila][columna] = n;
        s.vacias = s.vacias - 1;
    }
    return legal;
}

/*
 * Pre: [nombreFichero] es un fichero de texto con un sudoku válido.
 * Post: Si es posible leer el fichero [nombreFichero], asigna a [s] el
 *       sudoku contenido en él y devuelve true.
 *       Si no es posible hacerlo, devuelve false.
 */
bool leerSudoku(Sudoku& s, char nombreFichero[]) {
    bool exito = false;
    ifstream f;
    f.open(nombreFichero);
    if (!f.fail()) {
        exito = true;
        s.vacias = 0;
        for (int fila=0; fila<9; ++fila) {
            for (int col=0; col<9; ++col) {
                f >> s.celda[fila][col];
                if (s.celda[fila][col] == 0) s.vacias += 1;
            }
        }
    }
    f.close();
    return exito;
}

/*
 * Pre: ---
 * Post: Si es posible escribir en el fichero de texto [nombreFichero]
 *       el sudoku [s], lo hace y devuelve true.
 *       Si no es posible hacerlo, devuelve false.
 */
bool guardarSudoku(Sudoku s, char nombreFichero[]) {
    bool exito = false;
    ofstream f;
    f.open(nombreFichero);
    if (!f.fail()) {
        exito = true;
        for (int fila=0; fila<9; ++fila) {
            for (int col=0; col<9; ++col) {
                f << celda(s,fila,col) << " ";
            }
            f << "\n";
        }
    }
    f.close();
    return exito;
}
