#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

// Struct de Estado

struct state {
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento 
{

  public:

    // Constructor 
    ComportamientoJugador(unsigned int size) : Comportamiento(size){

      current_state.fil = current_state.col = 99;
      current_state.brujula = norte;
      last_action = actIDLE;
      girar_derecha = false;
      bien_situado = false;

	    bikini = false;
	    zapatillas = false;

      recien_situado = true;
      pintar_matriz_auxiliar= true;
      
      contador_giros = 0;
      cont_ciclos = 20;
      

      // Inicializamos el estado auxilizar
      auxiliar_state.col = mapaResultado.size();
      auxiliar_state.fil = mapaResultado.size();
      auxiliar_state.brujula = norte; 
      
      // Inicializamos el mapa auxiliar
      vector <unsigned char> aux_char (2*mapaResultado.size(),'?');
      for (int i=0; i < 2*mapaResultado.size(); i++) mapaAuxiliar.push_back(aux_char);

      // Inicializamos el mapa y vector contador
      vector<int> aux (mapaResultado.size(), 0);
      for (int i=0; i < mapaResultado.size(); i++) mapaContador.push_back(aux);

      // Pintamos precipicios en mapaResultado
      pintarPrecipicios();

    }

    ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport){}
    
    // Destructor
    ~ComportamientoJugador(){}

    // Métodos públicos
    Action think (Sensores sensores);
    int interact (Action accion, int valor);

    void PonerTerrenoEnMatriz (Sensores sensores, 
                              const state &st, vector<vector<unsigned char>> &matriz);
    void reiniciar();
    void establecerPosicion(Sensores sensores);
    void pintarPrecipicios();
    void modificarState(state &st);
    
    void copiarMatriz(Sensores sensores);
    void resetearMatrizAuxiliar();
    void rotarMatriz(Sensores sensores);

    int casillaPrioritaria(Sensores sensores);
    Action pasoAcasillaEspecial (int pos);
    bool casillaTransitable(int pos, Sensores sensores);

    int seleccionarCasilla ();

  private:
  
    // Variables de estado
    state current_state; 
    Action last_action;  
    bool bikini;
    bool zapatillas;
    bool girar_derecha;
    bool bien_situado;    


    // Variables para pintar sin estar situado
    vector<vector<unsigned char>> mapaAuxiliar;
    state auxiliar_state;
    bool recien_situado;
    bool pintar_matriz_auxiliar;


    // Variables para pasar por casillas menos visitadas
    vector<vector<int>> mapaContador;
    int contador_giros;
    int cont_ciclos;

};
#endif
