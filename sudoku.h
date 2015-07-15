/*
 *   Fichero de interfaz sudoku.h para el trabajo con sudokus de
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

#ifndef SUDOKU_H
#define SUDOKU_H

/*
 * Constante de sangrado del sudoku medido en número de caracteres de
 * desplazamiento hacia la derecha
 */
const int TAM_SANGRIA = 8;

/* Constantes de caracteres separadores de filas y de columnas */
const char SEP_V_GRUESO[4] = " | "; // vertical grueso de cuadrantes
const char SEP_V_FINO[4] = " + "; // vertical fino de columnas
const char SEP_H_GRUESO = '='; // horizontal grueso de cuadrantes
const char SEP_H_FINO = '-'; // horizontal fino de filas
const char SEP_CONVERG = '+'; // convergencia entre separadores

/*
 * Definición del tipo de dato Sudoku, que representa la información de
 * un sudoku de 81 celdas y 9 cuadrantes
 */
struct Sudoku {
    /*
     * Cada celda de la tabla [celda] representa una casilla del sudoku
     * en su misma situación, con igual distribución de filas y columnas.
     * Todos los valores de las celdas de [celda] representan los mismos
     * valores en el sudoku, a excepción del 0, que representa una celda
     * vacía.
     */
    int celda[9][9];
    /*
     * [vacias] indica el total de celdas en blanco del sudoku, es decir,
     * el número de ceros presentes en la tabla [celda].
     */
    int vacias;
};

/*
 * Pre: ---
 * Post: Presenta el sudoku [s] en forma de tabla.
 */
void mostrarSudoku (Sudoku s);

/*
 * Pre: ---
 * Post: Devuelve true solo si ninguna casilla del sudoku [s] está en
 *       blanco.
 */
bool finSudoku (Sudoku s);

/*
 * Pre: [s] es un sudoku válido.
 * Post: Si es lícito anotar el valor [n] en la celda de la fila [fila]
 *       y de la columna [columna] del sudoku [s], según las normas del
 *       juego, lo anota y devuelve true.
 *       Si no es lícito hacerlo, no lo anota y devuelve false.
 */
bool anotacion (Sudoku& s, int fila, int columna, int n);

/*
 * Pre: [nombreFichero] es un fichero de texto con un sudoku válido.
 * Post: Si es posible leer el fichero [nombreFichero], asigna a [s] el
 *       sudoku contenido en él y devuelve true.
 *       Si no es posible hacerlo, devuelve false.
 */
bool leerSudoku(Sudoku& s, char nombreFichero[]);

/*
 * Pre: ---
 * Post: Si es posible escribir en el fichero de texto [nombreFichero]
 *       el sudoku [s], lo hace y devuelve true.
 *       Si no es posible hacerlo, devuelve false.
 */
bool guardarSudoku(Sudoku s, char nombreFichero[]);

#endif
