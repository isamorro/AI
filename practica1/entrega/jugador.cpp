#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think (Sensores sensores)
{

	Action accion = actIDLE;
	int a;

	// Mostrar el valor de los sensores

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  	cout << " Norte\n";	break;
		case noreste: 	cout << " Noreste\n";	break;
		case este:    	cout << " Este\n";	break;
		case sureste: 	cout << " Sureste\n";	break;
		case sur:     	cout << " Sur\n";	break;
		case suroeste:	cout << " Suroeste\n";	break;
		case oeste:   	cout << " Oeste\n";	break;
		case noroeste:	cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;

	///////////////////////////////////////////////////////////////////////////////

	// PRIMERA FASE : Actualización de las variables de estado

	// Se actualizan los cambios que la última acción realizada provocó sobre el mundo

	modificarState(current_state);

	// Obtención de bikini o de zapatillas
	if (!bikini || !zapatillas){
		if (sensores.terreno[0] == 'K') bikini = true;
		else if (sensores.terreno[0] == 'D') zapatillas = true;
	}

	// Casos de reinicio
	if (sensores.reset == true) reiniciar();
	
	// Casos de giros continuados sin avance
	if (last_action == actTURN_L or last_action == actTURN_SR) contador_giros++;
	else contador_giros = 0;
	
	// Actualización si alcanza la casilla de posición correcta
	if ((sensores.terreno[0] == 'G' or sensores.nivel == 0)
		and !bien_situado) 
		establecerPosicion(sensores);
	
		
	// Pintar en mapa resultado 
	// Solo pinta si ha sido atacado por un lobo

	if (bien_situado) {

		if (recien_situado and pintar_matriz_auxiliar){
			modificarState(auxiliar_state);
			copiarMatriz(sensores);
			pintarPrecipicios();
			recien_situado = false;	
		}

		else PonerTerrenoEnMatriz(sensores, current_state, mapaResultado);
	
	} else if (pintar_matriz_auxiliar){
		modificarState(auxiliar_state);
		PonerTerrenoEnMatriz(sensores, auxiliar_state, mapaAuxiliar);	
	}

	// Actualiza el contador de mapaContador en la casilla actual
	if (bien_situado) mapaContador[current_state.fil][current_state.col] += 1; 
	

	///////////////////////////////////////////////////////////////////////////////

	// SEGUNDA FASE : determina la siguiente acción a realizar

	int casillaEspecial = casillaPrioritaria(sensores);
	bool hay_accion = false;

	// Controla giros continuados ineficientes
	if (contador_giros > 10 and casillaTransitable(2, sensores)) accion = actWALK;
	else {

		// Busca dentro de su visión alguna casilla especial
		if (casillaEspecial != -1) {
			accion = pasoAcasillaEspecial(casillaEspecial);
			hay_accion = true;
			if (accion == actWALK and !casillaTransitable(2, sensores))
				hay_accion = false;
		}

		// Caso en el que no hay casilla especial o no es posible llegar hacia esta
		if (!hay_accion){

			// En algunos casos selecciona las casillas menos transitadas
			if (sensores.vida < 1400 and bien_situado and cont_ciclos == 0){
				int pos = seleccionarCasilla();
				if (casillaTransitable(pos, sensores)) {
					if (pos == 1) accion = actTURN_L;
					if (pos == 2) accion = actWALK;
					if (pos == 3) {
						accion = actTURN_SR;
						cont_ciclos = 20;
					}
				}
			}

			// En caso contrario avanza si es posible
			else if (((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' 
					or sensores.terreno[2] == 'G' or sensores.terreno[2] == 'K' 
					or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'X'
					or sensores.terreno[2] == 'A' or sensores.terreno[2] == 'B')) 
					and sensores.agentes[2]=='_'){

						accion = actWALK;

			} else {

					// Finalmente decide aleatoriamente a donde gira
					if (!girar_derecha){
						accion = actTURN_L;
						girar_derecha = (rand()%2 == 0);
					} else {
						accion = actTURN_SR;
						girar_derecha = (rand()%2==0);
					}
			}
		}
	}

	cont_ciclos--;
	if (cont_ciclos == -1) cont_ciclos = 20;

	// Recargamos batería si estamos en casilla de recarga
	if (sensores.terreno[0] == 'X' and sensores.bateria < 3500)
		accion = actIDLE;
	
	// Devuelve el valor de la acción
	last_action = accion;
	return accion;
}


// Esta función pinta en mapaResultado si estamos bien situados y en
// mapaAuxiliar en caso contrario
void ComportamientoJugador::PonerTerrenoEnMatriz 	(Sensores sensores, 
                              						const state &st, 
													vector<vector<unsigned char>> &matriz)
{

	int filAct, colAct;
	Orientacion orientacion;
	vector <unsigned char> terreno = sensores.terreno;
	int nivel = sensores.nivel;

	filAct = st.fil;
	colAct = st.col;
	orientacion = st.brujula;

	switch(orientacion){
		case norte: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct-1][colAct-1] = terreno[1];
			matriz[filAct-1][colAct] = terreno[2];
			matriz[filAct-1][colAct+1] = terreno[3];
			matriz[filAct-2][colAct-2] = terreno[4];
			matriz[filAct-2][colAct-1] = terreno[5];
			matriz[filAct-2][colAct+1] = terreno[7];
			matriz[filAct-2][colAct+2] = terreno[8];
			matriz[filAct-3][colAct-3] = terreno[9];
			matriz[filAct-3][colAct-2] = terreno[10];
			matriz[filAct-3][colAct+2] = terreno[14];
			matriz[filAct-3][colAct+3] = terreno[15];

			if (nivel != 3) {
				matriz[filAct-2][colAct] = terreno[6];
				matriz[filAct-3][colAct-1] = terreno[11];
				matriz[filAct-3][colAct] = terreno[12];
				matriz[filAct-3][colAct+1] = terreno[13];
			}
				
			break;
		case noreste:
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct-1][colAct] = terreno[1];
			matriz[filAct-1][colAct+1] = terreno[2];
			matriz[filAct][colAct+1] = terreno[3];
			matriz[filAct-2][colAct] = terreno[4];
			matriz[filAct-2][colAct+1] = terreno[5];
			matriz[filAct-1][colAct+2] = terreno[7];
			matriz[filAct][colAct+2] = terreno[8];
			matriz[filAct-3][colAct] = terreno[9];
			matriz[filAct-3][colAct+1] = terreno[10];
			matriz[filAct-1][colAct+3] = terreno[14];
			matriz[filAct][colAct+3] = terreno[15];

			if (nivel != 3){
				matriz[filAct-2][colAct+2] = terreno[6];
				matriz[filAct-3][colAct+2] = terreno[11];
				matriz[filAct-3][colAct+3] = terreno[12];
				matriz[filAct-2][colAct+3] = terreno[13];
			}
			
			break;
		case este: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct-1][colAct+1] = terreno[1];
			matriz[filAct][colAct+1] = terreno[2];
			matriz[filAct+1][colAct+1] = terreno[3];
			matriz[filAct-2][colAct+2] = terreno[4];
			matriz[filAct-1][colAct+2] = terreno[5];
			matriz[filAct+1][colAct+2] = terreno[7];
			matriz[filAct+2][colAct+2] = terreno[8];
			matriz[filAct-3][colAct+3] = terreno[9];
			matriz[filAct-2][colAct+3] = terreno[10];
			matriz[filAct+2][colAct+3] = terreno[14];
			matriz[filAct+3][colAct+3] = terreno[15];
			
			if (nivel != 3){
				matriz[filAct][colAct+2] = terreno[6];
				matriz[filAct-1][colAct+3] = terreno[11];
				matriz[filAct][colAct+3] = terreno[12];
				matriz[filAct+1][colAct+3] = terreno[13];
			}
			
			break;
		case sureste: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct][colAct+1] = terreno[1];
			matriz[filAct+1][colAct+1] = terreno[2];
			matriz[filAct+1][colAct] = terreno[3];
			matriz[filAct][colAct+2] = terreno[4];
			matriz[filAct+1][colAct+2] = terreno[5];
			matriz[filAct+2][colAct+1] = terreno[7];
			matriz[filAct+2][colAct] = terreno[8];
			matriz[filAct][colAct+3] = terreno[9];
			matriz[filAct+1][colAct+3] = terreno[10];
			matriz[filAct+3][colAct+1] = terreno[14];
			matriz[filAct+3][colAct] = terreno[15];

			if (nivel != 3){
				matriz[filAct+2][colAct+2] = terreno[6];
				matriz[filAct+2][colAct+3] = terreno[11];
				matriz[filAct+3][colAct+3] = terreno[12];
				matriz[filAct+3][colAct+2] = terreno[13];
			}
			break;
		case sur:
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct+1][colAct+1] = terreno[1];
			matriz[filAct+1][colAct] = terreno[2];
			matriz[filAct+1][colAct-1] = terreno[3];
			matriz[filAct+2][colAct+2] = terreno[4];
			matriz[filAct+2][colAct+1] = terreno[5];
			matriz[filAct+2][colAct-1] = terreno[7];
			matriz[filAct+2][colAct-2] = terreno[8];
			matriz[filAct+3][colAct+3] = terreno[9];
			matriz[filAct+3][colAct+2] = terreno[10];
			matriz[filAct+3][colAct-2] = terreno[14];
			matriz[filAct+3][colAct-3] = terreno[15];

			if (nivel != 3){
				matriz[filAct+2][colAct] = terreno[6];
				matriz[filAct+3][colAct+1] = terreno[11];
				matriz[filAct+3][colAct] = terreno[12];
				matriz[filAct+3][colAct-1] = terreno[13];
			}
			break;
		case suroeste: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct+1][colAct] = terreno[1];
			matriz[filAct+1][colAct-1] = terreno[2];
			matriz[filAct][colAct-1] = terreno[3];
			matriz[filAct+2][colAct] = terreno[4];
			matriz[filAct+2][colAct-1] = terreno[5];
			matriz[filAct+1][colAct-2] = terreno[7];
			matriz[filAct][colAct-2] = terreno[8];
			matriz[filAct+3][colAct] = terreno[9];
			matriz[filAct+3][colAct-1] = terreno[10];
			matriz[filAct+1][colAct-3] = terreno[14];
			matriz[filAct][colAct-3] = terreno[15];

			if (nivel != 3){
				matriz[filAct+2][colAct-2] = terreno[6];
				matriz[filAct+3][colAct-2] = terreno[11];
				matriz[filAct+3][colAct-3] = terreno[12];
				matriz[filAct+2][colAct-3] = terreno[13];
			}
			break;
		case oeste: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct+1][colAct-1] = terreno[1];
			matriz[filAct][colAct-1] = terreno[2];
			matriz[filAct-1][colAct-1] = terreno[3];
			matriz[filAct+2][colAct-2] = terreno[4];
			matriz[filAct+1][colAct-2] = terreno[5];		
			matriz[filAct-1][colAct-2] = terreno[7];
			matriz[filAct-2][colAct-2] = terreno[8];
			matriz[filAct+3][colAct-3] = terreno[9];
			matriz[filAct+2][colAct-3] = terreno[10];
			matriz[filAct-2][colAct-3] = terreno[14];
			matriz[filAct-3][colAct-3] = terreno[15];

			if (nivel != 3){
				matriz[filAct][colAct-2] = terreno[6];
				matriz[filAct+1][colAct-3] = terreno[11];
				matriz[filAct][colAct-3] = terreno[12];
				matriz[filAct-1][colAct-3] = terreno[13];
			}
			break;
		case noroeste: 
			matriz[filAct][colAct] = terreno[0];
			matriz[filAct][colAct-1] = terreno[1];
			matriz[filAct-1][colAct-1] = terreno[2];
			matriz[filAct-1][colAct] = terreno[3];
			matriz[filAct][colAct-2] = terreno[4];
			matriz[filAct-1][colAct-2] = terreno[5];
			matriz[filAct-2][colAct-1] = terreno[7];
			matriz[filAct-2][colAct] = terreno[8];
			matriz[filAct][colAct-3] = terreno[9];
			matriz[filAct-1][colAct-3] = terreno[10];
			matriz[filAct-3][colAct-1] = terreno[14];
			matriz[filAct-3][colAct] = terreno[15];

			if (nivel != 3){
				matriz[filAct-2][colAct-2] = terreno[6];
				matriz[filAct-2][colAct-3] = terreno[11];
				matriz[filAct-3][colAct-3] = terreno[12];
				matriz[filAct-3][colAct-2] = terreno[13];
			}
			break;
	}
	
}

void ComportamientoJugador::reiniciar(){
	bien_situado = false;
	bikini = false;
	zapatillas = false;
	pintar_matriz_auxiliar = false;
	// resetearMatrizAuxiliar();
}

void ComportamientoJugador::establecerPosicion(Sensores sensores){
	// Activa los sensores de orientación y posición
	// Ofrece posición y orientación exacta 
	current_state.fil = sensores.posF;
	current_state.col = sensores.posC;
	current_state.brujula = sensores.sentido;
	bien_situado = true;
}

void ComportamientoJugador::pintarPrecipicios(){

	int tam = mapaResultado.size();

	// Tres primeras filas
	for (int i=0; i < 3; i++){
		for (int j=0; j < tam; j++){
			mapaResultado[i][j] = 'P'; 
		}
	}

	// Tres últimas filas
	for (int i=tam-3; i < tam; i++){
		for (int j=0; j < tam; j++){
			mapaResultado[i][j] = 'P'; 
		}
	}

	// Tres primeras columnas
	for (int i=3; i < tam - 3; i++){
		for (int j=0; j < 3; j++){
			mapaResultado[i][j] = 'P'; 
		}
	}

	// Tres últimas columnas
	for (int i=3; i < tam - 3; i++){
		for (int j=tam-3; j < tam; j++){
			mapaResultado[i][j] = 'P'; 
		}
	}

}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

void ComportamientoJugador::modificarState(state &st){

	int a;

	switch (last_action) {
		case actWALK:
			switch(st.brujula){
				case norte: 
					st.fil--;
					break;
				case noreste:
					st.fil--; 
					st.col++; 
					break;
				case este:
					st.col++;
					break;
				case sureste:
					st.col++;
					st.fil++;
					break;
				case sur:
					st.fil++;
					break;
				case suroeste:
					st.fil++;
					st.col--;
					break;
				case oeste:
					st.col--;
					break;
				case noroeste:
					st.col--;
					st.fil--;
					break;
			}

		break; // Fin del caso actWALK

		case actRUN:
			switch(st.brujula){
				case norte: 
					st.fil -= 2;
					break;
				case noreste:
					st.fil -= 2;
					st.col += 2; 
					break;
				case este:
					st.col += 2; 
					break;
				case sureste:
					st.col += 2; 
					st.fil += 2;
					break;
				case sur:
					st.fil += 2;
					break;
				case suroeste:
					st.fil += 2;
					st.col -= 2; 
					break;
				case oeste:
					st.col -= 2; 
					break;
				case noroeste:
					st.col -= 2; 
					st.fil -= 2;
					break;
			}

		break; // Fin del caso actRUN

		case actTURN_L:
			// Giro de 90 grados
			a = st.brujula;
			a = (a+6)%8;
			st.brujula = static_cast<Orientacion>(a); 
		break; // Fin del caso actTURN_L

		case actTURN_SR:
			// Giro de 45 grados 
			a = st.brujula;
			a = (a+1)%8;
			st.brujula = static_cast<Orientacion>(a); 
		break;

	}
}

// Copia mapaAuxiliar en mapaResultado una vez bien_situado está a true	
void ComportamientoJugador::copiarMatriz (Sensores sensores){
	
	int tam = mapaResultado.size();
	
	int columnas_por_la_izquierda, filas_por_arriba, f, c;

	if (sensores.nivel == 1 or sensores.nivel == 2){
		
		columnas_por_la_izquierda = current_state.col;
		filas_por_arriba = current_state.fil;
		f = auxiliar_state.fil - filas_por_arriba;
		c = auxiliar_state.col - columnas_por_la_izquierda;

		for (int i=0; i < tam; i++){
			for (int j=0; j < tam; j++){
				if (mapaResultado[i][j] == '?')
					mapaResultado[i][j] = mapaAuxiliar[f][c];
				c++;
			}
			c = auxiliar_state.col - columnas_por_la_izquierda;
			f++;
		}
	}

	// Comentado por que no da resultados óptimos
	// else if (sensores.nivel == 3) rotarMatriz(sensores);
		
}

// Escoge casilla prioritaria en el terreno a su visión si hay
int ComportamientoJugador::casillaPrioritaria(Sensores sensores){

	bool encontradoMasPrioridad = false;
	int pos = -1;

	for (int i=1; i < sensores.terreno.size(); i++){

		if (sensores.terreno[i] == 'G' and !bien_situado){
			pos = i;
			encontradoMasPrioridad = true;
		}

		if (((sensores.terreno[i] == 'K' and !bikini)
			or (sensores.terreno[i] == 'D' and !zapatillas)
			or (sensores.terreno[i] == 'X' and sensores.bateria < 600))
			and !encontradoMasPrioridad){
			pos = i;
		}

	}

	return pos;
}

Action ComportamientoJugador::pasoAcasillaEspecial (int pos)
{
	Action accion;

	switch (pos)
	{
		case 1: 
			accion = actTURN_L; 
			break;
		case 4: 
			accion = actTURN_L; 
			break;
		case 5: 
			accion = actTURN_L; 
			break;
		case 9: 
			accion = actTURN_L; 
			break;
		case 10: 
			accion = actTURN_L; 
			break;
		case 11:
			accion = actTURN_L; 
			break;
		case 2: 
			accion = actWALK;	
			break;
		case 6: 
			accion = actWALK;	
			break;
		case 7: 
			accion = actWALK;	
			break;
		case 12: 
			accion = actWALK;	
			break;
		case 13: 
			accion = actWALK;	
			break;
		case 14:
			accion = actWALK;	
			break;
		case 3: 
			accion = actTURN_SR;
			break;
		case 8: 
			accion = actTURN_SR;
			break;
		case 15:
			accion = actTURN_SR;
			break;
	}

	return accion;

}

bool ComportamientoJugador::casillaTransitable(int pos, Sensores sensores){

	bool transitable = false;

	if (	(sensores.terreno[pos]=='T' or sensores.terreno[pos]=='S' 
			or sensores.terreno[pos] == 'G' or sensores.terreno[pos] == 'K' 
			or sensores.terreno[pos] == 'D' or sensores.terreno[pos] == 'X'
			or sensores.terreno[pos] == 'A' or sensores.terreno[pos] == 'B')
			and sensores.agentes[pos] == '_'){
		
		transitable = true;
	}

	return transitable;
}

// Selecciona entre las casillas de alrededor, la menos visitada
int ComportamientoJugador::seleccionarCasilla (){

	int minimo;
	int num = 1;
	int filAct = current_state.fil;
	int colAct = current_state.col;

	switch (current_state.brujula)
	{
		case norte:

			minimo = mapaResultado [filAct-1][colAct-1];
			if (minimo > mapaResultado[filAct-1][colAct]){
				minimo = mapaResultado[filAct-1][colAct];
				num = 2;
			}
			if (minimo > mapaResultado[filAct-1][colAct+1]){
				minimo = mapaResultado[filAct-1][colAct+1];
				num = 3;
			}
			break;
		
		case noreste: 

			minimo = mapaResultado[filAct-1][colAct];
			if (minimo > mapaResultado[filAct-1][colAct+1]){
				minimo = mapaResultado[filAct-1][colAct+1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct][colAct+1]){
				minimo = mapaResultado[filAct][colAct+1];
				num = 3;
			}
			break;

		case este: 

			minimo = mapaResultado[filAct-1][colAct+1];
			if (minimo > mapaResultado[filAct][colAct+1]){
				minimo = mapaResultado[filAct][colAct+1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct+1][colAct+1]){
				minimo = mapaResultado[filAct+1][colAct+1];
				num = 3;
			}
			break;

		case sureste:
			
			minimo = mapaResultado[filAct][colAct+1];
			if (minimo > mapaResultado[filAct+1][colAct+1]){
				minimo = mapaResultado[filAct+1][colAct+1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct+1][colAct]){
				minimo = mapaResultado[filAct+1][colAct];
				num = 3;
			}
			break;

		case sur: 

			minimo = mapaResultado[filAct+1][colAct+1];
			if (minimo > mapaResultado[filAct+1][colAct]){
				minimo = mapaResultado[filAct+1][colAct];
				num = 2;
			}
			if (minimo > mapaResultado[filAct+1][colAct-1]){
				minimo = mapaResultado[filAct+1][colAct-1];
				num = 3;
			}
			break;

		case suroeste: 
	
			minimo = mapaResultado[filAct+1][colAct];
			if (minimo > mapaResultado[filAct+1][colAct-1]){
				minimo = mapaResultado[filAct+1][colAct-1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct][colAct-1]){
				minimo = mapaResultado[filAct][colAct-1];
				num = 3;
			}
			break;


		case oeste: 

			minimo = mapaResultado[filAct+1][colAct-1];
			if (minimo > mapaResultado[filAct][colAct-1]){
				minimo = mapaResultado[filAct][colAct-1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct-1][colAct-1]){
				minimo = mapaResultado[filAct-1][colAct-1];
				num = 3;
			} 
			break;

		case noroeste: 

			minimo = mapaResultado[filAct][colAct-1];
			if (minimo > mapaResultado[filAct-1][colAct-1]){
				minimo = mapaResultado[filAct-1][colAct-1];
				num = 2;
			}
			if (minimo > mapaResultado[filAct-1][colAct]){
				minimo = mapaResultado[filAct-1][colAct];
				num = 3;
			} 
			break;
	}

	return num;
}


///////////// FUNCIONES NO USADAS POR ERRORES EN EJECUCIÓN ////////////////////

void ComportamientoJugador::rotarMatriz(Sensores sensores){

	int diferencia = auxiliar_state.brujula - sensores.sentido;
	int tam = mapaAuxiliar.size();

	vector<vector<unsigned char>> mapaRotado;
	vector <unsigned char> aux_char (tam,'?');
    for (int i=0; i < tam; i++)
        mapaRotado.push_back(aux_char);


	int f, c;
	pair <int, int> pos;

	switch (diferencia)
	{
		case 0:

			for (int i=0; i < tam; i++){
				for (int j=0; j < tam; j++){
					mapaRotado[i][j] = mapaAuxiliar[i][j];
				}
			}

			pos.first = auxiliar_state.fil;
			pos.second = auxiliar_state.col;

			break;

		case -2: case 6:

			f = 0;
			c = tam-1;
			for (int i=0; i < tam; i++){
				for (int j=0; j < tam; j++){
					mapaRotado[i][j] = mapaAuxiliar[f][c];
					f++;
				}
				c--;
				f = 0;
			}

			pos.first = tam - 1 - auxiliar_state.col;
			pos.second = auxiliar_state.fil;

			break;

		case 4: case -4:

			f = tam-1;
			c = tam-1;
			for (int i=0; i < tam; i++){
				for (int j=0; j < tam; j++){
					mapaRotado[i][j] = mapaAuxiliar[f][c];
					c--;
				}
				f--;
				c = tam - 1;
			}

			pos.first = tam - 1 - auxiliar_state.fil;
			pos.second = tam - 1 - auxiliar_state.col;

			break;

		case -6: case 2:
		
			f = tam-1;
			c = 0;
			for (int i=0; i < tam; i++){
				for (int j=0; j < tam; j++){
					mapaRotado[i][j] = mapaAuxiliar[f][c];
					f--;
				}
				c++;
				f = tam - 1;
			}

			pos.first = auxiliar_state.col;
			pos.second = tam - 1 - auxiliar_state.fil;

			break;

	}

	
	int f_pintar = pos.first - current_state.fil;
	int c_pintar = pos.second - current_state.col;

	for (int i=3; i < mapaResultado.size()-3; i++){
		for (int j=3; j < mapaResultado.size()-3; j++){
			if (mapaResultado[i][j] == '?')
				mapaResultado[i][j] = mapaRotado[f_pintar+i][c_pintar+j];
		}
	}

}

// Resetea la matrizAuxiliar cuando hay caso de reinicio
void ComportamientoJugador::resetearMatrizAuxiliar(){

	int tam = mapaAuxiliar.size();
	for (int i=0; i < tam; i++){
		for (int j=0; j < tam; j++){
			mapaAuxiliar[i][j] = '?';
		}
	}

	auxiliar_state.fil = mapaResultado.size();
	auxiliar_state.col = mapaResultado.size();
	auxiliar_state.brujula = norte;
}
