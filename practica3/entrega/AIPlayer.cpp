# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta



bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

///////////////////  MÉTODOS TUTORIAL /////////////////////////////////////////

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const{
    
    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;

    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{

    // El número de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento.
    vector<int> current_dices;

    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Obtengo el vector de dados que puedo usar para el movimiento.
    // - actual->getAvailableNormalDices(player) -> solo dados disponibles en ese turno.
    // Importante : "getAvailableNormalDices" me da los dados que puedo usar en el turno actual.
    // Por ejemplo, si me tengo que contar 10 o 20 solo me saldrán los dados 10 y 20.
    // Puedo saber qué más dados tengo, aunque no los pueda usar en este turno, con:
    // - actual->getNormalDices(player) -> todos los dados
    current_dices = actual->getAvailableNormalDices(player);

    // En vez de elegir un dado al azar, miro primero cuáles tienen fichas que se puedan mover.
    vector<int> current_dices_que_pueden_mover_ficha;


    for (int i = 0; i < current_dices.size(); i++)
    {
    // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
    current_pieces = actual->getAvailablePieces(player, current_dices[i]);

    // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas.
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices[i]);
        }
    }
    
    // Si no tengo ningún dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me permite no mover).

    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices[rand() % current_dices.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha.
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];

        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, dice);

        // Muevo una ficha al azar de entre las que se pueden mover.
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    // Elijo el dado haciendo lo mismo que el jugador anterior.
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);
   
    // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.
    // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
    // el dado especial a) la que esté más adelantada
    // (equivalentemente, la más cercana a la meta).
    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
    int id_ficha_mas_adelantada = -1;
    color col_ficha_mas_adelantada = none;
    int min_distancia_meta = 9999;
   
    for (int i = 0; i < current_pieces.size(); i++)
    {
        // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
        color col = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distancia_meta = actual->distanceToGoal(col, id);
    
        if (distancia_meta < min_distancia_meta)
        {
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = id;
            col_ficha_mas_adelantada = col;
        }
    }
    // Si no he encontrado ninguna ficha, paso turno.
    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, moveré la ficha más adelantada.
    else
    {
        id_piece = id_ficha_mas_adelantada;
        c_piece = col_ficha_mas_adelantada;
    }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{

    // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.
    // Cuando ya he recorrido todos los hijos, la función devuelve el estado actual. De esta forma puedo saber
    // cuándo paro de iterar.
    // Para ello, vamos a iterar sobre los hijos con la función de Parchis getChildren().
    // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
    // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.
    ParchisBros hijos = actual->getChildren();
    bool me_quedo_con_esta_accion = false;

    // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
    // Al hacer begin() accedemos al primer hijo de la rama,
    // y cada vez que hagamos ++it saltaremos al siguiente hijo.
    // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.
    // Voy a moverme a la casilla siempre con la que gane más energía, salvo que me encuentre con
    // algún movimiento muy interesante, como comer fichas o llegar a la meta.
    int current_power = actual->getPowerBar(this->jugador).getPower();
    int max_power = -101; // Máxima ganancia de energía.

    for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() and !me_quedo_con_esta_accion; ++it)
    {
        Parchis siguiente_hijo = *it;
        // Si en el movimiento elegido comiera ficha, llegara a la meta o ganara, me quedo con esa acción.
        // Termino el bucle en este caso.
        if (siguiente_hijo.isEatingMove() or
        siguiente_hijo.isGoalMove() or
        (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador))
        {
            me_quedo_con_esta_accion = true;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
        }
        // En caso contrario, me voy quedando con el que me dé más energía.
        else{
            int new_power = siguiente_hijo.getPower(this->jugador);
            if(new_power - current_power > max_power){
                c_piece = it.getMovedColor();
                id_piece = it.getMovedPieceId();
                dice = it.getMovedDiceValue();
                max_power = new_power - current_power;
            }
        }
    }
    // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
    // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
}


///////////////////  ALGORITMO  ///////////////////////////////////////////////

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const {

    double alpha_aux, beta_aux = 0;

   // si nodo es un nodo terminal o hemos  alcanzado la profundidad máxima
    if (profundidad == profundidad_max or actual.gameOver()){
        return heuristic(actual, jugador);
    }

    // si jugador
    if (actual.getCurrentPlayerId() == jugador){

        ParchisBros child = actual.getChildren();

        for (ParchisBros::Iterator it = child.begin(); it != child.end() and alpha < beta; it.operator++()){
    
            alpha_aux = Poda_AlfaBeta   (it.operator*(), jugador, profundidad+1, profundidad_max, c_piece,
                                        id_piece, dice, alpha, beta, *heuristic);

            // Comprobamos si es máximo           
            if (alpha < alpha_aux){
                
                alpha = alpha_aux;

                if (profundidad == 0){
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();
                }

                if (alpha >= beta) {
                    return beta;
                    break;
                }
            }
        }      

        return alpha;
    }

    // si contrincante
    else {

        ParchisBros child = actual.getChildren();

        for (ParchisBros::Iterator it = child.begin(); it != child.end(); it.operator++()){

            beta_aux = Poda_AlfaBeta    (it.operator*(), jugador, profundidad+1, profundidad_max, c_piece, 
                                        id_piece, dice, alpha, beta, heuristic); 

            // Comprobamos si es mínimo           
            if (beta_aux < beta){
                beta = beta_aux;
                if (beta <= alpha){
                    return alpha;                    
                }
            }
        }  

        return beta;
    }
}

///////////////////  HEURÍSTICAS //////////////////////////////////////////////


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}


///////// HEURÍSTICA 1 

double AIPlayer::MiValoracion1(const Parchis &estado, int jugador){

    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    // Si no, devuelvo la puntuación de mi jugador menos la puntuación del oponente.
    if (ganador == jugador) return gana;
    else if (ganador == oponente) return pierde;
    else return calculosJugadores1(estado, jugador) - calculosJugadores1(estado, oponente);
    
}

double AIPlayer::calculosJugadores1(const Parchis &estado, int j1){
        
        // Colores que juega mi jugador 
        vector<color> my_colors = estado.getPlayerColors(j1);
        int puntuacion_jugador = 0;

        // Recorro colores de mi jugador
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            
            // Para cada color recorro sus fichas
            for (int j = 0; j < num_pieces; j++)
            {

                // Valoro positivamente que este cerca de la meta
                puntuacion_jugador += 68 - estado.distanceToGoal(c, j);

                // Valoro negativamente que tenga fichas en casa
                puntuacion_jugador -= estado.piecesAtHome(c);

                // Valoro positivamente que la ficha esté en casilla segura.
                if (estado.isSafePiece(c, j)) puntuacion_jugador += 2;

                // Valoro positivamente que sea movimiento estrella
                if(estado.isStarMove()) puntuacion_jugador += 2;

                // Valoro positivamente que sea movimiento bala
                if(estado.isBulletMove()) puntuacion_jugador += 3;

                // Valoro positivamente que se llegue meta.
                if (estado.isGoalMove()) puntuacion_jugador += 5;

                // Valoro positivamente que sea una partida donde se coma una ficha
                if (estado.isEatingMove()) puntuacion_jugador += 15;

            }
        }

    return puntuacion_jugador;
}


///////// HEURÍSTICA 2 (ampliación de la heurística 1) 

double AIPlayer::MiValoracion2(const Parchis &estado, int jugador){

    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    // Si no, devuelvo la puntuación de mi jugador menos la puntuación del oponente.
    if (ganador == jugador) return gana;
    else if (ganador == oponente) return pierde;
    else return calculosJugadores2(estado, jugador) - calculosJugadores2(estado, oponente);
    
}

double AIPlayer::calculosJugadores2(const Parchis &estado, int j1){

        int j2 = (j1+1) % 2;
        int solo_beneficiamos_a_uno = -1;
        
        // Colores que juega mi jugador 
        vector<color> my_colors = estado.getPlayerColors(j1);

        // Distancia a meta de mis colores 
        // (inicializado a uno porque se usa para dividir)
        vector<int> my_colors_distance = {1,1};

        // Colores de mi oponente
        vector<color> op_colors = estado.getPlayerColors(j2);
        
        int puntuacion_jugador = 0;

        // Recorro colores de mi jugador
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];

            // Para cada color recorro sus fichas
            for (int j = 0; j < num_pieces; j++)
            {
                if (solo_beneficiamos_a_uno == -1 or solo_beneficiamos_a_uno == i){

                    my_colors_distance[c] += estado.distanceToGoal(c, j);

                    // Valoro positivamente qeu este cerca de la meta
                    puntuacion_jugador += 68 - estado.distanceToGoal(c, j);

                    // Valoro negativamente que tenga fichas en casa
                    puntuacion_jugador -= estado.piecesAtHome(c)*3;

                    // Valoro positivamente que hayan fichas en la meta
                    puntuacion_jugador += estado.piecesAtGoal(c);

                    // Valoro positivamente que haga movimiento rápido
                    if (estado.isMushroomMove()) puntuacion_jugador += 2;

                    // Valoro positivamente que la ficha esté en casilla segura.
                    if (estado.isSafePiece(c, j)) puntuacion_jugador += 5;

                    // Valoro positivamente que sea movimiento estrella
                    if(estado.isStarMove()) puntuacion_jugador += 5;

                    // Valoro positivamente que sea movimiento bala
                    if(estado.isBulletMove()) puntuacion_jugador += 20;

                    // Valoro positivamente que haga movimiento ultra rápido
                    if (estado.isMegaMushroomMove()) puntuacion_jugador += 10;

                    // Valoro positivamente que se llegue meta.
                    if (estado.isGoalMove()) puntuacion_jugador += 15;

                    // Valoro positivamente que haya barrera
                    if (estado.isWall(estado.getBoard().getPiece(c, j).get_box())) puntuacion_jugador += 2;

                    // Valoro positivamente que sea una partida donde se coma una ficha
                    if (estado.isEatingMove()) puntuacion_jugador += 250;

                    // Para cada color del contrincante
                    for (int k = 0; k < op_colors.size(); k++)
                    {
                        color c_op = op_colors[k];
                        // Recorro las fichas de ese color.
                        for (int j_op = 0; j_op < num_pieces; j_op++)
                        {
                            // Veo si estoy por delante de fichas de mi oponente a menos de 7 casillas
                            // o si estoy 3 casillas por delante 
                            pair <color, int> ficha_op = {c_op, j_op};
                            int distancia = estado.distanceBoxtoBox(c_op, j_op, c, j);

                            if ((distancia > 0 and distancia < 3) or (distancia > 3 and distancia <= 7) 
                                and !estado.isSafePiece(c,j)){
                                puntuacion_jugador--;
                            }
                        }
                    }
                }
            }
            
            // Beneficiamos a aquel color que va ganando
            if (estado.piecesAtGoal(c) >= 2 and estado.piecesAtHome(c) == 0) solo_beneficiamos_a_uno = i;

            if (solo_beneficiamos_a_uno == i){
                puntuacion_jugador += 2000 / my_colors_distance[i];
            }
            
        }

        bool mi_ficha, ficha_oponente = false; 

        vector<pair<color, int>> v = estado.piecesDestroyedLastMove();
        for (int i=0; i < v.size(); i++){
            for (int j=0; j < my_colors.size(); j++){
                if (v[i].first == my_colors[j]) mi_ficha = true;
            }
            for (int k=0; k < op_colors.size(); k++){
                if (v[i].first == op_colors[k]) ficha_oponente = true;
            }
        }
        
            
        // Valoro positivamente que se destruyan fichas de 
        // algún color del oponente y ninguno de mis coloress
        if (ficha_oponente and !mi_ficha) puntuacion_jugador += 3;
                    
    return puntuacion_jugador;
}

///////////////////  MÉTODO THINK /////////////////////////////////////////////

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
   

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta

    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1: 
            // Mejor heurística
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
    }
    
    cout << "Valor Poda AlfaBeta: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

}



