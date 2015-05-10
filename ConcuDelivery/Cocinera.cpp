
#include "Cocinera.h"

#include "PedidosParaCocinar.h"
#include "PedidosParaHornear.h"

Cocinera::Cocinera() {

}

Cocinera::~Cocinera() {

}

void Cocinera::realizarTarea() {

	/** Esperar pedido para cocinar **/
    log(logDEBUG, "Esperando nuevo pedido para cocinar...");
    if (PedidosParaCocinar::getInstance()->esperarNuevoPedido() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO PARA COCINAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    /** Tomar pedido para cocinar **/
    if (PedidosParaCocinar::getInstance()->tomarNuevoPedido()) {
    	//TODO
        this->log(logDEBUG, "Tomando Nuevo pedido para cocinar.");

        /** Esperar PedidosParaHornear disponible**/
        //TODO Lock si no hay hornos disponibles

        /** AgregarPedido para Hornear **/
        PedidosParaHornear::getInstance()->hornearPedido();
        this->log(logDEBUG, "Nuevo pedido listo para entregar.");
    }

}

string Cocinera::nombre() {
    return "Cocinera " + to_string(getID());
}

void Cocinera::destruirRecursos() {

}
