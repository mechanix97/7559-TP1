
#include "Horno.h"

#include "PedidosParaHornear.h"
#include "PedidosParaEntregar.h"

Horno::Horno() {

}

Horno::~Horno() {

}

void Horno::realizarTarea() {

    log(logDEBUG, "Esperando pedido para hornear...");
    if (PedidosParaHornear::getInstance()->esperarNuevoPedido() != 0) {
        this->log(logERROR, "ERROR AL ESPERAR NUEVO PEDIDO PARA HORNEAR. - " + to_string(errno));
        cout << ">>>>>>>> FATAL ERROR: " << strerror(errno) << " <<<<<<<<" << endl;
        assert(false); // error al realizar la espera!
    }

    if (PedidosParaHornear::getInstance()->tomarNuevoPedido()) {

    	this->log(logDEBUG, "Tomando Nuevo pedido para hornear.");

        PedidosParaEntregar::getInstance()->nuevoPedidoListo();
        this->log(logDEBUG, "Nuevo pedido listo para entregar.");
    }
}

string Horno::nombre() {
    return "Horno " + to_string(getID());
}

void Horno::destruirRecursos() {

}
