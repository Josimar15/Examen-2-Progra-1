#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <windows.h>

using namespace std;

HANDLE hConsoleHandle = GetStdHandle (STD_OUTPUT_HANDLE);

struct Candidato {
    int id;
    string nombre;
    string partido;
    string plataforma;
    int votos;
};

struct Votante {
    string nombre;
    bool haVotado;
};

vector<Candidato> candidatos;
vector<Votante> votantes;

void menu();
void gotoxy(int x, int y);
void cuadro(int x1, int y1, int x2, int y2);
void ingresarCandidatos();
void votar();
void resultados();
void mostrarMensaje(string mensaje);
bool candidatoRegistrado(int id);
bool votanteRegistrado(string nombre);
bool yaVoto(string nombre);

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void cuadro(int x1, int y1, int x2, int y2) {
    int i;
    for (i = x1; i <= x2; i++) {
        gotoxy(i, y1); printf("%c", 205);
        gotoxy(i, y2); printf("%c", 205);
    }
    for (i = y1; i <= y2; i++) {
        gotoxy(x1, i); printf("%c", 186);
        gotoxy(x2, i); printf("%c", 186);
    }
    gotoxy(x1, y1); printf("%c", 201);
    gotoxy(x1, y2); printf("%c", 200);
    gotoxy(x2, y1); printf("%c", 187);
    gotoxy(x2, y2); printf("%c", 188);
}

void clearLine(int x, int y, int width) {
    gotoxy(x, y);
    for (int i = 0; i < width; i++) {
        cout << ' ';
    }
    gotoxy(x, y);
}

bool idCandidatoUnico(int id) {
    for (size_t i = 0; i < candidatos.size(); ++i) {
        if (candidatos[i].id == id) {
            return false;
        }
    }
    return true;
}

void ingresarCandidatos() {
    system("cls");
    SetConsoleTextAttribute(hConsoleHandle, 10);
    cuadro(10, 7, 100, 29);
    cuadro(18, 3, 53, 5);
    gotoxy(22, 4); cout << "*** INGRESAR CANDIDATOS ***";

    int id;
    string nombre, partido, plataforma;
    char continuar;

    do {
        SetConsoleTextAttribute(hConsoleHandle, 15);
        do {
            gotoxy(11, 9); cout << "Ingrese ID del candidato: ";
            cin >> id;
            cin.ignore();
            if (!idCandidatoUnico(id)) {
                gotoxy(11, 10); cout << "ID ya existe. Intente nuevamente.";
            }
        } while (!idCandidatoUnico(id));

        clearLine(11, 11, 80);
        gotoxy(11, 11); cout << "Ingrese nombre del candidato: ";
        getline(cin, nombre);

        clearLine(11, 13, 80);
        gotoxy(11, 13); cout << "Ingrese partido del candidato: ";
        getline(cin, partido);

        clearLine(11, 15, 80);
        gotoxy(11, 15); cout << "Ingrese plataforma del candidato: ";
        getline(cin, plataforma);

        Candidato candidato = { id, nombre, partido, plataforma, 0 };
        candidatos.push_back(candidato);

        SetConsoleTextAttribute(hConsoleHandle, 2);
        gotoxy(11, 18); cout << "Candidato registrado exitosamente.";

        gotoxy(11, 20); cout << "Desea ingresar otro candidato? (S/N): ";
        continuar = getch();

        clearLine(11, 9, 80);
        clearLine(11, 11, 80);
        clearLine(11, 13, 80);
        clearLine(11, 15, 80);
        clearLine(11, 18, 80);
        clearLine(11, 20, 80);
    } while (toupper(continuar) == 'S');
}


void votar() {
    system("cls");
    SetConsoleTextAttribute(hConsoleHandle, 10);
    cuadro(10, 7, 100, 29);
    cuadro(18, 3, 57, 5);
    gotoxy(19, 4); cout << "*** VOTACIONES ***";

    string nombreVotante;
    int idCandidato;

    gotoxy(11, 9); cout << "Ingrese su nombre: ";
    getline(cin, nombreVotante);

    if (yaVoto(nombreVotante)) {
        gotoxy(11, 11); cout << "Ya ha votado anteriormente.";
        _getch();
        return;
    }

    gotoxy(11, 13); cout << "Ingrese el ID del candidato por el que desea votar: ";
    cin >> idCandidato;
    cin.ignore();

    if (!candidatoRegistrado(idCandidato)) {
        gotoxy(11, 15); cout << "Candidato no registrado.";
        _getch();
        return;
    }
    
    bool votanteEncontrado = false;
    for (vector<Votante>::iterator it = votantes.begin(); it != votantes.end(); ++it) {
        if (it->nombre == nombreVotante) {
            it->haVotado = true;
            votanteEncontrado = true;
            break;
        }
    }
    if (!votanteEncontrado) {
        votantes.push_back(Votante());
        votantes.back().nombre = nombreVotante;
        votantes.back().haVotado = true;
    }

    for (vector<Candidato>::iterator it = candidatos.begin(); it != candidatos.end(); ++it) {
        if (it->id == idCandidato) {
            it->votos++;
            break;
        }
    }

    SetConsoleTextAttribute(hConsoleHandle, 2);
    gotoxy(11, 17); cout << "Voto registrado exitosamente.";
    _getch();
    system("cls");
}

void resultados() {
    system("cls");
    SetConsoleTextAttribute(hConsoleHandle, 10);
    cuadro(10, 7, 100, 29);
    cuadro(18, 3, 57, 5);
    gotoxy(19, 4); cout << "*** RESULTADOS ***";

    if (candidatos.empty()) {
        gotoxy(11, 9); cout << "No hay candidatos registrados.";
        _getch();
        return;
    }

    int totalVotos = 0;
    for (size_t i = 0; i < candidatos.size(); ++i) {
        totalVotos += candidatos[i].votos;
    }

    int maxVotos = -1;
    string ganador;

    for (size_t i = 0; i < candidatos.size(); ++i) {
        if (candidatos[i].votos > maxVotos) {
            maxVotos = candidatos[i].votos;
            ganador = candidatos[i].nombre;
        }
    }

    for (size_t i = 0; i < candidatos.size(); ++i) {
        gotoxy(11, 9 + i * 2);
        cout << "ID: " << candidatos[i].id << " Nombre: " << candidatos[i].nombre 
             << " Votos: " << candidatos[i].votos 
             << " (" << fixed << setprecision(2) << (static_cast<double>(candidatos[i].votos) / totalVotos * 100) << "%)";
        gotoxy(11, 10 + i * 2);
        cout << "------------------------------------";
    }

    gotoxy(11, 9 + candidatos.size() * 2 + 2);
    cout << "Ganador: " << ganador;

    _getch();
}


bool candidatoRegistrado(int id) {
    for (size_t i = 0; i < candidatos.size(); ++i) {
        if (candidatos[i].id == id) {
            return true;
        }
    }
    return false;
}

bool votanteRegistrado(string nombre) {
    for (size_t i = 0; i < votantes.size(); ++i) {
        if (votantes[i].nombre == nombre) {
            return true;
        }
    }
    return false;
}

bool yaVoto(string nombre) {
    for (size_t i = 0; i < votantes.size(); ++i) {
        if (votantes[i].nombre == nombre && votantes[i].haVotado) {
            return true;
        }
    }
    return false;
}


void menu() {
    char opc, conti;

    do {
        system("cls");
        cuadro(16, 2, 66, 6);
        cuadro(8, 8, 72, 24);
        cuadro(14, 21, 34, 23);

        SetConsoleTextAttribute(hConsoleHandle, 10);
        gotoxy(22, 4); printf("***** SISTEMA DE VOTACIONES 2024 *****");
        SetConsoleTextAttribute(hConsoleHandle, 10);
        gotoxy(27, 9); printf("-----MENU PRINCIPAL-----");
        SetConsoleTextAttribute(hConsoleHandle, 15);
        gotoxy(15, 12); printf("[1]  INGRESAR CANDIDATOS");
        gotoxy(15, 14); printf("[2]  VOTAR");
        gotoxy(15, 16); printf("[3]  RESULTADOS");
        gotoxy(15, 18); printf("[4]  SALIR");

        SetConsoleTextAttribute(hConsoleHandle, 10);
        gotoxy(15, 22); printf("* Su opcion es: ");
        opc = getch();

        switch (opc) {
            case '1': ingresarCandidatos(); break;
            case '2': votar(); break;
            case '3': resultados(); break;
            case '4':
                system("cls");
                gotoxy(20, 10); printf("Estas seguro que desea salir del sistema?");
                SetConsoleTextAttribute(hConsoleHandle, 2);
                gotoxy(20, 15); printf("Desea Continuar S/N");
                conti = getch();
                if (conti == 'N' || conti == 'n') {
                    menu();
                } else if (conti == 'S' || conti == 's') {
                    return;
                }
                break;
        }
    
    } while (opc != '4');
}

int main() {
    menu();
    return 0;
}


