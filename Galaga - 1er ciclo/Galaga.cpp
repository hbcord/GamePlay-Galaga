#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace System;

#define MAX_BALAS 5
#define MAX_TANDA 3
#define M_ALTO 40
#define M_ANCHO 40
#define T_DER 77
#define T_IZQ 75
#define T_ARR 72
#define T_ABA 80
#define T_ESP 32
#define T_ENT 13

char tecla = 0;
int puntuacion = 0;
enum T_enm { mariposa, avispa, comandante };
enum Tanda { tanda1, tanda2, tanda3 };
enum direccion { arriba, abajo, izquierda, derecha };
enum nivel { facil, normal, dificil };
int n = 0;
bool endGame00 = false;
bool n1 = true, n2 = false, n3 = false;
bool fin = false;

typedef struct {
	int pos_xa; int pos_xn; int pos_ya; int pos_yn;
	bool est;
}balas_navd;

balas_navd* b_navG = new balas_navd[MAX_BALAS];

class nav_def {
private:
	int x_ant, x_nue, y_ant, y_nue;
	int vidas;
	bool estado;
	int balas_disp;
	balas_navd b_nav[MAX_BALAS];
public:
	nav_def(int, int, int, int, int, bool, int);
	~nav_def() {}
	int getter_vida();
	void set_vida(int n) { vidas = vidas - n; };
	int get_x() { return x_nue; }
	int get_y() { return y_nue; }
	int get_balas_disp() { return balas_disp; }
	void mover_inicializar_bala();
	void mover_balas();
	void mostrar_balas_vidas();
	void get_b_navdef() {
		for (int i = 0; i < balas_disp; i++) {
			b_navG[i] = b_nav[i];
		}
	}
};
class nav_enm {
private:
	int x_ant, x_nue, y_ant, y_nue;
	T_enm tipo;
	Tanda tand;
	direccion dir;
	int indice;
	int orden;
	int hilera;
	bool estado;
public:
	nav_enm() {}
	~nav_enm() {  }
	void inicializar_enm(int, int, int, int, Tanda, T_enm, direccion, int, int, int, bool);
	void mover_nave();
	void borrar_nave();
	void pintar_nave();
	bool get_estado() { return estado; }
	void set_estado(bool _est) { estado = _est; }
	int get_xn() { return x_nue; };
	int get_yn() { return y_nue; };
	T_enm get_tipo() { return tipo; }
};

void val_pred();
void iniciando_juego(nav_def& nave);
void endGame_Galaga(bool&, nav_def& nave, int&);
void temporizador(int& tiemRel, int& tiemSeg, int& tiemMin, bool& endGame, nav_def&);
void presentacion_parte1(char& tecla);
void presentacion_parte2(char& tecla, int& pag);
void pantalla_de_instrucciones(char& tecla, int& pag);
T_enm enm_aleatorio(int);
void cant_enm(nivel, int&, int&, int&);
void inicializar_disparo_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&, int&);
void mover_balas_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&, int&, balas_navd*&);
void actualizar_disparo_enm(int&, int&, int&, int&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, balas_navd*&, int&);
void perder_vida(balas_navd*&, nav_def&, int, int&);
void eliminar_enm(nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, int, int, int, nav_def&);
bool enm_eliminados(nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, nav_enm*&, int&, int&, int&);

nav_def::nav_def(int _xa, int _xn, int _ya, int _yn, int _vid, bool _est, int _bdis) {
	x_ant = _xa; x_nue = _xn;
	y_ant = _ya; y_nue = _yn;
	vidas = _vid;
	estado = _est;
	balas_disp = _bdis;
}
int nav_def::getter_vida() {
	return vidas;
}
void nav_def::mover_inicializar_bala() {
	if (_kbhit()) {
		Console::SetCursorPosition(x_ant, y_ant); cout << "   ";
		tecla = getch();
		if (tecla == T_IZQ && x_nue > 0) x_nue--;
		else if (tecla == T_DER && x_nue < 37) x_nue++;
		else if (tecla == T_ESP && balas_disp < MAX_BALAS) {

			balas_disp++;
			b_nav[balas_disp].pos_xa = b_nav[balas_disp].pos_xn = x_nue;
			b_nav[balas_disp].pos_yn = b_nav[balas_disp].pos_ya = y_nue;
		}
		else if (fin == true && tecla == T_ENT) {
			endGame00 = true;
			tecla = 0;
			system("cls");
			system("cls"); fflush(stdin);
		}
		Console::SetCursorPosition(x_nue, y_nue); cout << "|w|";

		x_ant = x_nue;
		y_ant = y_nue;
	}
}
void nav_def::mover_balas() {
	if (balas_disp >= 0) {
		for (int i = 0; i <= balas_disp; i++) {
			b_nav[i].pos_yn--;
			if (b_nav[i].pos_yn > 4) {
				Console::SetCursorPosition(b_nav[i].pos_xa + 1, b_nav[i].pos_ya - 1); cout << " ";
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(b_nav[i].pos_xn + 1, b_nav[i].pos_yn - 1); cout << char(4);
				b_nav[i].pos_xa = b_nav[i].pos_xn;
				b_nav[i].pos_ya = b_nav[i].pos_yn;
			}
			else if (b_nav[i].pos_yn == 4) {
				Console::SetCursorPosition(b_nav[i].pos_xa + 1, b_nav[i].pos_ya - 1); cout << " ";
			}
			else if (b_nav[i].pos_yn < 4) {
				b_nav[i] = b_nav[balas_disp];
				balas_disp--;
			}
		}
	}
}
void nav_def::mostrar_balas_vidas() {
	Console::ForegroundColor = ConsoleColor::DarkYellow;
	Console::SetCursorPosition((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x" << vidas;
	Console::SetCursorPosition((M_ANCHO / 2) - 5, 2); cout << puntuacion;
	for (int i = 1; i <= MAX_BALAS - (balas_disp + 1); i++) {
		Console::ForegroundColor = ConsoleColor::DarkGreen;
		Console::SetCursorPosition(((M_ANCHO / 2) + 1) + i, M_ALTO - 1); cout << char(4);
	}
	for (int u = 0; u <= balas_disp; u++) {
		if (balas_disp < MAX_BALAS) {
			Console::ForegroundColor = ConsoleColor::DarkRed;
			Console::SetCursorPosition(((M_ANCHO / 2) + 1) + MAX_BALAS - balas_disp, M_ALTO - 1); cout << char(4);
		}
	}
}

void nav_enm::inicializar_enm(int xa, int xn, int ya, int yn, Tanda ta, T_enm ti, direccion _dir, int indic, int ord, int hil, bool est) {
	x_ant = xa;
	x_nue = xa;
	y_ant = ya;
	y_nue = yn;
	tand = ta;
	tipo = ti;
	dir = _dir;
	indice = indic;
	orden = ord;
	hilera = hil;
	estado = est;
}
void nav_enm::mover_nave() {
	indice++;
	borrar_nave();
	if (tand == tanda1) {
		if (hilera == 1) {
			if (indice >= 0 && indice <= 2) {
				y_nue += 2;
			}
			else if (indice > 2 && indice < 4) {
				y_nue += 2;
			}
			else if (indice >= 4 && indice < 10) {
				y_nue += 2;
			}
			else if (indice >= 10 && indice < 12) {
				y_nue += 2;
				x_nue--;
			}
			else if (indice >= 12 && indice < 16) {
				x_nue--;
			}
			else if (indice >= 16 && indice < 18) {
				y_nue -= 2;
				x_nue--;
			}
			else if (indice >= 18 && indice < 21) {
				y_nue -= 2;
			}
			else if (indice >= 21 && indice < 23) {
				y_nue -= 2;
			}
			else if (indice >= 23 && indice < 30) {
				x_nue = (x_nue + 3) - orden;
			}
		}
		else if (hilera == 2) {
			if (indice >= 0 && indice <= 2) {
				y_nue += 2;
			}
			else if (indice > 2 && indice < 4) {
				y_nue += 2;
			}

			else if (indice >= 4 && indice < 10) {
				y_nue += 2;
			}
			else if (indice >= 10 && indice < 12) {
				y_nue += 2;
				x_nue++;
			}
			else if (indice >= 12 && indice < 16) {
				x_nue++;
			}
			else if (indice >= 16 && indice < 18) {
				y_nue -= 2;
				x_nue++;
			}
			else if (indice >= 18 && indice < 21) {
				y_nue -= 3;
			}
			else if (indice >= 21 && indice < 23) {
				y_nue -= 2;
				x_nue += 2;
			}
			else if (indice >= 23 && indice < 30) {
				x_nue = (x_nue - 3) + orden;
			}
		}
		if (indice >= 40) {

			if (x_nue >= 0 && dir == izquierda) {
				if (x_nue == 0) {
					dir = abajo;
					y_nue += 2;
					x_nue += 1;
				}
				else x_nue--;
			}
			else if (x_nue <= 37 && dir == abajo) {
				if (x_nue == 37) {
					dir = izquierda;
					y_nue -= 2;
					x_nue -= 1;
				}
				else {
					x_nue++;
				}
			}
		}
	}
	else if (tand == tanda2) {
		if (hilera == 1) {
			if (indice >= 40 && indice < 42) {
				y_nue++;
			}
			else if (indice > 42 && indice < 52) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 52 && indice < 61) {
				y_nue++;
			}
			else if (indice >= 61 && indice < 64) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 64 && indice < 70) {
				x_nue--;
			}
			else if (indice > 70 && indice < 73) {
				y_nue--;
				x_nue--;
			}
			else if (indice > 73 && indice < 84) {
				y_nue--;
			}
			else if (indice >= 85 && indice < 90) {
				y_nue--;
				x_nue += 2;
			}
			else if (indice >= 90 && indice <= 92) {
				x_nue = x_nue - orden;
				y_nue = y_nue - orden;
			}
		}
		else if (hilera == 2) {
			if (indice >= 40 && indice < 42) {
				y_nue++;
			}
			else if (indice > 42 && indice < 52) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 52 && indice < 61) {//3
				y_nue++;
			}
			else if (indice >= 61 && indice < 64) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 64 && indice < 70) {
				x_nue++;
			}
			else if (indice > 70 && indice < 73) {
				y_nue--;
				x_nue++;
			}
			else if (indice > 73 && indice < 85) {
				y_nue--;
			}
			else if (indice >= 85 && indice < 90) {
				x_nue--;
				y_nue -= 2;
			}
			else if (indice >= 90 && indice <= 92) {
				x_nue = x_nue - orden;
				y_nue = y_nue + orden;
			}
		}
	}
	else if (tand == tanda3) {
		if (hilera == 1) {
			if (indice >= 98 && indice < 103) {
				x_nue++;
			}
			else if (indice >= 103 && indice < 108) {
				x_nue++;
				y_nue++;
			}
			else if (indice >= 108 && indice < 111) {
				y_nue--;
			}
			else if (indice >= 111 && indice < 116) {
				y_nue--;
				x_nue--;
			}
			else if (indice >= 116 && indice < 119) {
				x_nue++;
			}
			else if (indice >= 119 && indice < 123) {
				y_nue++;
				x_nue--;
			}
			else if (indice >= 123 && indice < 126) {
				y_nue++;
			}
			else if (indice >= 126 && indice < 128) {
				y_nue++;
				x_nue++;
			}
			else if (indice >= 128 && indice < 136) {
				x_nue++;
			}
			else if (indice >= 136 && indice < 140) {
				y_nue--;
				x_nue++;
			}
			else if (indice >= 140 && indice < 143) {
				y_nue--;
			}
			else if (indice >= 143 && indice < 150) {
				y_nue--;
				x_nue--;
			}
			else if (indice >= 150 && indice < 154) {
				y_nue = y_nue - orden;
				x_nue = x_nue - orden;
			}
			else if (indice >= 198 && indice <= 203) {
				x_nue++;
			}
			else if (indice >= 202 && indice <= 208) {
				x_nue--;
			}
			else if (indice >= 209) {
				indice = 198;
			}
		}
		else if (hilera == 2) {
			if (indice >= 175 && indice < 180) {
				x_nue--;
			}
			else if (indice >= 180 && indice < 189) {
				y_nue--;
			}
			else if (indice >= 189 && indice < 190) {
				x_nue--;
				y_nue++;
			}
			else if (indice >= 190 && indice < 194) {
				x_nue--;
			}
			else if (indice >= 194 && indice <= 197) {
				y_nue = y_nue - orden;
				x_nue = x_nue + orden;
			}
			else if (indice >= 198 && indice <= 203) {
				x_nue--;
			}
			else if (indice >= 204 && indice <= 208) {
				x_nue++;
			}
			else if (indice >= 209) {
				indice = 198;
			}

		}
	}
	pintar_nave();
	x_ant = x_nue;
	y_ant = y_nue;
}
void nav_enm::borrar_nave() {
	Console::SetCursorPosition(x_ant, y_ant); cout << "   ";
}
void nav_enm::pintar_nave() {
	if (tipo == mariposa && estado == true) {
		Console::ForegroundColor = ConsoleColor::Blue;
		Console::SetCursorPosition(x_nue, y_nue); cout << ">";
		Console::SetCursorPosition(x_nue + 2, y_nue); cout << "<";
		Console::ForegroundColor = ConsoleColor::Yellow;
		Console::SetCursorPosition(x_nue + 1, y_nue); cout << "|";
	}
	else if (tipo == avispa && estado == true) {
		Console::ForegroundColor = ConsoleColor::Green;
		Console::SetCursorPosition(x_nue, y_nue); cout << char(4);
		Console::SetCursorPosition(x_nue + 2, y_nue); cout << char(4);
		Console::ForegroundColor = ConsoleColor::White;
		Console::SetCursorPosition(x_nue + 1, y_nue); cout << "|";
	}
	else if (tipo == comandante && estado == true) {
		Console::ForegroundColor = ConsoleColor::Red;
		Console::SetCursorPosition(x_nue, y_nue); cout << "}";
		Console::SetCursorPosition(x_nue + 2, y_nue); cout << "{";
		Console::ForegroundColor = ConsoleColor::White;
		Console::SetCursorPosition(x_nue + 1, y_nue); cout << "-";
	}
}


int main() {
	bool endGame = false;
	int tiemSeg = 0;
	int tiemMin = 0;
	int tiemRel = 0;
	int pag = 0;
	bool nivel2 = false, nivel3 = false;
	int n = 0;

	val_pred();
	presentacion_parte1(tecla);
	do {
		presentacion_parte2(tecla, pag);
		if (pag == 1) {
			nav_def nave(M_ANCHO / 2, M_ANCHO / 2, M_ALTO - 2, M_ALTO - 2, 4, true, -1);

			nivel nv = normal;
			int c_T1 = 0, c_T2 = 0, c_T3 = 0, C_balas = 3, r = 0;
			balas_navd* bala_enm = new balas_navd[C_balas];

			cant_enm(nv, c_T1, c_T2, c_T3);

			nav_enm* T1_H1 = new nav_enm[c_T1];
			nav_enm* T1_H2 = new nav_enm[c_T1];
			nav_enm* T2_H1 = new nav_enm[c_T2];
			nav_enm* T2_H2 = new nav_enm[c_T2];
			nav_enm* T3_H1 = new nav_enm[c_T3];
			nav_enm* T3_H2 = new nav_enm[c_T3];

			for (int i = 0; i < c_T1; i++) {
				T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
				T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

			}
			for (int u = 0; u < c_T2; u++) {
				T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
				T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
			}
			for (int j = 0; j < c_T3; j++) {
				T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
				T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
			}
			bool eliminados = false;

			iniciando_juego(nave);
			while (!endGame00) {

				tiemRel++;
				r++;
				nave.mover_inicializar_bala();
				nave.mover_balas();
				nave.mostrar_balas_vidas();

				temporizador(tiemRel, tiemSeg, tiemMin, endGame, nave);

				for (int i = 0; i < c_T1; i++) { //TANDA 1
					T1_H1[i].mover_nave();
					T1_H2[i].mover_nave();
				}
				for (int u = 0; u < c_T2; u++) { //TANDA 2
					T2_H1[u].mover_nave();
					T2_H2[u].mover_nave();
				}
				for (int j = 0; j < c_T3; j++) { //TANDA 3
					T3_H1[j].mover_nave();
					T3_H2[j].mover_nave();
				}
				nave.get_b_navdef();
				eliminar_enm(T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, c_T1, c_T2, c_T3, nave);

				Sleep(30);

				inicializar_disparo_enm(C_balas, c_T1, c_T2, c_T3, T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, bala_enm, r);
				mover_balas_enm(C_balas, c_T1, c_T2, c_T3, T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, bala_enm, r, bala_enm);

				perder_vida(bala_enm, nave, C_balas, r);

				endGame_Galaga(endGame, nave, tiemMin);

				if (enm_eliminados(T1_H1, T1_H2, T2_H1, T2_H2, T3_H1, T3_H2, c_T1, c_T2, c_T3) == true) {
					if (nivel2 == false) {
						nv = normal;
						Console::SetCursorPosition(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "NIVEL 2";
						Sleep(1000);
						for (int i = 0; i < c_T1; i++) {
							T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
							T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

						}
						for (int u = 0; u < c_T2; u++) {
							T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
							T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
						}
						for (int j = 0; j < c_T3; j++) {
							T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
							T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
						}
						nivel2 = true;
						Console::SetCursorPosition(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "       ";
					}
					else if (nivel3 == false) {
						nv = dificil;
						Console::SetCursorPosition(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "NIVEL 3";
						Sleep(1000);
						for (int i = 0; i < c_T1; i++) {
							T1_H1[i].inicializar_enm(18, 18, 6, 6, tanda1, enm_aleatorio(12), abajo, -1 - 5 * i, i, 1, true);
							T1_H2[i].inicializar_enm(22, 22, 6, 6, tanda1, enm_aleatorio(6), izquierda, -1 - 5 * i, i, 2, true);

						}
						for (int u = 0; u < c_T2; u++) {
							T2_H1[u].inicializar_enm(25, 25, 4, 4, tanda2, enm_aleatorio(36), abajo, -1 - 10 * u, u, 1, true);
							T2_H2[u].inicializar_enm(15, 15, 4, 4, tanda2, enm_aleatorio(18), abajo, -1 - 10 * u, u, 2, true);
						}
						for (int j = 0; j < c_T3; j++) {
							T3_H1[j].inicializar_enm(0, 0, 30, 30, tanda3, enm_aleatorio(12), abajo, -1 - 6 * j, j, 1, true);
							T3_H2[j].inicializar_enm(37, 39, 30, 30, tanda3, enm_aleatorio(24), abajo, -1 - 6 * j, j, 2, true);
						}
						nivel3 = true;
						Console::SetCursorPosition(M_ANCHO / 2 - 2, M_ALTO / 2); cout << "       ";
					}
					else if (nivel2 == true && nivel3 == true) {
						Console::ForegroundColor = ConsoleColor::DarkYellow;
						Console::SetCursorPosition(M_ANCHO / 2 - 6, M_ALTO / 2); cout << "Congratulation!!";
						n++;
						if (n % 10 != 0)
							Console::ForegroundColor = ConsoleColor::Cyan;
						else 
							Console::ForegroundColor = ConsoleColor::Red;
						Console::SetCursorPosition(M_ANCHO / 2 - 10, M_ALTO / 2 + 2); cout << "Presione ENTER para salir";
						if (n > 1000)
							n = 0;
						fin = true;
						pag = 0;
					}

				}
			}
			endGame00 = false;
			tiemRel = tiemMin = tiemSeg = 0;
			puntuacion = 0;
		}
		else if (pag == 2) {
			pantalla_de_instrucciones(tecla, pag);
		}
		system("cls");
	} while (endGame == false);


	return 0;
}

void val_pred() {
	Console::WindowHeight = M_ALTO;
	Console::WindowWidth = M_ANCHO;
	Console::CursorVisible = false;
	Console::Title = "GALAGA - EN POO";
}
void iniciando_juego(nav_def& nave) {
	//PINTANDO LAS VIDAS
	Console::ForegroundColor = ConsoleColor::DarkYellow;
	Console::SetCursorPosition((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x" << nave.getter_vida();
	//CAJA DE TIEMPO E IMFORMACION
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(11, 1); cout << "PUNTUACION";
	Console::SetCursorPosition(22, 1); cout << "TIEMPO";
	Console::ForegroundColor = ConsoleColor::DarkYellow;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 19; j++) {
			Console::SetCursorPosition(10 + j, i);
			printf("%c", 196);
			Console::SetCursorPosition(10 + j, 3);
			printf("%c", 196);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 1; j++) {
			Console::SetCursorPosition(10, i);
			printf("%c", 179);
			Console::SetCursorPosition(21, i);
			printf("%c", 179);
			Console::SetCursorPosition(28, i);
			printf("%c", 179);
		}
	}
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(10, 0); printf("%c", 218);
	Console::SetCursorPosition(21, 0); printf("%c", 194);
	Console::SetCursorPosition(28, 0); printf("%c", 191);
	Console::SetCursorPosition(10, 3); printf("%c", 192);
	Console::SetCursorPosition(21, 3); printf("%c", 193);
	Console::SetCursorPosition(28, 3); printf("%c", 217);
	//PINTANDO POSICION INICIAL NAVE
	Console::SetCursorPosition(M_ANCHO / 2, M_ALTO - 2); cout << "|w|";
	//TIEMPO DE INCIO
	Console::SetCursorPosition(17, 20); cout << "LISTO?";
	Sleep(3000);
	Console::SetCursorPosition(17, 20); cout << "       ";
	Console::SetCursorPosition(17, 20); cout << "VAMOS!";
	Sleep(1000);
	Console::SetCursorPosition(17, 20); cout << "       ";
}
void temporizador(int& tiemRel, int& tiemSeg, int& tiemMin, bool& endGame, nav_def& nave) {
	if (tiemRel == 10) {
		tiemSeg++;
		tiemRel = 0;
	}

	if (tiemSeg == 60) {
		Console::SetCursorPosition(26, 2); cout << "  ";
		tiemMin++;
		tiemSeg = 0;
	}
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(23, 2); cout << "0";
	Console::SetCursorPosition(24, 2); cout << tiemMin;
	Console::SetCursorPosition(25, 2); cout << ":";

	if (tiemSeg >= 10) {
		Console::SetCursorPosition(26, 2); cout << tiemSeg;
	}
	else {
		Console::SetCursorPosition(26, 2); cout << "0";
		Console::SetCursorPosition(27, 2); cout << tiemSeg;
	}

}
void endGame_Galaga(bool& endGame, nav_def& nave, int& _tiem) {
	if (nave.getter_vida() == 0 || _tiem == 5) {
		Console::SetCursorPosition((M_ANCHO / 2) - 4, M_ALTO - 1); cout << char(3) << "x0";
		bool ani1 = false;
		
		int r = 0, formEND[7][38] =
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0},
			{0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0},
			{0,0,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0},
			{0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		};
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 38; j++) {
				if (formEND[i][j] == 0) {
					Console::SetCursorPosition(j + 2, i + 16); cout << " ";
				}
				else if (formEND[i][j] == 1) {
					Console::ForegroundColor = ConsoleColor::Red;
					Console::SetCursorPosition(j + 2, i + 16); cout << "*";

				}
				Sleep(5);
			}
		}
		while (!ani1) {
			r++;
			if (r % 10 == 0) {
				Console::ForegroundColor = ConsoleColor::Yellow;
				Console::SetCursorPosition(14, 23); cout << "PRESIONE ENTER";
			}
			else {
				Console::ForegroundColor = ConsoleColor::Red;
				Console::SetCursorPosition(14, 23); cout << "PRESIONE ENTER";
			}
			if (_kbhit()) {
				tecla = getch();
				if (tecla == T_ENT) {
					ani1 = true;
					tecla = 0;
					endGame00 = true;
					system("cls");
				}
			}
		}

	}
}
void pantalla_de_instrucciones(char& tecla, int& pag) {

	int pos = 3;
	Console::Clear();
	Console::ForegroundColor = ConsoleColor::DarkRed;
	Console::SetCursorPosition(7, 2); printf("%c%c%c  %c%c  %c   %c%c  %c%c%c  %c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	Console::SetCursorPosition(7, 3); printf("%c   %c  %c %c  %c  %c %c   %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	Console::SetCursorPosition(7, 4); printf("%c %c %c%c%c%c %c  %c%c%c%c %c %c %c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	Console::SetCursorPosition(7, 5); printf("%c%c%c %c  %c %c%c %c  %c %c%c%c %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::SetCursorPosition(3, 7); cout << "Es un juego matamarcianos clasico, \n   donde el objetivo es eliminar las \n   distintas naves enemigas.";
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(12, 12); printf("* ** *  * ** *");
	Console::SetCursorPosition(12, 13); printf("* <- *  * -> *");
	Console::SetCursorPosition(12, 14); printf("* ** *  * ** *");
	Console::ForegroundColor = ConsoleColor::DarkYellow;
	Console::SetCursorPosition(5, 16); printf("Para desplazarnos y esquivar");
	Console::SetCursorPosition(5, 17); printf(" las balas enemigas, debemos ");
	Console::SetCursorPosition(5, 18); printf("usar las teclas direccionales");
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(3, 21); printf("* * * * * * * * * * * * * * * * * *");
	Console::SetCursorPosition(3, 22); printf("*             esp                 *");
	Console::SetCursorPosition(3, 23); printf("* * * * * * * * * * * * * * * * * *");
	Console::ForegroundColor = ConsoleColor::DarkYellow;
	Console::SetCursorPosition(0, 25); printf("    Para disparas usaremos la tecla");
	Console::SetCursorPosition(0, 26); printf("  espacio, recuerde que son 3 disparos");
	Console::SetCursorPosition(0, 27); printf("      que podemos hacer a la vez");
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::SetCursorPosition(3, 32); printf("    *");
	Console::SetCursorPosition(3, 33); printf("  * * * *");
	Console::SetCursorPosition(3, 34); printf("* * * * *");
	Console::SetCursorPosition(3, 35); printf("  * * * *");
	Console::SetCursorPosition(3, 36); printf("    *");
	Console::SetCursorPosition(5, 38); cout << "volver";
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(27, 32); printf("    *");
	Console::SetCursorPosition(27, 33); printf("* * * *  ");
	Console::SetCursorPosition(27, 34); printf("* * * * *");
	Console::SetCursorPosition(27, 35); printf("* * * *  ");
	Console::SetCursorPosition(27, 36); printf("    *");
	while (1)
	{

		if (_kbhit()) {
			tecla = _getch();
			if (tecla == T_IZQ) {
				pos = 3;
				Console::ForegroundColor = ConsoleColor::Blue;
				Console::SetCursorPosition(5, 38); cout << "volver";
				Console::SetCursorPosition(29, 38); cout << "     ";
				Console::SetCursorPosition(3, 32); printf("    *");
				Console::SetCursorPosition(3, 33); printf("  * * * *");
				Console::SetCursorPosition(3, 34); printf("* * * * *");
				Console::SetCursorPosition(3, 35); printf("  * * * *");
				Console::SetCursorPosition(3, 36); printf("    *");
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(27, 32); printf("    *");
				Console::SetCursorPosition(27, 33); printf("* * * *  ");
				Console::SetCursorPosition(27, 34); printf("* * * * *");
				Console::SetCursorPosition(27, 35); printf("* * * *  ");
				Console::SetCursorPosition(27, 36); printf("    *");
			}
			else if (tecla == T_DER) {
				pos = 27;
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(3, 32); printf("    *");
				Console::SetCursorPosition(3, 33); printf("  * * * *");
				Console::SetCursorPosition(3, 34); printf("* * * * *");
				Console::SetCursorPosition(3, 35); printf("  * * * *");
				Console::SetCursorPosition(3, 36); printf("    *");
				Console::ForegroundColor = ConsoleColor::Blue;
				Console::SetCursorPosition(27, 32); printf("    *");
				Console::SetCursorPosition(27, 33); printf("* * * *  ");
				Console::SetCursorPosition(27, 34); printf("* * * * *");
				Console::SetCursorPosition(27, 35); printf("* * * *  ");
				Console::SetCursorPosition(27, 36); printf("    *");
				Console::SetCursorPosition(5, 38); cout << "      ";
				Console::SetCursorPosition(29, 38); cout << "jugar";
			}
			else if (pos == 27 && tecla == 13) {
				pag = 1;
				Console::Clear();
				fflush(stdin);
				break;
			}
			else if (pos == 3 && tecla == 13) {
				tecla = 0;
				Console::Clear();
				fflush(stdin);
				break;
			}
		}
	}

}
void presentacion_parte2(char& tecla, int& pag) {
	int aum = 0, posy = 19;
	Console::ForegroundColor = ConsoleColor::DarkGreen;
	Console::SetCursorPosition(9, 19); printf("-->");
	while (1) {
		if (aum % 2 == 0) Console::ForegroundColor = ConsoleColor::DarkYellow;
		else Console::ForegroundColor = ConsoleColor::DarkGreen;
		Console::SetCursorPosition(7, 2); printf("%c%c%c  %c%c  %c   %c%c  %c%c%c  %c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
		Console::SetCursorPosition(7, 3); printf("%c   %c  %c %c  %c  %c %c   %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
		Console::SetCursorPosition(7, 4); printf("%c %c %c%c%c%c %c  %c%c%c%c %c %c %c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
		Console::SetCursorPosition(7, 5); printf("%c%c%c %c  %c %c%c %c  %c %c%c%c %c  %c", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);


		Console::ForegroundColor = ConsoleColor::DarkRed;
		Console::SetCursorPosition(2 + aum, 9); printf("%c%c%c", 178, 178, 178);
		Console::ForegroundColor = ConsoleColor::White;
		Console::SetCursorPosition(2 + aum, 10); printf(" %c  ", 219);
		Console::SetCursorPosition(2 + aum, 11); printf("%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219);
		Console::SetCursorPosition(2 + aum, 12); printf(" %c  ", 219);
		Console::ForegroundColor = ConsoleColor::DarkRed;
		Console::SetCursorPosition(2 + aum, 13); printf("%c%c%c", 178, 178, 178);
		Sleep(100);
		Console::SetCursorPosition(2 + aum, 9); printf("   ");
		Console::SetCursorPosition(2 + aum, 10); printf("    ");
		Console::SetCursorPosition(2 + aum, 11); printf("      ");
		Console::SetCursorPosition(2 + aum, 12); printf("    ");
		Console::ForegroundColor = ConsoleColor::DarkRed;
		Console::SetCursorPosition(2 + aum, 13); printf("    ");

		Console::ForegroundColor = ConsoleColor::White;
		Console::SetCursorPosition(13, 19); printf("Iniciar juego");
		Console::SetCursorPosition(13, 21); printf("Instrucciones");

		Console::ForegroundColor = ConsoleColor::DarkRed;
		Console::SetCursorPosition(6, 28); cout << " 1er BONUS POR 30000 PUNTOS";
		Console::SetCursorPosition(6, 30); cout << "2do BONUS POR 120000 PUNTOS";
		Console::SetCursorPosition(6, 32); cout << " Y POR CADA 120000 PUNTOS";
		Console::ForegroundColor = ConsoleColor::Blue;
		Console::SetCursorPosition(1, 38); cout << "GALAGA 2019 - PROYECTO UPC - 1er CICLO";
		aum++;
		if (_kbhit()) {
			tecla = getch();
			if (tecla == T_ARR) {
				Console::SetCursorPosition(9, 21); printf("   ");
				Console::SetCursorPosition(9, 19); printf("   ");
				Console::ForegroundColor = ConsoleColor::DarkGreen;
				Console::SetCursorPosition(9, 19); printf("-->");
				posy = 19;
			}
			else if (tecla == T_ABA) {
				Console::SetCursorPosition(9, 19); printf("   ");
				Console::SetCursorPosition(9, 21); printf("   ");
				Console::ForegroundColor = ConsoleColor::DarkGreen;
				Console::SetCursorPosition(9, 21); printf("-->");
				posy = 21;
			}
		}
		if (posy == 19 && tecla == 13) {
			Console::Clear();
			fflush(stdin);
			pag = 1;
			break;
		}
		else if (posy == 21 && tecla == 13) {
			Console::Clear();
			fflush(stdin);
			pag = 2;
			break;
		}
		if (aum == 33) aum = 0;
	}
}
void presentacion_parte1(char& tecla) {
	int matriz[40][40] =
	{ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	system("color 7C");
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			if (matriz[i][j] == 1) {
				Console::ForegroundColor = ConsoleColor::DarkRed;
				Console::SetCursorPosition(j, i);
				printf("*");

			}
		}
	}
	Console::SetCursorPosition(11, 35); cout << " exigete, innova";
	while (1) {
		if (_kbhit()) {
			tecla = getch();
			if (tecla == 13) {
				tecla = 0;
				break;
			}

		}
	}

	fflush(stdin);
	system("color 0F");
	system("cls");
}
T_enm enm_aleatorio(int p) {
	Random r;
	n = r.Next(0, p);
	Sleep(p);

	if (n >= 0 && n <= p / 3) {
		n = 0;
		return mariposa;
	}
	else if (n >= (p / 3) + 1 && n <= ((2 * p) / 3)) {
		n = 0;
		return avispa;
	}
	else if (n <= ((2 * p) / 3) + 1 && n <= p) {
		n = 0;
		return comandante;
	}
}
void cant_enm(nivel _nv, int& _cT1, int& _cT2, int& _cT3) {
	if (_nv == facil) {
		_cT1 = 2;
		_cT2 = 1;
		_cT3 = 1;
	}
	else if (_nv == normal) {
		_cT1 = 4;
		_cT2 = 2;
		_cT3 = 2;
	}
	else if (_nv == dificil) {
		_cT1 = 4;
		_cT2 = 3;
		_cT3 = 2;
	}
}
void inicializar_disparo_enm(int& cant_balas, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm, int& _r) {
	int n, m, x = 0, y = 0;
	Random r;
	if (_r == 200) {
		for (int i = 0; i < cant_balas; i++) {
			n = r.Next(0, 7);

			if (n == 1) {
				m = r.Next(0, _ct1);
				Sleep(2);
				if (t1_h1[m].get_estado() == true) {
					x = t1_h1[m].get_xn() + 1;
					y = t1_h1[m].get_yn() + 1;
				}
			}
			else if (n == 2) {
				m = r.Next(0, _ct1);
				Sleep(2);
				if (t1_h2[m].get_estado() == true) {
					x = t1_h2[m].get_xn() + 1;
					y = t1_h2[m].get_yn() + 1;
				}
			}
			else if (n == 3) {
				m = r.Next(0, _ct2);
				Sleep(2);
				if (t2_h1[m].get_estado() == true) {
					x = t2_h1[m].get_xn() + 1;
					y = t2_h1[m].get_yn() + 1;
				}
			}
			else if (n == 4) {
				m = r.Next(0, _ct2);
				Sleep(2);
				if (t2_h2[i].get_estado() == true) {
					x = t2_h2[m].get_xn() + 1;
					y = t2_h2[m].get_yn() + 1;
				}
			}
			else if (n == 5) {
				m = r.Next(0, _ct3);
				Sleep(2);
				if (t3_h1[m].get_estado() == true) {
					x = t3_h1[m].get_xn() + 1;
					y = t3_h1[m].get_yn() + 1;
				}
			}
			else if (n == 6) {
				m = r.Next(0, _ct1);
				Sleep(2);
				if (t3_h2[m].get_estado() == true) {
					x = t3_h2[m].get_xn() + 1;
					y = t3_h2[m].get_yn() + 1;
				}
			}
			if (x != 0 && y != 0) {
				balas_enm[i].pos_xa = balas_enm[i].pos_xn = x;
				balas_enm[i].pos_ya = balas_enm[i].pos_yn = y;
				balas_enm[i].est = true;
			}
			else {
				balas_enm[i].est = false;
			}
		}
	}
}
void mover_balas_enm(int& cant_disp, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm, int& _r, balas_navd*& bala_enm) {
	if (_r >= 200) {
		for (int i = 0; i < cant_disp; i++) {
			bala_enm[i].pos_yn++;
			if (bala_enm[i].pos_yn >= 0 && bala_enm[i].pos_yn < 39 && balas_enm[i].est == true) {
				Console::SetCursorPosition(bala_enm[i].pos_xa, bala_enm[i].pos_ya); cout << " ";
				Console::SetCursorPosition(bala_enm[i].pos_xn, bala_enm[i].pos_yn); cout << "v";
				bala_enm[i].pos_ya = bala_enm[i].pos_yn;
			}
			else if (bala_enm[i].pos_yn == 39 && balas_enm[i].est == true) {
				Console::SetCursorPosition(bala_enm[i].pos_xa, bala_enm[i].pos_ya); cout << " ";
			}
			else if (bala_enm[i].pos_yn > 39) {
				bala_enm[i] = bala_enm[cant_disp - 1];
				actualizar_disparo_enm(cant_disp, _ct1, _ct2, _ct3, t1_h1, t1_h2, t2_h1, t2_h2, t3_h1, t3_h2, balas_enm, _r);
			}

		}
	}

}
void actualizar_disparo_enm(int& cant_balas, int& _ct1, int& _ct2, int& _ct3, nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, balas_navd*& balas_enm, int& _r) {
	int n, m, p, x = 0, y = 0;
	m = rand() % _ct1;
	if (t1_h1[m].get_estado() == true) {
		x = t1_h1[m].get_xn() + 1;
		y = t1_h1[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	if (t1_h2[m].get_estado() == true) {
		x = t1_h2[m].get_xn() + 1;
		y = t1_h2[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	n = rand() % _ct2;
	if (t2_h1[m].get_estado() == true) {
		x = t2_h1[m].get_xn() + 1;
		y = t2_h1[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	if (t2_h2[m].get_estado() == true) {
		x = t2_h2[m].get_xn() + 1;
		y = t2_h2[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	p = rand() % _ct3;
	if (t3_h1[m].get_estado() == true) {
		x = t3_h1[m].get_xn() + 1;
		y = t3_h1[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	if (t3_h2[m].get_estado() == true) {
		x = t3_h2[m].get_xn() + 1;
		y = t3_h2[m].get_yn() + 1;
		balas_enm[cant_balas - 1].pos_xa = balas_enm[cant_balas - 1].pos_xn = x;
		balas_enm[cant_balas - 1].pos_ya = balas_enm[cant_balas - 1].pos_yn = y;
	}
	
	if (x != 0 && y != 0) {
		balas_enm[cant_balas - 1].est = true;
	}
	else
		balas_enm[cant_balas - 1].est = false;
	
}
void perder_vida(balas_navd*& b_enm, nav_def& nave, int c_balas, int& _r) {
	if (_r >= 201) {
		for (int i = 0; i < c_balas; i++) {
			if ((b_enm[i].pos_xn == nave.get_x() || b_enm[i].pos_xn == nave.get_x() + 1 || b_enm[i].pos_xn == nave.get_x() + 2) && b_enm[i].pos_yn == nave.get_y() && b_enm[i].est == true) {
				if (nave.getter_vida() >= 1) {
					nave.set_vida(1);
					Console::SetCursorPosition(nave.get_x(), nave.get_y()); cout << "   ";
					Console::SetCursorPosition(nave.get_x() - 1, nave.get_y()); cout << "* * *";
					Sleep(100);
					Console::SetCursorPosition(nave.get_x() - 1, nave.get_y()); cout << "     ";
					Sleep(1000);
					b_enm[i].est = false;
				}

			}
			Console::SetCursorPosition(nave.get_x(), nave.get_y()); cout << "|w|";
		}
	}
}
void eliminar_enm(nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, int ct1, int ct2, int ct3, nav_def& nave) {
	for (int i = 0; i <= nave.get_balas_disp(); i++) {
		for (int j = 0; j < ct1; j++) {
			if ((t1_h1[j].get_xn() == b_navG[i].pos_xn || t1_h1[j].get_xn() + 1 == b_navG[i].pos_xn || t1_h1[j].get_xn() + 2 == b_navG[i].pos_xn) && t1_h1[j].get_yn() == b_navG[i].pos_yn && t1_h1[j].get_estado() == true) {
				t1_h1[j].borrar_nave();
				t1_h1[j].set_estado(false);
				if (t1_h1[j].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t1_h1[j].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t1_h1[j].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
			else if ((t1_h2[j].get_xn() == b_navG[i].pos_xn || t1_h2[j].get_xn() + 1 == b_navG[i].pos_xn || t1_h2[j].get_xn() + 2 == b_navG[i].pos_xn) && t1_h2[j].get_yn() == b_navG[i].pos_yn && t1_h2[j].get_estado() == true) {
				t1_h2[j].borrar_nave();
				t1_h2[j].set_estado(false);
				if (t1_h2[j].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t1_h2[j].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t1_h2[j].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
		}
		for (int u = 0; u < ct2; u++) {
			if ((t2_h1[u].get_xn() == b_navG[i].pos_xn || t2_h1[u].get_xn() + 1 == b_navG[i].pos_xn || t2_h1[u].get_xn() + 2 == b_navG[i].pos_xn) && t2_h1[u].get_yn() == b_navG[i].pos_yn && t2_h1[u].get_estado() == true) {
				t2_h1[u].borrar_nave();
				t2_h1[u].set_estado(false);
				if (t2_h1[u].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t2_h1[u].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t2_h1[u].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
			else if ((t2_h2[u].get_xn() == b_navG[i].pos_xn || t2_h2[u].get_xn() + 1 == b_navG[i].pos_xn || t2_h2[u].get_xn() + 2 == b_navG[i].pos_xn) && t2_h2[u].get_yn() == b_navG[i].pos_yn && t2_h2[u].get_estado() == true) {
				t2_h2[u].borrar_nave();
				t2_h2[u].set_estado(false);
				if (t2_h2[u].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t2_h2[u].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t2_h2[u].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
		}
		for (int w = 0; w < ct3; w++) {
			if ((t3_h1[w].get_xn() == b_navG[i].pos_xn || t3_h1[w].get_xn() + 1 == b_navG[i].pos_xn || t3_h1[w].get_xn() + 2 == b_navG[i].pos_xn) && t3_h1[w].get_yn() == b_navG[i].pos_yn && t3_h1[w].get_estado() == true) {
				t3_h1[w].borrar_nave();
				t3_h1[w].set_estado(false);
				if (t3_h1[i].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t3_h1[i].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t3_h1[i].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
			else if ((t3_h2[w].get_xn() == b_navG[i].pos_xn || t3_h2[w].get_xn() + 1 == b_navG[i].pos_xn || t3_h2[w].get_xn() + 2 == b_navG[i].pos_xn) && t3_h2[w].get_yn() == b_navG[i].pos_yn && t3_h2[w].get_estado() == true) {
				t3_h2[w].borrar_nave();
				t3_h2[w].set_estado(false);
				if (t3_h2[w].get_tipo() == mariposa) {
					puntuacion += 100;
				}
				else if (t3_h2[w].get_tipo() == avispa) {
					puntuacion += 150;
				}
				else if (t3_h2[w].get_tipo() == avispa) {
					puntuacion += 200;
				}
			}
		}
	}
}
bool enm_eliminados(nav_enm*& t1_h1, nav_enm*& t1_h2, nav_enm*& t2_h1, nav_enm*& t2_h2, nav_enm*& t3_h1, nav_enm*& t3_h2, int& ct1, int& ct2, int& ct3) {
	int n1, n2, n3;  n1 = n2 = n3 = 0;
	bool r1, r2, r3; r1 = r2 = r3 = false;
	for (int i = 0; i < ct1; i++) {
		if (t1_h1[i].get_estado() == false && t1_h2[i].get_estado() == false) {
			n1++;
			if (n1 == ct1) {
				r1 = true;
			}
		}
	}
	for (int i = 0; i < ct2; i++) {
		if (t2_h1[i].get_estado() == false && t2_h2[i].get_estado() == false) {
			n2++;
			if (n2 == ct2) {
				r2 = true;
			}
		}
	}
	for (int i = 0; i < ct3; i++) {
		if (t3_h1[i].get_estado() == false && t3_h2[i].get_estado() == false) {
			n3++;
			if (n3 == ct3) {
				r3 = true;
			}
		}
	}
	if (r3 == true && r2 == true && r1 == true) {
		return true;
	}
	else
		return false;
}