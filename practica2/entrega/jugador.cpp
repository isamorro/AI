#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>


//////////////////////// FUNCIONES INDEPENDIENTES DEL NIVEL DE JUEGO //////////////////////////////////

bool ComportamientoJugador::CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M' );
}

ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}

void ComportamientoJugador::AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

void ComportamientoJugador::PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK){
			cout << "W ";
		}
		else if (*it == actRUN){
			cout << "R ";
		}
		else if (*it == actTURN_SR){
			cout << "r ";
		}
		else if (*it == actTURN_L){
			cout << "L ";
		}
		else if (*it == act_CLB_WALK){
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP){
			cout << "cS ";
		}
		else if (*it == actIDLE){
			cout << "I ";
		}
		else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

int ComportamientoJugador::ValorCoste (int f, int c, bool bikini, bool zapatillas, Action accion,
									  const vector<vector<unsigned char> > &mapa){

	int coste = 0;

	switch (accion)
	{
		case act_CLB_WALK: case actWALK:
			
			if (mapa[f][c] == 'A'){
				if (bikini) coste = 10;
				else coste = 100;
			}
			else if (mapa[f][c] == 'B'){
				if (zapatillas) coste = 15;
				else coste = 50;
			}
			else if (mapa[f][c] == 'T') coste = 2;
			else coste = 1;
		break;

		case actRUN:
			
			if (mapa[f][c] == 'A'){
				if (bikini) coste = 15;
				else coste = 150;
			}
			else if (mapa[f][c] == 'B'){
				if (zapatillas) coste = 25;
				else coste = 75;
			}
			else if (mapa[f][c] == 'T') coste = 3;
			else coste = 1;

		break;

		case actTURN_L:
			if (mapa[f][c] == 'A'){
				if (bikini) coste = 5;
				else coste = 30;
			}
			else if (mapa[f][c] == 'B'){
				if (zapatillas) coste = 1;
				else coste = 7;
			}
			else if (mapa[f][c] == 'T') coste = 2;
			else coste = 1;
		break;

		case act_CLB_TURN_SR: case actTURN_SR:
			if (mapa[f][c] == 'A'){
				if (bikini) coste = 2;
				else coste = 10;
			}
			else if (mapa[f][c] == 'B'){
				if (zapatillas) coste = 1;
				else coste = 5;
			}
			else coste = 1;
		break;

	}

	return coste;

}

bool ComportamientoJugador::colaboradorAVista(ubicacion jugador, ubicacion colaborador) {

	int fc = colaborador.f;
	int cc = colaborador.c;
	int fj = jugador.f;
	int cj = jugador.c;

	switch (jugador.brujula)
	{
		case norte: 
			if 		(fc == fj -1 and (cc == cj-1 || cc == cj || cc == cj+1)) return true;
			else if (fc == fj -2 and (cc == cj-2 || cc == cj-1 || cc == cj || cc == cj+1 || cc == cj+2)) return true;
			else if (fc == fj -3 and (cc == cj-3 || cc == cj-2 || cc == cj-1 || cc == cj || cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else return false;
		break;

		case noreste:
			if 		(cc == cj and (fc == fj - 1 || fc == fj - 2 || fc == fj - 3)) return true;
			else if (cc == cj +1 and (fc == fj || fc == fj - 1 || fc == fj - 2 || fc == fj - 3)) return true;
			else if (cc == cj + 2 and (fc == fj || fc == fj - 1 || fc == fj - 2 || fc == fj - 3)) return true;
			else if (cc == cj + 3 and (fc == fj || fc == fj - 1 || fc == fj - 2 || fc == fj - 3)) return true;
			else return false;
		break;

		case este: 
			if 		(cc == cj + 1 and (fc == fj-1 || fc == fj || fc == fj+1)) return true;
			else if (cc == cj + 2 and (fc == fj-2 || fc == fj-1 || fc == fj || fc == fj+1 || fc == fj+2)) return true;
			else if (cc == cj + 3 and (fc == fj-3 || fc == fj-2 || fc == fj-1 || fc == fj || fc == fj+1 || fc == fj+2 || fc == fj+3)) return true;
			else return false;
		break;
			
		case sureste: 
			if		(fc == fj and (cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else if (fc == fj +1 and (cc == cj || cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else if (fc == fj +2 and (cc == cj || cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else if (fc == fj +3 and (cc == cj || cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else return false;
		break;

		case sur:
			if 		(fc == fj+1 and (cc == cj-1 || cc == cj || cc == cj+1)) return true;
			else if (fc == fj+2 and (cc == cj-2 || cc == cj-1 || cc == cj || cc == cj+1 || cc == cj+2)) return true;
			else if (fc == fj+3 and (cc == cj-3 || cc == cj-2 || cc == cj-1 || cc == cj || cc == cj+1 || cc == cj+2 || cc == cj+3)) return true;
			else return false;
		break;

		case suroeste: 
			if 		(fc == fj and (cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj +1 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj +2 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj +3 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else return false;
		break;

		case oeste: 
			if 		(cc == cj - 1 and (fc == fj-1 || fc == fj || fc == fj+1)) return true;
			else if (cc == cj - 2 and (fc == fj-2 || fc == fj-1 || fc == fj || fc == fj+1 || fc == fj+2)) return true;
			else if (cc == cj - 3 and (fc == fj-3 || fc == fj-2 || fc == fj-1 || fc == fj || fc == fj+1 || fc == fj+2 || fc == fj+3)) return true;
			else return false;
		break;;
			
		case noroeste: 
			if 		(fc == fj and (cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj -1 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj -2 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else if (fc == fj -3 and (cc == cj || cc == cj-1 || cc == cj-2 || cc == cj-3)) return true;
			else return false;
		break;
	}
	
}

//////////////////////// FUNCIONES ESPECIFICAS DEL NIVEL 0 /////////////////////////////////////////

stateN0 ComportamientoJugador::apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa){
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}
	return st_result;
}

bool ComportamientoJugador::Find(const stateN0 &item, const list<stateN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !((*it) == item))
		it++;

	return (!(it == lista.end()));
}

bool ComportamientoJugador::Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

list<Action> ComportamientoJugador::Anchura_nivel0	(const stateN0 &inicio, const ubicacion &final,
													const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo actWALK
		nodeN0 child_walk = current_node; 
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);

		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if (explored.find(child_walk) == explored.end()){
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if (explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}		

			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while (!frontier.empty()and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if (!frontier.empty())
					current_node = frontier.front();
			}
		}
	}
	
	if (SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan :";
		PintaPlan(current_node.secuencia);
	}
	
	
	return plan;
}

//////////////////////// FUNCIONES ESPECIFICAS DEL NIVEL 1 /////////////////////////////////////////

bool ComportamientoJugador::Find(const stateN1 &item, const list<nodeN1> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}

void ComportamientoJugador::VisualizaPlan(const stateN1 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN1 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

stateN1 ComportamientoJugador::apply(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa){

	stateN1 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2, sig_ubi_colab;

	switch (a)
	{
		case actWALK:

			sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				(sig_ubicacion.f != st.colaborador.f or sig_ubicacion.c != st.colaborador.c)){
					
					if (st.ultimaOrdenColaborador == act_CLB_WALK) {
						sig_ubi_colab = NextCasilla(st.colaborador);
						if (CasillaTransitable(sig_ubi_colab, mapa) and 
							(sig_ubi_colab.f != sig_ubicacion.f or sig_ubi_colab.c != sig_ubicacion.c)){
							st_result.jugador = sig_ubicacion;
							st_result.colaborador = sig_ubi_colab;
						}		
					}
					else {
						st_result.jugador = sig_ubicacion;
						if (st.ultimaOrdenColaborador == act_CLB_TURN_SR)
							st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
					}
			}
			
		break;
		
		case actRUN:

			sig_ubicacion = NextCasilla(st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				(sig_ubicacion.f != st.colaborador.f or sig_ubicacion.c != st.colaborador.c)){
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if (CasillaTransitable(sig_ubicacion2, mapa) and 
						(sig_ubicacion2.f != st.colaborador.f or sig_ubicacion2.c != st.colaborador.c)){
			
						if (st.ultimaOrdenColaborador == act_CLB_WALK) {
							sig_ubi_colab = NextCasilla(st.colaborador);
							if (CasillaTransitable(sig_ubi_colab, mapa) and 
							(sig_ubi_colab.f != sig_ubicacion2.f or sig_ubi_colab.c != sig_ubicacion2.c)){
								st_result.jugador = sig_ubicacion2;
								st_result.colaborador = sig_ubi_colab;
							}
								
						}
						else {
							st_result.jugador = sig_ubicacion2;
						 	if (st.ultimaOrdenColaborador == act_CLB_TURN_SR)
								st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
						}
					}
				}
		break;

		case actTURN_L:

			if (st.ultimaOrdenColaborador == act_CLB_WALK) {
				sig_ubi_colab = NextCasilla(st.colaborador);
				if (CasillaTransitable(sig_ubi_colab, mapa) and 
					(sig_ubi_colab.f != st_result.jugador.f or sig_ubi_colab.c != st_result.jugador.c)){
					st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
					st_result.colaborador = sig_ubi_colab;
				}
			}
			else {
				
				st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);

				if (st.ultimaOrdenColaborador == act_CLB_TURN_SR)
					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
			}

		break;

		case actTURN_SR:

			if (st.ultimaOrdenColaborador == act_CLB_WALK) {
				sig_ubi_colab = NextCasilla(st.colaborador);
				if (CasillaTransitable(sig_ubi_colab, mapa) and 
					(sig_ubi_colab.f != st_result.jugador.f or sig_ubi_colab.c != st_result.jugador.c)){
					st_result.colaborador = sig_ubi_colab;
					st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
				}
			}
			else {

				st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
				if (st.ultimaOrdenColaborador == act_CLB_TURN_SR)
					st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
			}

		break;

		case act_CLB_TURN_SR:
			if (colaboradorAVista(st.jugador, st.colaborador)){
				st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
			}
		break;

		case act_CLB_WALK:
			sig_ubi_colab = NextCasilla(st.colaborador);
			if (CasillaTransitable(sig_ubi_colab, mapa) and 
				(sig_ubi_colab.f != st_result.jugador.f or sig_ubi_colab.c != st_result.jugador.c) and
				colaboradorAVista(st.jugador, st.colaborador)){
					st_result.colaborador = sig_ubi_colab;
					st_result.ultimaOrdenColaborador = act_CLB_WALK;
			}
		break;

		case act_CLB_STOP:
			if (colaboradorAVista(st.jugador, st.colaborador)){
				st_result.colaborador = st.colaborador;
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			} 
			
		break;

	}
	return st_result;
}

list<Action> ComportamientoJugador::Anchura_nivel1	(const stateN1 &inicio, const ubicacion &final,
													const vector<vector<unsigned char>> &mapa)
{
	// Definiciones de las estructuras de datos
	nodeN1 current_node;
	list<nodeN1> frontier;	// Estados pendientes a explorar
	set<nodeN1> explored;	// estados ya explorados
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.colaborador.f == final.f and
						  current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound){

		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo act_CLBWALK
		nodeN1 colab_child_walk = current_node;
		colab_child_walk.st = apply (act_CLB_WALK, current_node.st, mapa); 
		if (explored.find(colab_child_walk) == explored.end())
		{
			colab_child_walk.secuencia.push_back(act_CLB_WALK);
			frontier.push_back(colab_child_walk);
			if (colab_child_walk.st.colaborador.f == final.f and colab_child_walk.st.colaborador.c == final.c){
				current_node = colab_child_walk;
				SolutionFound = true;
			}
			
		}
		
		if (!SolutionFound){
			// Generar hijo act_CLB_TURN_SR
			nodeN1 colab_child_turnr = current_node;
			colab_child_turnr.st = apply(act_CLB_TURN_SR, current_node.st, mapa);	
			if (explored.find(colab_child_turnr) == explored.end()){
				colab_child_turnr.secuencia.push_back(act_CLB_TURN_SR);	
				frontier.push_back(colab_child_turnr);
			}
		}	

		if (!SolutionFound){
			// Generar hijo act_CLB_STOP
			nodeN1 colab_child_stop = current_node;
			colab_child_stop.st = apply (act_CLB_STOP, current_node.st, mapa);	
			if (explored.find(colab_child_stop) == explored.end()){
				colab_child_stop.secuencia.push_back(act_CLB_STOP);
				frontier.push_back(colab_child_stop);
			}
		}	


		if (!SolutionFound){
			// Generar hijo actWALK
			nodeN1 child_walk = current_node; 
			child_walk.st = apply(actWALK, current_node.st, mapa);
			if (explored.find(child_walk) == explored.end()){
				child_walk.secuencia.push_back(actWALK);
				frontier.push_back(child_walk);
				if (child_walk.st.colaborador.f == final.f and child_walk.st.colaborador.c == final.c){
					current_node = child_walk;
					SolutionFound = true;
				}
			}
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN1 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			if (explored.find(child_run) == explored.end()){
				child_run.secuencia.push_back(actRUN);
				frontier.push_back(child_run);
				if (child_run.st.colaborador.f == final.f and child_run.st.colaborador.c == final.c){
					current_node = child_run;
					SolutionFound = true;
				} 
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			nodeN1 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl) == explored.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
				if (child_turnl.st.colaborador.f == final.f and child_turnl.st.colaborador.c == final.c){
					current_node = child_turnl;
					SolutionFound = true;
				}
			}	
		}

		if (!SolutionFound){
			// Generar hijo actTURN_SR
			nodeN1 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			if (explored.find(child_turnsr) == explored.end()){
				child_turnsr.secuencia.push_back(actTURN_SR);
				frontier.push_back(child_turnsr);
				if (child_turnsr.st.colaborador.f == final.f and child_turnsr.st.colaborador.c == final.c){
					current_node = child_turnsr;
					SolutionFound = true;
				}		
			}		
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
			while (!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if (!frontier.empty())
					current_node = frontier.front();
			}
		}
	}

	
	if (SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan :";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}

//////////////////////// FUNCIONES ESPECIFICAS DEL NIVEL 2 /////////////////////////////////////////

void ComportamientoJugador::VisualizaPlan(const stateN2 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN2 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

nodeN2 ComportamientoJugador::apply(const Action &a, const nodeN2 &n, const vector<vector<unsigned char>> &mapa){
	
	nodeN2 n_result = n;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: 
		sig_ubicacion = NextCasilla(n.st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == n.st.colaborador.f and sig_ubicacion.c == n.st.colaborador.c)){
				n_result.coste += ValorCoste (n_result.st.jugador.f, n_result.st.jugador.c, 
											n_result.st.bikini, n_result.st.zapatillas, 
											actWALK, mapa); 
				n_result.st.jugador = sig_ubicacion;
		}
		break;
	
	case actRUN: 
		sig_ubicacion = NextCasilla(n.st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == n.st.colaborador.f and sig_ubicacion.c == n.st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == n.st.colaborador.f and sig_ubicacion2.c == n.st.colaborador.c)){
						n_result.coste += ValorCoste (n_result.st.jugador.f, n_result.st.jugador.c, 
														n_result.st.bikini, n_result.st.zapatillas, 
														actRUN, mapa); 
						n_result.st.jugador = sig_ubicacion2;
				}
		}
		break;

	case actTURN_L:
		n_result.coste += ValorCoste (n_result.st.jugador.f, n_result.st.jugador.c, 
									n_result.st.bikini, n_result.st.zapatillas, actTURN_L, mapa); 
		n_result.st.jugador.brujula = static_cast<Orientacion>((n_result.st.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		n_result.coste += ValorCoste (n_result.st.jugador.f, n_result.st.jugador.c, 
									n_result.st.bikini, n_result.st.zapatillas, actTURN_SR, mapa); 
		n_result.st.jugador.brujula = static_cast<Orientacion>((n_result.st.jugador.brujula+1)%8);
		break;
	}

	ObtencionElementos(n_result, mapa);	

	return n_result;
}

void ComportamientoJugador::ObtencionElementos (nodeN2 &current_node, const vector<vector<unsigned char>> &mapa){

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K'){
			current_node.st.bikini = true;
			current_node.st.zapatillas = false;
		}

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D'){
			current_node.st.bikini = false;
			current_node.st.zapatillas = true;
		}
}

list<Action> ComportamientoJugador::MinimoCaminoJugador (const stateN2 &inicio, const ubicacion &final,
														const vector<vector<unsigned char>> &mapa){

	nodeN2 current_node;
	priority_queue <nodeN2> frontier;
	set<stateN2> explored; 			 
	list<Action> plan;		
	
	current_node.st = inicio;
	current_node.st.bikini = false;
	current_node.st.zapatillas = false;
	current_node.coste = 0;
	ObtencionElementos(current_node, mapa);	

	frontier.push(current_node);

	while (!frontier.empty() and !(current_node.st.jugador.f == final.f and
								   current_node.st.jugador.c == final.c)){

		frontier.pop();			
		explored.insert(current_node.st);

		// Generar hijo actWALK
		nodeN2 child_walk = current_node; 
		child_walk = apply(actWALK, child_walk, mapa);

		if (explored.find(child_walk.st) == explored.end()){
			child_walk.secuencia.push_back(actWALK);
			frontier.push(child_walk);
		}

		// Generar hijo actRUN
		nodeN2 child_run = current_node;
		child_run = apply(actRUN, child_run, mapa);
		
		if (explored.find(child_run.st) == explored.end()){
			child_run.secuencia.push_back(actRUN);
			frontier.push(child_run);
		}
		
		// Generar hijo actTURN_L
		nodeN2 child_turnl = current_node;
		child_turnl = apply(actTURN_L, child_turnl, mapa);

		if (explored.find(child_turnl.st) == explored.end()){
			child_turnl.secuencia.push_back(actTURN_L);
			frontier.push(child_turnl);
		}
	
		// Generar hijo actTURN_SR
		nodeN2 child_turnsr = current_node;
		child_turnsr = apply(actTURN_SR, child_turnsr, mapa);
		
		if (explored.find(child_turnsr.st) == explored.end()){
			child_turnsr.secuencia.push_back(actTURN_SR);
			frontier.push(child_turnsr);
		}

		if (!frontier.empty() and !(current_node.st.jugador.f == final.f and
								   	current_node.st.jugador.c == final.c)){
			current_node = frontier.top();
			while (!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if (!frontier.empty())
					current_node = frontier.top();
			}
		}
	}
	
	if (current_node.st.jugador.f == final.f and
		current_node.st.jugador.c == final.c){
		plan = current_node.secuencia;
		cout << "Encontrado un plan :";
		PintaPlan(current_node.secuencia);
	}
		
	return plan;
}

//////////////////////// FUNCIONES ESPECIFICAS DEL NIVEL 3 /////////////////////////////////////////

void ComportamientoJugador::VisualizaPlan(const stateN3 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN3 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

nodeN3 ComportamientoJugador::apply(const Action &a, const nodeN3 &n, const vector<vector<unsigned char>> &mapa){
	
	nodeN3 n_result = n;
	ubicacion sig_ubicacion, sig_ubicacion2, sig_ubicacion_colab;
	switch (a)
	{
		case actWALK:
			sig_ubicacion = NextCasilla(n.st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				(sig_ubicacion.f != n.st.colaborador.f or sig_ubicacion.c != n.st.colaborador.c)){
					
					if (n.st.ultimaOrdenColaborador == act_CLB_WALK){
						sig_ubicacion_colab = NextCasilla(n.st.colaborador);
						if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
							(sig_ubicacion_colab.f != sig_ubicacion.f or sig_ubicacion_colab.c != sig_ubicacion.c)){
							
							n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
											  		n.st.zapatillasJugador, actWALK, mapa);

							n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
													n.st.bikiniColaborador, n.st.zapatillasColaborador, 
													act_CLB_WALK, mapa);

							n_result.st.colaborador = sig_ubicacion_colab;
							n_result.st.jugador = sig_ubicacion;
						}
					}
					else {
						
						n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
											  	n.st.zapatillasJugador, actWALK, mapa);
						n_result.st.jugador = sig_ubicacion;
						
						if (n.st.ultimaOrdenColaborador == act_CLB_TURN_SR){
							n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
													n.st.bikiniColaborador, n.st.zapatillasColaborador, 
													act_CLB_TURN_SR, mapa);
							n_result.st.colaborador.brujula = static_cast<Orientacion>((n_result.st.colaborador.brujula+1)%8);
						}

						
					} 
			}
			
		break;
		
		case actRUN:
			sig_ubicacion = NextCasilla(n.st.jugador);
			if (CasillaTransitable(sig_ubicacion, mapa) and 
				(sig_ubicacion.f != n.st.colaborador.f or sig_ubicacion.c != n.st.colaborador.c)){
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if (CasillaTransitable(sig_ubicacion2, mapa) and 
						(sig_ubicacion2.f != n.st.colaborador.f or sig_ubicacion2.c != n.st.colaborador.c)){

						if (n.st.ultimaOrdenColaborador == act_CLB_WALK ){
							sig_ubicacion_colab = NextCasilla(n.st.colaborador);
							if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
								(sig_ubicacion_colab.f != sig_ubicacion2.f or sig_ubicacion_colab.c != sig_ubicacion2.c)){
								
								n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
														n.st.zapatillasJugador, actRUN, mapa);	

								n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
														n.st.bikiniColaborador, n.st.zapatillasColaborador, 
														act_CLB_WALK, mapa);

								n_result.st.colaborador = sig_ubicacion_colab;
								n_result.st.jugador = sig_ubicacion2;
							}
						}
						else {
							
							n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
														n.st.zapatillasJugador, actRUN, mapa);	
							n_result.st.jugador = sig_ubicacion2;
							
							if (n.st.ultimaOrdenColaborador == act_CLB_TURN_SR){
								n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
														n.st.bikiniColaborador, n.st.zapatillasColaborador, 
														act_CLB_TURN_SR, mapa);
								n_result.st.colaborador.brujula = static_cast<Orientacion>((n_result.st.colaborador.brujula+1)%8);
							}

							
						} 
					}
			}
				
		break;

		case actTURN_L:

			if (n.st.ultimaOrdenColaborador == act_CLB_WALK ){
				sig_ubicacion_colab = NextCasilla(n.st.colaborador);
				if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
					(sig_ubicacion_colab.f != n.st.jugador.f or sig_ubicacion_colab.c != n.st.jugador.c)){
					
					n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, 
													n.st.bikiniJugador, 
													n.st.zapatillasJugador, 
													actTURN_L, mapa);

					n_result.st.jugador.brujula = static_cast<Orientacion>((n.st.jugador.brujula+6)%8);
					
					n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_WALK, mapa);

					n_result.st.colaborador = sig_ubicacion_colab;
				
				}
			}
			else {
				
				n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, 
													n.st.bikiniJugador, 
													n.st.zapatillasJugador, 
													actTURN_L, mapa);

				n_result.st.jugador.brujula = static_cast<Orientacion>((n.st.jugador.brujula+6)%8);
				
				if (n.st.ultimaOrdenColaborador == act_CLB_TURN_SR){
					n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_TURN_SR, mapa);
					n_result.st.colaborador.brujula = static_cast<Orientacion>((n.st.colaborador.brujula+1)%8);
				}
			}

		break;

		case actTURN_SR:

			if (n.st.ultimaOrdenColaborador == act_CLB_WALK){
				sig_ubicacion_colab = NextCasilla(n.st.colaborador);
				if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
					(sig_ubicacion_colab.f != n_result.st.jugador.f or sig_ubicacion_colab.c != n_result.st.jugador.c)){

					n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, 
										 	n.st.bikiniJugador, 
											n.st.zapatillasJugador, 
											actTURN_SR, mapa);

					n_result.st.jugador.brujula = static_cast<Orientacion>((n.st.jugador.brujula+1)%8);
					
					n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_WALK, mapa);

					n_result.st.colaborador = sig_ubicacion_colab;
				
				}
			}
			else { 

				n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, 
										 	n.st.bikiniJugador, 
											n.st.zapatillasJugador, 
											actTURN_SR, mapa);

				n_result.st.jugador.brujula = static_cast<Orientacion>((n.st.jugador.brujula+1)%8);

				if (n.st.ultimaOrdenColaborador == act_CLB_TURN_SR){
					n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_TURN_SR, mapa);
					n_result.st.colaborador.brujula = static_cast<Orientacion>((n.st.colaborador.brujula+1)%8);
				}
			}

		break;

		case actIDLE:

			if (n.st.ultimaOrdenColaborador == act_CLB_WALK){
				sig_ubicacion_colab = NextCasilla(n.st.colaborador);
				if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
					(sig_ubicacion_colab.f != n.st.jugador.f or sig_ubicacion_colab.c != n.st.jugador.c)){

					n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_WALK, mapa);
					n_result.st.colaborador = sig_ubicacion_colab;
				
				}
			}
			else { 

				if (n.st.ultimaOrdenColaborador == act_CLB_TURN_SR){
					n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_TURN_SR, mapa);
					n_result.st.colaborador.brujula = static_cast<Orientacion>((n.st.colaborador.brujula+1)%8);
				}
			}

		break;

		case act_CLB_TURN_SR:
			if (colaboradorAVista(n.st.jugador, n.st.colaborador)){
				n_result.g +=  ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
										n.st.bikiniColaborador, n.st.zapatillasColaborador, 
										act_CLB_TURN_SR, mapa);
				n_result.st.colaborador.brujula = static_cast<Orientacion>((n.st.colaborador.brujula+1)%8);
				n_result.st.ultimaOrdenColaborador = act_CLB_TURN_SR;
			}
		break;

		case act_CLB_WALK:
			sig_ubicacion_colab = NextCasilla(n.st.colaborador);
			if (CasillaTransitable(sig_ubicacion_colab, mapa) and 
				(sig_ubicacion_colab.f != n.st.jugador.f or sig_ubicacion_colab.c != n.st.jugador.c) and
				colaboradorAVista(n.st.jugador,n.st.colaborador)){
					n_result.g += ValorCoste (n.st.colaborador.f, n.st.colaborador.c, 
											n.st.bikiniColaborador, n.st.zapatillasColaborador, 
											act_CLB_WALK, mapa);
					n_result.st.colaborador = sig_ubicacion_colab;
					n_result.st.ultimaOrdenColaborador = act_CLB_WALK;
			}
		break;

		case act_CLB_STOP:
			if (colaboradorAVista(n.st.jugador, n.st.colaborador)){
				n_result.st.colaborador = n.st.colaborador;
				n_result.st.ultimaOrdenColaborador = act_CLB_STOP;
			} 
			
		break;
	}

	n_result.h = CosteEstimado(n_result.st.colaborador, goal);
	n_result.f = n_result.g + n_result.h;
	ObtencionElementosJugadores(n_result, mapa);
	
	return n_result;
}

void ComportamientoJugador::ObtencionElementosJugadores (nodeN3 &current_node, const vector<vector<unsigned char>> &mapa){

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K'){
			current_node.st.bikiniJugador = true;
			current_node.st.zapatillasJugador = false;
		}

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D'){
			current_node.st.bikiniJugador = false;
			current_node.st.zapatillasJugador = true;
		}

		if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'K'){
			current_node.st.bikiniColaborador = true;
			current_node.st.zapatillasColaborador = false;
		}

		if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'D'){
			current_node.st.bikiniColaborador = false;
			current_node.st.zapatillasColaborador = true;
		}
}

int ComportamientoJugador::CosteEstimado (ubicacion ini, ubicacion fin){
	return max(abs(ini.c - fin.c), abs (ini.f - fin.f));
}

list<Action> ComportamientoJugador::Aestrella (const nodeN3 &inicio, const ubicacion &final,
											   const vector<vector<unsigned char>> &mapa){

	nodeN3 current_node;
	priority_queue <nodeN3> frontier; 
	set<stateN3> explored; 			 
	list<Action> plan;		
	
	current_node = inicio;

	current_node.st.bikiniColaborador = false;
	current_node.st.zapatillasColaborador = false;
	current_node.st.bikiniJugador = false;
	current_node.st.zapatillasJugador = false;
	ObtencionElementosJugadores(current_node, mapa);

	frontier.push(current_node);

	while (!(current_node.st.colaborador.f == final.f and
			current_node.st.colaborador.c == final.c) and !frontier.empty()){

		frontier.pop();
		
		explored.insert(current_node.st);

		// Generar hijo act_CLBWALK
		nodeN3 colab_child_walk = current_node;
		colab_child_walk = apply(act_CLB_WALK, current_node, mapa);

		if (explored.find(colab_child_walk.st) == explored.end()){
			colab_child_walk.secuencia.push_back(act_CLB_WALK);
			frontier.push(colab_child_walk);
		}

		// Generar hijo act_CLB_TURN_SR
		nodeN3 colab_child_turnr = current_node;
		colab_child_turnr = apply(act_CLB_TURN_SR, current_node, mapa);	

		if (explored.find(colab_child_turnr.st) == explored.end()){
			colab_child_turnr.secuencia.push_back(act_CLB_TURN_SR);	
			frontier.push(colab_child_turnr);
		}

		// Generar hijo act_CLB_STOP
		nodeN3 colab_child_stop = current_node;	
		colab_child_stop = apply (act_CLB_STOP, current_node, mapa);	

		if (explored.find(colab_child_stop.st) == explored.end()){
			colab_child_stop.secuencia.push_back(act_CLB_STOP);
			frontier.push(colab_child_stop);
		}

		
		// Generar hijo actTURN_L
		nodeN3 child_turnl = current_node;
		child_turnl = apply(actTURN_L, current_node, mapa);

		if (explored.find(child_turnl.st) == explored.end()){
			child_turnl.secuencia.push_back(actTURN_L);
			frontier.push(child_turnl);
		}	
	

		// Generar hijo actTURN_SR
		nodeN3 child_turnsr = current_node;
		child_turnsr = apply(actTURN_SR, current_node, mapa);

		if (explored.find(child_turnsr.st) == explored.end()){
			child_turnsr.secuencia.push_back(actTURN_SR);
			frontier.push(child_turnsr);	
		}

		// Generar hijo actWALK
		nodeN3 child_walk = current_node; 
		child_walk = apply(actWALK, current_node, mapa);

		if (explored.find(child_walk.st) == explored.end()){
			child_walk.secuencia.push_back(actWALK);
			frontier.push(child_walk);
		}

		// Generar hijo actRUN
		nodeN3 child_run = current_node;
		child_run = apply(actRUN, current_node, mapa);

		if (explored.find(child_run.st) == explored.end()){
			child_run.secuencia.push_back(actRUN);
			frontier.push(child_run);
		}

		// Generar hijo actIDLE
		nodeN3 child_idle = current_node;
		child_idle = apply(actIDLE, current_node, mapa);

		if (explored.find(child_idle.st) == explored.end()){
			child_idle.secuencia.push_back(actIDLE);
			frontier.push(child_idle);
		}
	
		if (!frontier.empty() and !(current_node.st.colaborador.f == final.f and
								   	current_node.st.colaborador.c == final.c)){
			current_node = frontier.top();
			while (!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if (!frontier.empty())
					current_node = frontier.top();
			}
		}
	}

	if ((current_node.st.colaborador.f == final.f and
		current_node.st.colaborador.c == final.c)){
		plan = current_node.secuencia;
		cout << "Encontrado un plan :";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}

//////////////////////// FUNCIONES ESPECIFICAS DEL NIVEL 4 /////////////////////////////////////////

// Comprueba si en la visión del jugador hay casilla de batería
// y guarda dicho indice del vector
void ComportamientoJugador:: Bateria(Sensores sensores){

	for (int i=0; i< sensores.terreno.size(); i++){
		if (sensores.terreno[i] == 'X'){
			indiceBateria = i;
			hayCasillaBateria = true;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const stateN4 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN4 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
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

// Pinta el terreno a vista del jugador y guarda la posición de la casilla
// de batería si la hubiera
void ComportamientoJugador::PintarMapa (Sensores sensores, 
										vector<vector<unsigned char>> &matriz)
{

	int filAct, colAct;
	Orientacion orientacion;
	vector <unsigned char> terreno = sensores.terreno;

	filAct = c_state_n4.st.jugador.f;
	colAct = c_state_n4.st.jugador.c;
	orientacion = c_state_n4.st.jugador.brujula;

	Bateria(sensores);

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
			matriz[filAct-2][colAct] = terreno[6];
			matriz[filAct-3][colAct-1] = terreno[11];
			matriz[filAct-3][colAct] = terreno[12];
			matriz[filAct-3][colAct+1] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct-1; bateria.c = colAct-1;	break;
				case 2: bateria.f = filAct-1; bateria.c = colAct; break;
				case 3: bateria.f = filAct-1; bateria.c = colAct+1; break;
				case 4: bateria.f = filAct-2; bateria.c = colAct-2; break;
				case 5: bateria.f = filAct-2; bateria.c=colAct-1; break;
				case 6: bateria.f = filAct-2; bateria.c=colAct; break;
				case 7: bateria.f = filAct-2; bateria.c=colAct+1; break;
				case 8: bateria.f = filAct-2; bateria.c=colAct+2; break;
				case 9: bateria.f = filAct-3; bateria.c=colAct-3; break;
				case 10: bateria.f = filAct-3; bateria.c=colAct-2; break;
				case 11: bateria.f = filAct-3; bateria.c=colAct-1; break;
				case 12: bateria.f = filAct-3; bateria.c=colAct; break;
				case 13: bateria.f = filAct-3; bateria.c=colAct+1; break;
				case 14: bateria.f = filAct-3; bateria.c=colAct+2; break;
				case 15: bateria.f = filAct-3; bateria.c=colAct+3; break;
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
			matriz[filAct-2][colAct+2] = terreno[6];
			matriz[filAct-3][colAct+2] = terreno[11];
			matriz[filAct-3][colAct+3] = terreno[12];
			matriz[filAct-2][colAct+3] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct-1; bateria.c = colAct; break;
				case 2: bateria.f = filAct-1; bateria.c = colAct+1; break;
				case 3: bateria.f = filAct; bateria.c = colAct+1; break;
				case 4: bateria.f = filAct-2; bateria.c = colAct; break;
				case 5: bateria.f = filAct-2; bateria.c=colAct+1; break;
				case 6: bateria.f = filAct-2; bateria.c=colAct+2; break;
				case 7: bateria.f = filAct-1; bateria.c=colAct+2; break;
				case 8: bateria.f = filAct; bateria.c=colAct+2; break;
				case 9: bateria.f = filAct-3; bateria.c=colAct; break;
				case 10: bateria.f = filAct-3; bateria.c=colAct+1; break;
				case 11: bateria.f = filAct-3; bateria.c=colAct+2; break;
				case 12: bateria.f = filAct-3; bateria.c=colAct+3; break;
				case 13: bateria.f = filAct-2; bateria.c=colAct+3; break;
				case 14: bateria.f = filAct-1; bateria.c=colAct; break;
				case 15: bateria.f = filAct; bateria.c=colAct+1; break;
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
			matriz[filAct][colAct+2] = terreno[6];
			matriz[filAct-1][colAct+3] = terreno[11];
			matriz[filAct][colAct+3] = terreno[12];
			matriz[filAct+1][colAct+3] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct-1; bateria.c = colAct+1; break;
				case 2: bateria.f = filAct; bateria.c = colAct+1; break;
				case 3: bateria.f = filAct+1; bateria.c = colAct+1; break;
				case 4: bateria.f = filAct-2; bateria.c = colAct+2; break;
				case 5: bateria.f = filAct-1; bateria.c=colAct+2; break;
				case 6: bateria.f = filAct; bateria.c=colAct+2; break;
				case 7: bateria.f = filAct+1; bateria.c=colAct+2; break;
				case 8: bateria.f = filAct+2; bateria.c=colAct+2; break;
				case 9: bateria.f = filAct-3; bateria.c=colAct+3; break;
				case 10: bateria.f = filAct-2; bateria.c=colAct+3; break;
				case 11: bateria.f = filAct-1; bateria.c=colAct+3; break;
				case 12: bateria.f = filAct; bateria.c=colAct+3; break;
				case 13: bateria.f = filAct+1; bateria.c=colAct+3; break;
				case 14: bateria.f = filAct+2; bateria.c=colAct+3; break;
				case 15: bateria.f = filAct+3; bateria.c=colAct+3; break;
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
			matriz[filAct+2][colAct+2] = terreno[6];
			matriz[filAct+2][colAct+3] = terreno[11];
			matriz[filAct+3][colAct+3] = terreno[12];
			matriz[filAct+3][colAct+2] = terreno[13];
			
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct; bateria.c = colAct+1; break;
				case 2: bateria.f = filAct+1; bateria.c = colAct+1; break;
				case 3: bateria.f = filAct+1; bateria.c = colAct; break;
				case 4: bateria.f = filAct; bateria.c = colAct+2; break;
				case 5: bateria.f = filAct+1; bateria.c=colAct+2; break;
				case 6: bateria.f = filAct+2; bateria.c=colAct+2; break;
				case 7: bateria.f = filAct+2; bateria.c=colAct+1; break;
				case 8: bateria.f = filAct+2; bateria.c=colAct; break;
				case 9: bateria.f = filAct; bateria.c=colAct+3; break;
				case 10: bateria.f = filAct+1; bateria.c=colAct+3; break;
				case 11: bateria.f = filAct+2; bateria.c=colAct+3; break;
				case 12: bateria.f = filAct+3; bateria.c=colAct+3; break;
				case 13: bateria.f = filAct+3; bateria.c=colAct+2; break;
				case 14: bateria.f = filAct+3; bateria.c=colAct+1; break;
				case 15: bateria.f = filAct+3; bateria.c=colAct; break;
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
			matriz[filAct+2][colAct] = terreno[6];
			matriz[filAct+3][colAct+1] = terreno[11];
			matriz[filAct+3][colAct] = terreno[12];
			matriz[filAct+3][colAct-1] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct+1; bateria.c = colAct+1; break;
				case 2: bateria.f = filAct+1; bateria.c = colAct; break;
				case 3: bateria.f = filAct+1; bateria.c = colAct-1; break;
				case 4: bateria.f = filAct+2; bateria.c = colAct+2; break;
				case 5: bateria.f = filAct+2; bateria.c=colAct+1; break;
				case 6: bateria.f = filAct+2; bateria.c=colAct; break;
				case 7: bateria.f = filAct+2; bateria.c=colAct-1; break;
				case 8: bateria.f = filAct+2; bateria.c=colAct-2; break;
				case 9: bateria.f = filAct+3; bateria.c=colAct+3; break;
				case 10: bateria.f = filAct+3; bateria.c=colAct+2; break;
				case 11: bateria.f = filAct+3; bateria.c=colAct+1; break;
				case 12: bateria.f = filAct+3; bateria.c=colAct; break;
				case 13: bateria.f = filAct+3; bateria.c=colAct-1; break;
				case 14: bateria.f = filAct+3; bateria.c=colAct-2; break;
				case 15: bateria.f = filAct+3; bateria.c=colAct-3; break;
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
			matriz[filAct+2][colAct-2] = terreno[6];
			matriz[filAct+3][colAct-2] = terreno[11];
			matriz[filAct+3][colAct-3] = terreno[12];
			matriz[filAct+2][colAct-3] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct+1; bateria.c = colAct; break;
				case 2: bateria.f = filAct+1; bateria.c = colAct-1; break;
				case 3: bateria.f = filAct; bateria.c = colAct-1; break;
				case 4: bateria.f = filAct+2; bateria.c = colAct; break;
				case 5: bateria.f = filAct+2; bateria.c=colAct-1; break;
				case 6: bateria.f = filAct+2; bateria.c=colAct-2; break;
				case 7: bateria.f = filAct+1; bateria.c=colAct-2; break;
				case 8: bateria.f = filAct; bateria.c=colAct-2; break;
				case 9: bateria.f = filAct+3; bateria.c=colAct; break;
				case 10: bateria.f = filAct+3; bateria.c=colAct-1; break;
				case 11: bateria.f = filAct+3; bateria.c=colAct-2; break;
				case 12: bateria.f = filAct+3; bateria.c=colAct-3; break;
				case 13: bateria.f = filAct+2; bateria.c=colAct-3; break;
				case 14: bateria.f = filAct+1; bateria.c=colAct-3; break;
				case 15: bateria.f = filAct; bateria.c=colAct-3; break;
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
			matriz[filAct][colAct-2] = terreno[6];
			matriz[filAct+1][colAct-3] = terreno[11];
			matriz[filAct][colAct-3] = terreno[12];
			matriz[filAct-1][colAct-3] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct+1; bateria.c = colAct-1; break;
				case 2: bateria.f = filAct; bateria.c = colAct-1; break;
				case 3: bateria.f = filAct-1; bateria.c = colAct-1; break;
				case 4: bateria.f = filAct+2; bateria.c = colAct-2; break;
				case 5: bateria.f = filAct+1; bateria.c=colAct-2; break;
				case 6: bateria.f = filAct; bateria.c=colAct-2; break;
				case 7: bateria.f = filAct-1; bateria.c=colAct-2; break;
				case 8: bateria.f = filAct-2; bateria.c=colAct-2; break;
				case 9: bateria.f = filAct+3; bateria.c=colAct-3; break;
				case 10: bateria.f = filAct+2; bateria.c=colAct-3; break;
				case 11: bateria.f = filAct+1; bateria.c=colAct-3; break;
				case 12: bateria.f = filAct; bateria.c=colAct-3; break;
				case 13: bateria.f = filAct-3; bateria.c=colAct-3; break;
				case 14: bateria.f = filAct-2; bateria.c=colAct-3; break;
				case 15: bateria.f = filAct-1; bateria.c=colAct-3; break;
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
			matriz[filAct-2][colAct-2] = terreno[6];
			matriz[filAct-2][colAct-3] = terreno[11];
			matriz[filAct-3][colAct-3] = terreno[12];
			matriz[filAct-3][colAct-2] = terreno[13];
			switch (indiceBateria)
			{
				case 0: bateria.f = filAct; bateria.c = colAct; break;
				case 1:	bateria.f = filAct; bateria.c = colAct-1; break;
				case 2: bateria.f = filAct-1; bateria.c = colAct-1; break;
				case 3: bateria.f = filAct-1; bateria.c = colAct; break;
				case 4: bateria.f = filAct; bateria.c = colAct-2; break;
				case 5: bateria.f = filAct-1; bateria.c=colAct-2; break;
				case 6: bateria.f = filAct-2; bateria.c=colAct-2; break;
				case 7: bateria.f = filAct-2; bateria.c=colAct-1; break;
				case 8: bateria.f = filAct-2; bateria.c=colAct; break;
				case 9: bateria.f = filAct; bateria.c=colAct-3; break;
				case 10: bateria.f = filAct-1; bateria.c=colAct-3; break;
				case 11: bateria.f = filAct-2; bateria.c=colAct-3; break;
				case 12: bateria.f = filAct-3; bateria.c=colAct-3; break;
				case 13: bateria.f = filAct-3; bateria.c=colAct-2; break;
				case 14: bateria.f = filAct-3; bateria.c=colAct-1; break;
				case 15: bateria.f = filAct-3; bateria.c=colAct; break;
			}
		break;
	}
	
}

void ComportamientoJugador::ObtencionElementosN4 (nodeN4 &current_node, const vector<vector<unsigned char>> &mapa){

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K'){
			current_node.st.bikiniJugador = true;
			current_node.st.zapatillasJugador = false;
		}

		if (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D'){
			current_node.st.bikiniJugador = false;
			current_node.st.zapatillasJugador = true;
		}

		if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'K'){
			current_node.st.bikiniColaborador = true;
			current_node.st.zapatillasColaborador = false;
		}

		if (mapa[current_node.st.colaborador.f][current_node.st.colaborador.c] == 'D'){
			current_node.st.bikiniColaborador = false;
			current_node.st.zapatillasColaborador = true;
		}

}

nodeN4 ComportamientoJugador::apply(const Action &a, const nodeN4 &n, const vector<vector<unsigned char>> &mapa){
	
	nodeN4 n_result = n;
	ubicacion sig_ubicacion, sig_ubicacion2, sig_ubicacion_colab;

	switch (a)
	{
		case actWALK: 

			// Si la casilla siguiente no se conoce o si es conocida no debe ser muro o precipicio y 
			// no está el colaborador
			sig_ubicacion = NextCasilla(n.st.jugador);
			if ((CasillaTransitable(sig_ubicacion, mapa) or mapa[sig_ubicacion.f][sig_ubicacion.c]  == '?') and 
				(sig_ubicacion.f != n.st.colaborador.f or sig_ubicacion.c != n.st.colaborador.c)
				and (sig_ubicacion.f > 0 and sig_ubicacion.f < mapa.size()) 
				and (sig_ubicacion.c > 0 and sig_ubicacion.c < mapa.size())) {

					n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
												n.st.zapatillasJugador, actWALK, mapa);
					n_result.st.jugador = sig_ubicacion;
			}
		break;
		
		case actRUN: 
			sig_ubicacion = NextCasilla(n.st.jugador);
			if ((CasillaTransitable(sig_ubicacion, mapa) or mapa[sig_ubicacion.f][sig_ubicacion.c]  == '?') and 
				(sig_ubicacion.f != n.st.colaborador.f and sig_ubicacion.c != n.st.colaborador.c)){
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					// Si la casilla siguiente no se conoce o si es conocida no debe ser muro o precipicio y 
					// no está el colaborador
					if ((CasillaTransitable(sig_ubicacion2, mapa) or mapa[sig_ubicacion2.f][sig_ubicacion2.c]  == '?')
						and (sig_ubicacion2.f != n.st.colaborador.f or sig_ubicacion2.c != n.st.colaborador.c)
						and (sig_ubicacion2.f > 0 and sig_ubicacion2.f < mapa.size()) 
						and (sig_ubicacion2.c > 0 and sig_ubicacion2.c < mapa.size())){

						n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
													n.st.zapatillasJugador, actRUN, mapa);
						n_result.st.jugador = sig_ubicacion2;
					}				
			}

		break;

		case actTURN_L:
			n_result.g += ValorCoste (n.st.jugador.f, n.st.jugador.c, n.st.bikiniJugador, 
										n.st.zapatillasJugador, actTURN_L, mapa);
			n_result.st.jugador.brujula = static_cast<Orientacion>((n.st.jugador.brujula+6)%8);
		break;

		case actTURN_SR:
			n_result.g += ValorCoste (n_result.st.jugador.f, n_result.st.jugador.c, 
									n_result.st.bikiniJugador, n_result.st.zapatillasJugador, 
									actTURN_SR, mapa); 
			n_result.st.jugador.brujula = static_cast<Orientacion>((n_result.st.jugador.brujula+1)%8);
		break;
	}

	ObtencionElementosN4(n_result, mapa);
	n_result.f = n_result.g + n_result.h;

	return n_result;
}

list<Action> ComportamientoJugador::MinimoCaminoJugador (const stateN4 &inicio, const ubicacion &final,
														const vector<vector<unsigned char>> &mapa){

	nodeN4 current_node;
	priority_queue <nodeN4> frontier;
	set<stateN4> explored; 			 
	list<Action> plan;		
	
	current_node.st = inicio;
	current_node.f = 0;
	ObtencionElementosN4(current_node, mapa);	

	frontier.push(current_node);

	while (!frontier.empty() and !(current_node.st.jugador.f == final.f and
								   current_node.st.jugador.c == final.c)){

		frontier.pop();			
		explored.insert(current_node.st);

		// Generar hijo actWALK
		nodeN4 child_walk = current_node; 
		child_walk = apply(actWALK, child_walk, mapa);

		if (explored.find(child_walk.st) == explored.end()){
			child_walk.secuencia.push_back(actWALK);
			frontier.push(child_walk);
		}

		// Generar hijo actRUN
		nodeN4 child_run = current_node;
		child_run = apply(actRUN, child_run, mapa);
		
		if (explored.find(child_run.st) == explored.end()){
			child_run.secuencia.push_back(actRUN);
			frontier.push(child_run);
		}
		
		// Generar hijo actTURN_L
		nodeN4 child_turnl = current_node;
		child_turnl = apply(actTURN_L, child_turnl, mapa);

		if (explored.find(child_turnl.st) == explored.end()){
			child_turnl.secuencia.push_back(actTURN_L);
			frontier.push(child_turnl);
		}
	
		// Generar hijo actTURN_SR
		nodeN4 child_turnsr = current_node;
		child_turnsr = apply(actTURN_SR, child_turnsr, mapa);
		
		if (explored.find(child_turnsr.st) == explored.end()){
			child_turnsr.secuencia.push_back(actTURN_SR);
			frontier.push(child_turnsr);
		}

		if (!frontier.empty() and !(current_node.st.jugador.f == final.f and
								   	current_node.st.jugador.c == final.c)){
			current_node = frontier.top();
			while (!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if (!frontier.empty())
					current_node = frontier.top();
			}
		}
	}
	
	if (current_node.st.jugador.f == final.f and
		current_node.st.jugador.c == final.c){
		plan = current_node.secuencia;
		cout << "Encontrado un plan :";
		PintaPlan(current_node.secuencia);
	}
		
	return plan;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	if (sensores.nivel != 4){
		if (!hayPlan){

			cout << "Calculamos un nuevo plan\n";
			
			switch (sensores.nivel){

				case 0: 
					c_state.st.jugador.f = sensores.posF;
					c_state.st.jugador.c = sensores.posC;
					c_state.st.jugador.brujula = sensores.sentido;
					c_state.st.colaborador.f = sensores.CLBposF;
					c_state.st.colaborador.c = sensores.CLBposC;
					c_state.st.colaborador.brujula = sensores.CLBsentido;
					goal.f = sensores.destinoF;
					goal.c = sensores.destinoC; 
					plan = Anchura_nivel0 (c_state.st, goal, mapaResultado); 
					if (plan.size() > 0){
						VisualizaPlan(c_state.st, plan);
						hayPlan = true;
					}
				break;

				case 1: 

					c_state_n1.st.jugador.f = sensores.posF;
					c_state_n1.st.jugador.c = sensores.posC;
					c_state_n1.st.jugador.brujula = sensores.sentido;
					c_state_n1.st.colaborador.f = sensores.CLBposF;
					c_state_n1.st.colaborador.c = sensores.CLBposC;
					c_state_n1.st.colaborador.brujula = sensores.CLBsentido;
					c_state_n1.st.ultimaOrdenColaborador = act_CLB_STOP;
					goal.f = sensores.destinoF;
					goal.c = sensores.destinoC;
					plan = Anchura_nivel1(c_state_n1.st, goal, mapaResultado);
					if (plan.size() > 0){
						VisualizaPlan(c_state_n1.st, plan);
						hayPlan = true;
					}
				break;

				case 2: 
					c_state_n2.st.jugador.f = sensores.posF;
					c_state_n2.st.jugador.c = sensores.posC;
					c_state_n2.st.jugador.brujula = sensores.sentido;
					c_state_n2.st.colaborador.f = sensores.CLBposF;
					c_state_n2.st.colaborador.c = sensores.CLBposC;
					c_state_n2.st.colaborador.brujula = sensores.CLBsentido;
					c_state_n2.coste = 0;
					goal.f = sensores.destinoF;
					goal.c = sensores.destinoC;
					plan = MinimoCaminoJugador(c_state_n2.st, goal, mapaResultado);
					if (plan.size() > 0){
						VisualizaPlan(c_state_n2.st, plan);
						hayPlan = true;
					}
				break;

				case 3: 
					c_state_n3.st.jugador.f = sensores.posF;
					c_state_n3.st.jugador.c = sensores.posC;
					c_state_n3.st.jugador.brujula = sensores.sentido;
					c_state_n3.st.colaborador.f = sensores.CLBposF;
					c_state_n3.st.colaborador.c = sensores.CLBposC;
					c_state_n3.st.colaborador.brujula = sensores.CLBsentido;
					c_state_n3.st.ultimaOrdenColaborador = act_CLB_STOP;
					c_state_n3.f = 0;
					c_state_n3.g = 0;
					c_state_n3.h = 0;
					goal.f = sensores.destinoF;
					goal.c = sensores.destinoC;
					plan = Aestrella(c_state_n3, goal, mapaResultado);
					if (plan.size() > 0){
						VisualizaPlan(c_state_n3.st, plan);
						hayPlan = true;
					}
				break;

			}
		}
		
		if (hayPlan and plan.size() > 0){
			accion = plan.front();
			plan.pop_front();
		}

		if (hayPlan and plan.size() == 0){
			cout << "Se completó el plan\n";
			hayPlan = false;
		}

	}

	// Nivel 4
	else {

		// Controla cuando es el segundo movimiento y 
		// ya está situado
		if (!primerMovimiento and !segundoMovimiento) {
			// tomamos los valores iniciales
			situado = true;
			c_state_n4.st.jugador.f = sensores.posF;
			c_state_n4.st.jugador.c = sensores.posC;
			c_state_n4.st.jugador.brujula = sensores.sentido;
			c_state_n4.st.colaborador.f = sensores.CLBposF;
			c_state_n4.st.colaborador.c = sensores.CLBposC;
			c_state_n4.st.colaborador.brujula = sensores.CLBsentido;
			c_state_n4.st.ultimaOrdenColaborador = act_CLB_STOP;
			segundoMovimiento = true;
		}

		// Controla cuando es el primer mov. para situarse
		if (primerMovimiento){
			accion = actWHEREIS;
			primerMovimiento = false;
			pintarPrecipicios();
		}

		// Si no hay plan pero estamos bien situados
		if (!hayPlan and situado and !planBateria){

			c_state_n4.f = 0;
			c_state_n4.g = 0;
			c_state_n4.h = 0;
			
			switch (ult_accion) {
				case actWALK:
					switch(c_state_n4.st.jugador.brujula){
						case norte: 
							c_state_n4.st.jugador.f--;
							break;
						case noreste:
							c_state_n4.st.jugador.f--; 
							c_state_n4.st.jugador.c++; 
							break;
						case este:
							c_state_n4.st.jugador.c++;
							break;
						case sureste:
							c_state_n4.st.jugador.c++;
							c_state_n4.st.jugador.f++;
							break;
						case sur:
							c_state_n4.st.jugador.f++;
							break;
						case suroeste:
							c_state_n4.st.jugador.f++;
							c_state_n4.st.jugador.c--;
							break;
						case oeste:
							c_state_n4.st.jugador.c--;
							break;
						case noroeste:
							c_state_n4.st.jugador.c--;
							c_state_n4.st.jugador.f--;
							break;
					}

				
				break; // Fin del caso actWALK

				case actRUN:
					switch(c_state_n4.st.jugador.brujula){
						case norte: 
							c_state_n4.st.jugador.f -= 2;
							break;
						case noreste:
							c_state_n4.st.jugador.f -= 2;
							c_state_n4.st.jugador.c += 2; 
							break;
						case este:
							c_state_n4.st.jugador.c += 2; 
							break;
						case sureste:
							c_state_n4.st.jugador.c += 2; 
							c_state_n4.st.jugador.f += 2;
							break;
						case sur:
							c_state_n4.st.jugador.f += 2;
							break;
						case suroeste:
							c_state_n4.st.jugador.f += 2;
							c_state_n4.st.jugador.c -= 2; 
							break;
						case oeste:
							c_state_n4.st.jugador.c -= 2; 
							break;
						case noroeste:
							c_state_n4.st.jugador.c -= 2; 
							c_state_n4.st.jugador.f -= 2;
							break;
					}	
				break; // Fin del caso actRUN

				case actTURN_L:
					c_state_n4.st.jugador.brujula = static_cast<Orientacion>((c_state_n4.st.jugador.brujula+6)%8); 
				break; // Fin del caso actTURN_L

				case actTURN_SR:
					c_state_n4.st.jugador.brujula = static_cast<Orientacion>((c_state_n4.st.jugador.brujula+1)%8);
				break;
			}

			ult_accion_modificada = true;

			// Si estamos bajos de batería pero tenemos vida y además hemos localizado
			// en algún momento una casilla de batería entonces nuestro objetivo
			// es que el jugador llegue a dicha casilla
			// (no se porqué da fallo, luego lo comento)

			// if (sensores.bateria < 1000 and hayCasillaBateria
			// 	and sensores.vida > 1200){
			// 	goal.f = bateria.f;
			// 	goal.c = bateria.c;
			// 	planBateria = true;
			// } else {
				
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;
			
			
			cout << "Calculamos un nuevo plan\n";
			plan = MinimoCaminoJugador(c_state_n4.st, goal, mapaResultado);

			// Si hay plan se visualiza
			if (plan.size() > 0){
				VisualizaPlan(c_state_n4.st, plan);
				hayPlan = true;
			}

			// Como estamos ubicados podemos pintar
			puedoPintar = true;
		}

		// modifica posJ para poder pintar
	
		if (!ult_accion_modificada){ 
		switch (ult_accion) {
		case actWALK:
			switch(c_state_n4.st.jugador.brujula){
				case norte: 
					c_state_n4.st.jugador.f--;
					break;
				case noreste:
					c_state_n4.st.jugador.f--; 
					c_state_n4.st.jugador.c++; 
					break;
				case este:
					c_state_n4.st.jugador.c++;
					break;
				case sureste:
					c_state_n4.st.jugador.c++;
					c_state_n4.st.jugador.f++;
					break;
				case sur:
					c_state_n4.st.jugador.f++;
					break;
				case suroeste:
					c_state_n4.st.jugador.f++;
					c_state_n4.st.jugador.c--;
					break;
				case oeste:
					c_state_n4.st.jugador.c--;
					break;
				case noroeste:
					c_state_n4.st.jugador.c--;
					c_state_n4.st.jugador.f--;
					break;
			}

		
		break; // Fin del caso actWALK

		case actRUN:
			switch(c_state_n4.st.jugador.brujula){
				case norte: 
					c_state_n4.st.jugador.f -= 2;
					break;
				case noreste:
					c_state_n4.st.jugador.f -= 2;
					c_state_n4.st.jugador.c += 2; 
					break;
				case este:
					c_state_n4.st.jugador.c += 2; 
					break;
				case sureste:
					c_state_n4.st.jugador.c += 2; 
					c_state_n4.st.jugador.f += 2;
					break;
				case sur:
					c_state_n4.st.jugador.f += 2;
					break;
				case suroeste:
					c_state_n4.st.jugador.f += 2;
					c_state_n4.st.jugador.c -= 2; 
					break;
				case oeste:
					c_state_n4.st.jugador.c -= 2; 
					break;
				case noroeste:
					c_state_n4.st.jugador.c -= 2; 
					c_state_n4.st.jugador.f -= 2;
					break;
			}	
		break; // Fin del caso actRUN

		case actTURN_L:
			c_state_n4.st.jugador.brujula = static_cast<Orientacion>((c_state_n4.st.jugador.brujula+6)%8); 
		break; // Fin del caso actTURN_L

		case actTURN_SR:
			c_state_n4.st.jugador.brujula = static_cast<Orientacion>((c_state_n4.st.jugador.brujula+1)%8);
		break;
	}
		}
		// Si estoy ubicado puedo pintar lo que veo
		if (puedoPintar) PintarMapa(sensores, mapaResultado);
						
		// Nos mantenemos en la casilla batería hasta rellenarla o hasta que quede poca vida
		if (sensores.bateria < 2900 and c_state_n4.st.jugador.f == goal.f and
			c_state_n4.st.jugador.c == goal.c and sensores.vida > 100 and planBateria)
		{
			accion = actIDLE;
			ult_accion_modificada = false;
		}
		else planBateria = false;
		
		// Si hay plan
		if (hayPlan and plan.size() > 0){
			
			// Tomamos la siguiente accion
			accion = plan.front();
			plan.pop_front();
			ult_accion_modificada = false;

			if ((sensores.terreno[2] == 'M' and accion == actWALK)
				or ((sensores.terreno[6] == 'M' or sensores.terreno[2] == 'M') and accion == actRUN)
				or (sensores.terreno[2] == 'P' and accion == actWALK)
				or ((sensores.terreno[6] == 'P' or sensores.terreno[2] == 'P') and accion == actRUN
				or sensores.colision)){
					plan.empty();
					accion = actIDLE;
					hayPlan = false;
					puedoPintar = false;
			}
		}

		// Si se acaba el plan, replanificamos
		if (hayPlan and plan.size() == 0){
			cout << "Se completó el plan\n";
			hayPlan = false;
			puedoPintar = false;
		}

		
	}

	ult_accion = accion;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}