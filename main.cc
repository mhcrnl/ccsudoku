/*
 *   Programa en C++ sin parámetros para seguir la resolución de sudokus
 *   de 81 celdas y 9 cuadrantes por medio de la terminal.
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

#include "sudoku.h"

#include <iostream>
#include <iomanip>

using namespace std;

const char ORDEN_FIN[] = "FIN"; // orden de finalización del programa
const int MAX_LONG_NOMBRE = 80; // longitud máxima en caracteres de la
                                //    ruta y el nombre de los ficheros

/*
 * Pre: ---
 * Post: Devuelve true si, y solo si, [cadena] es igual a la constante
 *       [ORDEN_FIN] sin distinguir entre mayúsculas y minúsculas.
 */
bool comprobarFin (const char cadena[]) {
    bool fin = true;
    for (int i=0; i<3 && fin != false; ++i) {
        if (toupper(cadena[i]) != toupper(ORDEN_FIN[i])) fin = false;
    }
    return fin;
}

/*
 * Pre: ---
 * Post: Pide reiteradamente al operador que especifique un fichero de
 *       texto válido hasta que esto sucede, lee el sudoku contenido en
 *       dicho fichero y lo asigna a [s].
 */
void iteracionCargar (Sudoku& s) {
    char nombreFichero[MAX_LONG_NOMBRE];
    bool exito = false;
    while (!exito) {
        cout << "Fichero con el sudoku a resolver: " << flush;
        nombreFichero[0] = '\0';
        cin >> nombreFichero;
        exito = leerSudoku(s, nombreFichero);
        if (exito) {
            cout << "Fichero cargado correctamente de " << nombreFichero
                 << "." << endl << endl;
        }
        else {
            cerr << "Error, el sudoku no ha podido cargarse." << endl;
            cerr << "Inténtelo de nuevo con otros datos." << endl << endl;
        }
    }
}

/*
 * Pre: ---
 * Post: Pide reiteradamente al operador que especifique un fichero de
 *       texto válido hasta que esto sucede y guarda el sudoku [s] en
 *       dicho fichero.
 */
void iteracionGuardar (Sudoku s) {
    char nombreFichero[MAX_LONG_NOMBRE];
    bool exito = false;
    while (!exito) {
        cout << "Fichero en el que guardar este sudoku: " << flush;
        nombreFichero[0] = '\0';
        cin >> nombreFichero;
        exito = guardarSudoku(s, nombreFichero);
        if (exito) {
            cout << "El sudoku ha sido guardado correctamente en "
                 << nombreFichero << "." << endl;
        }
        else {
            cerr << "Error, no se pudo guardar el sudoku." << endl;
            cerr << "Inténtelo de nuevo con otros datos." << endl << endl;
        }
    }
}

int main () {
    Sudoku s; iteracionCargar(s);
    mostrarSudoku(s);
    bool fin = false;
    while (!fin) {
        cout << endl << endl;
        if (finSudoku(s)) {
            fin = true;
            cout << "El sudoku ha sido resuelto con éxito." << endl;
        }
        else {
            cout << "orden? " << flush;
            char fila[4]; fila[0] = '\0';
            cin >> fila;
            fin = comprobarFin(fila);
            if (!fin) {
                int n;
                char col[4]; col[0] = '\0';
                cin >> col >> n;
                if (fila[0] > 'Z' && col[0] > 'Z') {
                    anotacion(s, fila[0]-'a', col[0]-'a', n);
                }
                else if (fila[0] > 'Z' && col[0] <= 'Z') {
                    anotacion(s, fila[0]-'a', col[0]-'A', n);
                }
                else if (fila[0] <= 'Z' && col[0] > 'Z') {
                    anotacion(s, fila[0]-'A', col[0]-'a', n);
                }
                else {
                    anotacion(s, fila[0]-'A', col[0]-'A', n);
                }
                cout << endl;
                mostrarSudoku(s);
            }
        }
    }
    iteracionGuardar(s);
    return 0;
}
