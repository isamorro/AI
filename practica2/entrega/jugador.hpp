#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <queue>

// NIVEL 0

struct stateN0 {

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x) const {
    if  (jugador == x.jugador 
        and colaborador.f == x.colaborador.f
        and colaborador.c == x.colaborador.c)
        return true;
    else return false;
  }

};

struct nodeN0 {

	stateN0 st;
	list<Action> secuencia;

	bool operator==(const nodeN0 &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeN0 &b)  const {
		if (st.jugador.f < b.st.jugador.f) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c) {
      return true;
    } else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula) {
      return true;
    }
    else return false;
	}

};

// NIVEL 1

struct stateN1 {

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN1 &x) const {
    if  (jugador == x.jugador and
        colaborador == x.colaborador
        and ultimaOrdenColaborador == x.ultimaOrdenColaborador)
        return true;
    else return false;
  }

};

struct nodeN1 {

  stateN1 st;
  list<Action> secuencia;

  bool operator==(const nodeN1 &n) const { return (st == n.st); }
  bool operator<(const nodeN1 &b)  const {
		
  if (st.jugador.f < b.st.jugador.f) return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c) return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c 
            and st.jugador.brujula < b.st.jugador.brujula)  return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c 
            and st.jugador.brujula == b.st.jugador.brujula
            and st.colaborador.f < b.st.colaborador.f)  return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c 
            and st.jugador.brujula == b.st.jugador.brujula
            and st.colaborador.f == b.st.colaborador.f
            and st.colaborador.c < b.st.colaborador.c)  return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c 
            and st.jugador.brujula == b.st.jugador.brujula
            and st.colaborador.f == b.st.colaborador.f
            and st.colaborador.c == b.st.colaborador.c
            and st.colaborador.brujula < b.st.colaborador.brujula)  return true;
    else if (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c 
            and st.jugador.brujula == b.st.jugador.brujula
            and st.colaborador.f == b.st.colaborador.f
            and st.colaborador.c == b.st.colaborador.c
            and st.colaborador.brujula == b.st.colaborador.brujula
            and st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador)  return true;
    else return false;
     
	}

};

// NIVEL 2

struct stateN2 {

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  bool bikini;
  bool zapatillas;

  bool operator==(const stateN2 &b) const {
    if  (jugador == b.jugador 
        and colaborador.f == b.colaborador.f
        and colaborador.c == b.colaborador.c
        and bikini == b.bikini 
        and zapatillas == b.zapatillas)
        return true;
    else return false;
  }

  bool operator<(const stateN2 &b) const{
    if (jugador.f < b.jugador.f) return true;
    else if (jugador.f == b.jugador.f and jugador.c < b.jugador.c) return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c and 
            jugador.brujula < b.jugador.brujula) return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula and bikini < b.bikini) 
            return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikini == b.bikini and zapatillas < b.zapatillas) 
            return true;
    else return false;
	}
};

struct nodeN2 {

  stateN2 st;
  list<Action> secuencia;
  int coste;

  bool operator==(const nodeN2 &n) const { return (st == n.st and coste == n.coste); }
  bool operator<(const nodeN2 &b)  const { return (coste > b.coste); }

};

// NIVEL 3

struct stateN3 {

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  bool bikiniJugador;
  bool zapatillasJugador;  
  bool bikiniColaborador;
  bool zapatillasColaborador;

  bool operator==(const stateN3 &x) const {
    if  (jugador == x.jugador and
        colaborador == x.colaborador
        and ultimaOrdenColaborador == x.ultimaOrdenColaborador
        and bikiniJugador == x.bikiniJugador
        and bikiniColaborador == x.bikiniColaborador
        and zapatillasJugador == x.zapatillasJugador
        and zapatillasColaborador == x.zapatillasColaborador)
        return true;
    else return false;
  }

  bool operator<(const stateN3 &b) const{
    if (jugador.f < b.jugador.f) return true;
    else if (jugador.f == b.jugador.f and jugador.c < b.jugador.c) return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula < b.jugador.brujula)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula and bikiniJugador < b.bikiniJugador) 
            return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador < b.zapatillasJugador) 
            return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f < b.colaborador.f)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c < b.colaborador.c)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula < b.colaborador.brujula)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador < b.ultimaOrdenColaborador)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador == b.ultimaOrdenColaborador
            and bikiniColaborador < b.bikiniColaborador)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador == b.ultimaOrdenColaborador
            and bikiniColaborador == b.bikiniColaborador
            and zapatillasColaborador < b.zapatillasColaborador)  return true;
    else return false;
     
	}

};

struct nodeN3 {

  stateN3 st;
  list<Action> secuencia;
  int h;      // h(n) coste del camino de n al objetivo "aprox" 
  int g;      // g(n) coste del nodo inicial al n 
  int f;      // f(n) = g(n) + h(n)

  bool operator==(const nodeN3 &n) const { return  (st == n.st); }
  bool operator<(const nodeN3 &b)  const { return (f > b.f); }

};

// NIVEL 4

struct stateN4 {

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;
  bool bikiniJugador;
  bool zapatillasJugador;  
  bool bikiniColaborador;
  bool zapatillasColaborador;

  bool operator==(const stateN4 &x) const {
    if  (jugador == x.jugador and
        colaborador == x.colaborador
        and ultimaOrdenColaborador == x.ultimaOrdenColaborador
        and bikiniJugador == x.bikiniJugador
        and bikiniColaborador == x.bikiniColaborador
        and zapatillasJugador == x.zapatillasJugador
        and zapatillasColaborador == x.zapatillasColaborador)
        return true;
    else return false;
  }

  bool operator<(const stateN4 &b) const{
    if (jugador.f < b.jugador.f) return true;
    else if (jugador.f == b.jugador.f and jugador.c < b.jugador.c) return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula < b.jugador.brujula)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula and bikiniJugador < b.bikiniJugador) 
            return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador < b.zapatillasJugador) 
            return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f < b.colaborador.f)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c < b.colaborador.c)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula < b.colaborador.brujula)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador < b.ultimaOrdenColaborador)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador == b.ultimaOrdenColaborador
            and bikiniColaborador < b.bikiniColaborador)  return true;
    else if (jugador.f == b.jugador.f and jugador.c == b.jugador.c 
            and jugador.brujula == b.jugador.brujula
            and bikiniJugador == b.bikiniJugador 
            and zapatillasJugador == b.zapatillasJugador
            and colaborador.f == b.colaborador.f
            and colaborador.c == b.colaborador.c
            and colaborador.brujula == b.colaborador.brujula
            and ultimaOrdenColaborador == b.ultimaOrdenColaborador
            and bikiniColaborador == b.bikiniColaborador
            and zapatillasColaborador < b.zapatillasColaborador)  return true;
    else return false;
     
	}

};

struct nodeN4 {

	stateN4 st;
	list<Action> secuencia;
        int h;      // h(n)
        int g;      // g(n)  
        int f;      // f(n) = g(n) + h(n)

	bool operator==(const nodeN4 &n) const {
		return  (st == n.st);
	}

        bool operator<(const nodeN4 &b)  const {
                return (f > b.f);
	}

};


class ComportamientoJugador : public Comportamiento {
  public:

    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      hayPlan = false;
      primerMovimiento = true;
      segundoMovimiento = false;
      situado = false;
      puedoPintar = false;
      ult_accion = actIDLE;
      hayCasillaBateria = false;
      planBateria = false;
      c_state_n4.st.bikiniColaborador = false;
      c_state_n4.st.zapatillasColaborador = false;
      c_state_n4.st.bikiniJugador = false;
      c_state_n4.st.zapatillasJugador = false;
      c_state_n4.st.ultimaOrdenColaborador = act_CLB_STOP;
      ult_accion_modificada = false;
    }

    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
        hayPlan = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa);
    ubicacion NextCasilla(const ubicacion &pos);
    void AnulaMatriz(vector<vector<unsigned char> > &matriz);
    void PintaPlan(const list<Action> &plan);
   

    // Nivel 0

    bool Find(const stateN0 &item, const list<stateN0> &lista);
    stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa);
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    list<Action> Anchura_nivel0  (const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    bool Find(const stateN0 &item, const list<nodeN0> &lista);

    // Nivel 1

    bool colaboradorAVista(ubicacion jugador, ubicacion colaborador);
    list<Action> Anchura_nivel1	(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    bool Find(const stateN1 &item, const list<nodeN1> &lista);
    stateN1 apply(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa);
    void VisualizaPlan(const stateN1 &st, const list<Action> &plan);

    // Nivel 2
    void ObtencionElementos (nodeN2 &current_node, const vector<vector<unsigned char>> &mapa);
    void VisualizaPlan(const stateN2 &st, const list<Action> &plan);
    nodeN2 apply(const Action &a, const nodeN2 &n, const vector<vector<unsigned char>> &mapa);
    int ValorCoste (int f, int c, bool bikini, bool zapatillas, Action accion, 
                   const vector<vector<unsigned char> > &mapa);
    list<Action> MinimoCaminoJugador (const stateN2 &inicio, const ubicacion &final,const vector<vector<unsigned char>> &mapa);    

    // Nivel 3

    void VisualizaPlan(const stateN3 &st, const list<Action> &plan);
    nodeN3 apply(const Action &a, const nodeN3 &n, const vector<vector<unsigned char>> &mapa);
    void ObtencionElementosJugadores (nodeN3 &current_node, const vector<vector<unsigned char>> &mapa);
    int CosteEstimado (ubicacion ini, ubicacion fin);
    list<Action> Aestrella (const nodeN3 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);

    // Nivel 4

    list<Action> MinimoCaminoJugador (const stateN4 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa);
    void VisualizaPlan(const stateN4 &st, const list<Action> &plan);
    void ObtencionElementosN4 (nodeN4 &current_node, const vector<vector<unsigned char>> &mapa);
    void PintarMapa (Sensores sensores, vector<vector<unsigned char>> &matriz);
    void pintarPrecipicios();
    void Bateria(Sensores sensores);
    nodeN4 apply(const Action &a, const nodeN4 &n, const vector<vector<unsigned char>> &mapa);



  private:
        
        list <Action> plan;
        bool hayPlan;
        ubicacion goal;

        nodeN0 c_state;       // Nivel 0
        nodeN1 c_state_n1;    // Nivel 1
        nodeN2 c_state_n2;    // Nivel 2
        nodeN3 c_state_n3;    // Nivel 3
        nodeN4 c_state_n4;    // Nivel 4

        bool primerMovimiento, segundoMovimiento;
        bool situado;
        bool puedoPintar;

        Action ult_accion;

        int indiceBateria;
        bool planBateria;
        ubicacion bateria;
        bool hayCasillaBateria;
        bool ult_accion_modificada;
};

#endif
