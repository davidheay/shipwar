#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <limits>

using namespace std;
int filas;
int columnas;
map<int, map<string, string>> informa = {
    {1, {{}}},
    {2, {{}}},
};
string jugador1;
string jugador2;
void linea(int fila, int num, string primero, string segundo, string **tablero)
{
    for (int j = 0; j < columnas * 2 + 1; j++)
    {
        if (num == 0)
        {
            cout << primero;
        }
        else
        {
            if (primero == "|")
            {
                int pos = floor(j / 2);
                cout << " "
                     << tablero[fila][pos].substr(0, tablero[fila][pos].find("-"))
                     << " ";
            }
            else
            {
                cout << segundo;
            }

            num = -1;
        }
        num++;
    }
    if (primero == "|")
        cout << fila;
    cout << "\n";
}
bool validarEspacio(string **tablero, int x, int y)
{
    bool ocupado = false;
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (tablero[i][j] != " ")
            {
                ocupado = true;
            }
        }
    }
    return ocupado;
}
void generarBarco(string **tablero)
{
    bool inserto = false;
    srand(time(0));
    do
    {
        int x = rand() % (columnas - 2) + 1;
        int y = rand() % (filas - 2) + 1;
        if (!validarEspacio(tablero, x, y))
        {
            for (int i = y - 1; i <= y + 1; i++)
            {
                for (int j = x - 1; j <= x + 1; j++)
                {
                    tablero[i][j] = "x-b";
                }
            }
            tablero[y][x] = "o-b";
            inserto = true;
        }
    } while (!inserto);
}
void generarBarcoImpactado(string **tablero, int y, int x)
{
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            tablero[i][j] = "*";
        }
    }
    tablero[y][x] = "*";
}
void generarAvion(string **tablero)
{
    bool inserto = false;
    srand(time(0));
    do
    {
        int x = rand() % (columnas - 2) + 1;
        int y = rand() % (filas - 2) + 1;
        if (!validarEspacio(tablero, x, y))
        {
            for (int i = y - 1; i <= y + 1; i++)
            {
                for (int j = x - 1; j <= x + 1; j++)
                {
                    if (!(abs(i - y) == 1 && abs(x - j) == 1))
                    {
                        tablero[i][j] = "x-a";
                    }
                }
            }
            tablero[y][x] = "o-a";
            inserto = true;
        }
    } while (!inserto);
}
void generarAvionImpactado(string **tablero, int y, int x)
{
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (!(abs(i - y) == 1 && abs(x - j) == 1))
            {
                tablero[i][j] = "*";
            }
        }
    }
    tablero[y][x] = "*";
}
void pintarTablero(string **tablero)
{
    int num = 0;
    for (int j = 0; j < columnas; j++)
        cout << "  " << j << ((j < 10) ? " " : "");
    cout << endl;
    for (int i = 0; i < filas; i++)
    {
        if (num == 0)
        {
            linea(i, num, " ", "---", tablero);
        }
        num = 0;
        linea(i, num, "|", "   ", tablero);
        num = 0;
        linea(i, num, " ", "---", tablero);
        num = 1;
    }
}
int solicitarNumero(int var, string nombre, int min, int max)
{
    do
    {
        cout << "Por favor ingrese el numero " + nombre + ": ", cin >> var;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Por favor ingrese un numero: ", cin >> var;
            cin.ignore();
        }
        if (var < min || var >= max)
            cout << "El numero deber ser mayor o igual a " << min << " y menor a " << max << endl;
    } while (var < min || var >= max);
    return var;
}
string **crearMatriz()
{
    string **tablero;
    tablero = new string *[filas];
    for (int i = 0; i < filas; i++)
    {
        tablero[i] = new string[columnas];
        for (int j = 0; j < columnas; j++)
            tablero[i][j] = " ";
    }
    return tablero;
}
string **crearTablero()
{
    string **tablero = crearMatriz();
    for (int i = 0; i < 3; i++)
    {
        generarBarco(tablero);
        generarAvion(tablero);
    }
    return tablero;
}
string getTipo(string cadena)
{
    return cadena.substr(cadena.find("-") + 1, 2);
}
bool esCentro(string cadena)
{
    return cadena.substr(0, cadena.find("-")) == "o";
}
void limpiar()
{
    system("cls");
    cout << "================================ASTUCIA NAVAL================================\n\n";
}
void juega(string **tablero, string **jugadas, int jugador)
{
    int filaJug = 0;
    filaJug = solicitarNumero(filaJug, "la fila", 0, filas);
    int colJug = 0;
    colJug = solicitarNumero(colJug, "la columna", 0, columnas);

    string temp = tablero[filaJug][colJug];
    if (temp != " " && temp != "+")
    {
        jugadas[filaJug][colJug] = "*";
        cout << "!Impactooo! Has golpeado a un ";
        if (getTipo(temp) == "b")
        {
            cout << "Barco\n";
            if (esCentro(temp))
            {
                cout << "En el centro! Has ganado 225 puntos\n";
                generarBarcoImpactado(jugadas, filaJug, colJug);
                int temp = stoi(informa[jugador]["PUN"]);
                temp += 225;
                informa[jugador]["PUN"] = to_string(temp);

                temp = stoi(informa[jugador]["HUN"]);
                temp++;
                informa[jugador]["HUN"] = to_string(temp);
            }
            else
            {
                cout << "Has ganado 25 puntos\n";
                int puntaje = stoi(informa[jugador]["PUN"]);
                puntaje += 25;
                informa[jugador]["PUN"] = to_string(puntaje);
            }
        }
        else
        {
            cout << "Avion\n";
            if (esCentro(temp))
            {
                generarAvionImpactado(jugadas, filaJug, colJug);
                cout << "En el centro! Has ganado 300 puntos\n";
                int temp = stoi(informa[jugador]["PUN"]);
                temp += 300;
                informa[jugador]["PUN"] = to_string(temp);

                temp = stoi(informa[jugador]["HUN"]);
                temp++;
                informa[jugador]["HUN"] = to_string(temp);
            }
            else
            {
                cout << "Has ganado 60 puntos\n";
                int puntaje = stoi(informa[jugador]["PUN"]);
                puntaje += 60;
                informa[jugador]["PUN"] = to_string(puntaje);
            }
        }
    }
    else
    {
        cout << "Has fallado\n";
        jugadas[filaJug][colJug] = "=";
    }
    pintarTablero(jugadas);
    cout << "Puntaje:" + informa[jugador]["PUN"] + "\n";
}
int main()
{
    limpiar();
    cout << "Por favor ingrese el nombre del jugador 1: ", cin >> informa[1]["NOM"];
    cout << "Por favor ingrese el nombre del jugador 2: ", cin >> informa[2]["NOM"];

    limpiar();
    cout << "Bienvenidos " + informa[1]["NOM"] + " y " + informa[2]["NOM"] << endl;

    filas = solicitarNumero(filas, "de filas", 10, 50);
    columnas = solicitarNumero(columnas, "de columnas", 10, 50);
    limpiar();
    cout << "Seleccionaste un tablero con " << filas << " filas y " << columnas << " columnas\n";

    cout << "A continuacion se mostrar el tablero para el jugador " + informa[1]["NOM"] + "\n\n";
    system("pause");
    string **tablero1 = crearTablero();
    string **jugadas1 = crearMatriz();
    pintarTablero(tablero1);
    system("pause");
    limpiar();

    cout << "A continuacion se mostrar el tablero para el jugador " + informa[2]["NOM"] + "\n\n";
    system("pause");
    string **tablero2 = crearTablero();
    string **jugadas2 = crearMatriz();
    pintarTablero(tablero2);
    system("pause");
    limpiar();

    informa[1]["PUN"] = "0";
    informa[2]["PUN"] = "0";
    informa[1]["HUN"] = "0";
    informa[2]["HUN"] = "0";

    cout << "Que inicie el juego!!\n";
    bool activo = true;
    do
    {
        if (activo)
        {
            cout << "Turno de " + informa[1]["NOM"] + "\n";
            pintarTablero(jugadas1);
            juega(tablero2, jugadas1, 1);
            if (stoi(informa[1]["HUN"]) == 3)
            {
                cout << "Has hundido 3 elementos " + informa[1]["NOM"] + " el juego se termina!\n";
                activo = false;
            }
            system("pause");
            limpiar();
        }
        if (activo)
        {
            cout << "Turno de " + informa[2]["NOM"] + "\n";
            pintarTablero(jugadas2);
            juega(tablero1, jugadas2, 2);
            if (stoi(informa[2]["HUN"]) == 3)
            {
                cout << "Has hundido 3 elementos " + informa[2]["NOM"] + " el juego se termina!\n";
                activo = false;
            }
            system("pause");
            limpiar();
        }
    } while (activo);

    cout << "Juego Terminado \n";

    cout << "Tablero del jugador " + informa[1]["NOM"] + "\n";
    pintarTablero(tablero1);

    cout << "Tablero del jugador " + informa[2]["NOM"] + "\n";
    pintarTablero(tablero2);

    cout << "Puntajes:\n" + informa[1]["NOM"] + ":" + informa[1]["PUN"] + "\n" + informa[2]["NOM"] + ":" + informa[2]["PUN"] + "\n";

    if (stoi(informa[1]["PUN"]) > stoi(informa[2]["PUN"]))
        cout << "Ganaste " + informa[1]["NOM"] + "!\n";
    else
        cout << "Ganaste " + informa[2]["NOM"] + "!\n";

    system("pause");
    return 0;
}
