#include <iostream>
#include <getopt.h>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

// Include de objectos del modelo
#include "Recepcionista.h"
#include "Cocinera.h"
#include "Cadeta.h"
#include "Supervisora.h"

#include "MemoriaCompartida2.h"
#include "MemoriaCompartidaConcurrente.h"


using namespace std;

#define kAppVersion "0.1"

#define kDefaultRecepcionistasCount 2
#define kDefaultCadetasCount        1
#define kDefaultCocinerasCount      4
#define kDefaultHornosCount         4
#define kDefaultSimulacionCount     4

#define kCLINoAargument         0
#define kCLIRequiredArgument    1
#define kCLIOptionalArgument    2

#define kDebugModeON            true
#define kDebugModeOFF           false
#define kDefaultDebugMode       kDebugModeOFF

bool            debugMode               = kDefaultDebugMode;
long   recepcionistasCount     = kDefaultRecepcionistasCount;
long   cadetasCount            = kDefaultCadetasCount;
long   hornosCount             = kDefaultHornosCount;
long   cocinerasCount          = kDefaultCocinerasCount;

long            simulacionCount         = kDefaultSimulacionCount;

void print_version() {
    cout << "ConcuDelivery v" << kAppVersion << endl << endl;
    cout << "[     ]\tOpromolla, Giovanni\t\tgiopromolla@gmail.com " << endl;
    cout << "[84316]\tAceto, Ezequiel\t\tezequiel.aceto@gmail.com  " << endl;
}


void printHelp() {
    cout << endl;
    cout << "Usage: ConcuDelivery <parameter>"  << endl;
    cout << endl;
    cout << "where <parameter> may be one or more of: " << endl;
    cout << "\tr (recepcionista)\t\t[int] Number of process with type 'recepcionistas'" << endl;
    cout << "\tc (cocineras)\t\t\t[int] Number of process with type 'cocineras'" << endl;
    cout << "\to (hornos)\t\t\t[int] Number of process with type 'hornos'" << endl;
    cout << "\ta (cadetas)\t\t\t[int] Number of process with type 'cadetas'" << endl;
    cout << endl;
    cout << "ConcuDelivery --help\t\t\tdisplay help" <<endl;
    cout << "ConcuDelivery --version\t\t\tdisplay version" <<endl;
    cout << endl;
}

void printVersion() {
    print_version();
}

int setupCLI(int argc, char **argv) {
    const struct option longopts[] =
        {
                {"version",         kCLINoAargument,        NULL, 'v'},
                {"help",            kCLINoAargument,        NULL, 'h'},
                {"debug",           kCLINoAargument,        NULL, 'd'},
                {"recepcionistas",  kCLIRequiredArgument,   NULL, 'r'},
                {"cocineras",       kCLIRequiredArgument,   NULL, 'c'},
                {"hornos",          kCLIRequiredArgument,   NULL, 'o'},
                {"cadetas",         kCLIRequiredArgument,   NULL, 'a'},
                {0,0,0,0},
        };

    int index;
    int iarg=0;

    //turn off getopt error message
    opterr=1;

    while(iarg != -1)
    {
        iarg = getopt_long(argc, argv, "vhdr:c:o:a:", longopts, &index);

        switch (iarg) {
            case 'h':
                printHelp();
                return -1;

            case 'v':
                printVersion();
                return -1;

            case 'd':
                debugMode = kDebugModeON;
                break;

            case 'r':
                if (optarg) {
                    recepcionistasCount = atol(optarg);
                }
                if (recepcionistasCount < 0) {
                    recepcionistasCount = kDefaultRecepcionistasCount;
                }
                break;

            case 'c':
                if (optarg) {
                    cocinerasCount = atol(optarg);
                }
                if (cocinerasCount < 0) {
                    cocinerasCount = kDefaultCocinerasCount;
                }
                break;

            case 'o':
                if (optarg) {
                    hornosCount = atol(optarg);
                }
                if (hornosCount < 0) {
                    hornosCount = kDefaultHornosCount;
                }
                break;

            case 'a':
                if (optarg) {
                    cadetasCount = atol(optarg);
                }
                if (cadetasCount < 0) {
                    cadetasCount = kDefaultCadetasCount;
                }
                break;
        }
    }

    return 0;
}

void inicializar() {
    Logger* log = Logger::getInstance();
    log->log(logINFO,"Inicio de ConcuDelivery.");
}

void loggearParametros() {
    Logger* log = Logger::getInstance();
    log->log(logINFO,"Recepcionistas: " + to_string(recepcionistasCount));
    log->log(logINFO,"Cocineras: " + to_string(cocinerasCount));
    log->log(logINFO,"Hornos: " + to_string(hornosCount));
    log->log(logINFO,"Cadetas: " + to_string(cadetasCount));
    log->log(logINFO,"Pedidos a simular: " + to_string(simulacionCount));
}

void crearRecepcionistas(vector<pid_t>& recepcionistas) {
    recepcionistas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < recepcionistasCount; i++) {
        Recepcionista r;
        pid_t rpid = r.iniciar();
        log->log(logINFO,"Nueva recepcionista. PID: " + to_string(rpid));
        recepcionistas.push_back(rpid);
    }
}


void crearCocineras(vector<pid_t>& cocineras) {
    cocineras.reserve(cocinerasCount);
    Logger* log = Logger::getInstance();
    Cocinera c;
    for (unsigned long i = 0; i < cocinerasCount; i++) {
        pid_t rpid = c.iniciar();
        log->log(logINFO,"Nueva Cocinera. PID: " + to_string(rpid));
        cocineras.push_back(rpid);
    }
}


void crearCadetas(vector<pid_t>& cadetas) {
    cadetas.reserve(cadetasCount);
    Logger* log = Logger::getInstance();
    Cadeta c;
    for (unsigned long i = 0; i < cadetasCount; i++) {
        pid_t rpid = c.iniciar();
        log->log(logINFO,"Nueva Cadeta. PID: " + to_string(rpid));
        cadetas.push_back(rpid);
    }
}

void crearHornos() {

}


void pararTodas(vector<pid_t>& pids) {
    Logger* log = Logger::getInstance();
    for (unsigned long i = 0; i < pids.size(); i++) {
        pid_t pid = pids.at(i);
        log->log(logINFO,"Parando pid "+ to_string(pid));
        Proceso::parar(pid);
        int status = 0;
        waitpid(pid,&status,0);
        log->log(logINFO,"Proceso " + to_string(pid) + " parado con status: " + to_string(status));
    }
}

bool realizarPedido(MemoriaCompartidaConcurrente<unsigned long> pedidosSiendoAtendidos) {
    unsigned long siendoAtendidos = pedidosSiendoAtendidos.leer();
    if (siendoAtendidos < recepcionistasCount) {
        pedidosSiendoAtendidos.escribir(siendoAtendidos + 1);
        return true;
    }
    return false;
}

void comenzarTrabajo() {
    Logger* log = Logger::getInstance();

    SIGINT_Handler sigint_handler;
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGTERM, &sigint_handler);

    // crear recursos
    crearHornos();

    // memoria compartida donde se almacena el contador de pedidos entregados

    // crear empleadas
    log->log(logINFO,"Creando procesos");
    vector<pid_t> recepcionistas;
    vector<pid_t> cocineras;
    vector<pid_t> cadetas;
    Supervisora s;
    pid_t supervisora = s.iniciar();

    crearRecepcionistas(recepcionistas);
    crearCocineras(cocineras);
    crearCadetas(cadetas);

    bool working = true;

    int cantidadDePedidosEntregados = 0;
    int cantidadDePedidosRealizados = 0;

    MemoriaCompartidaConcurrente<unsigned long> pedidosSiendoAtendidos (".pedidosSiendoAtendidos.sh",'A');
    pedidosSiendoAtendidos.escribir(0);

    while (sigint_handler.getGracefulQuit() == 0 && working && cantidadDePedidosRealizados < simulacionCount) {
        log->log(logINFO,"<< Contador simulacion " + to_string(simulacionCount));

        // hacer nuevo pedido
        if (realizarPedido(pedidosSiendoAtendidos)) {
            cantidadDePedidosRealizados++;
        }

        // sleep max 1seg
        long time_in_usec = std::rand() % 1000;
        usleep(time_in_usec * 1000);
    }

    // esperar a que se terminen de entregar todos los productos
    do {
        cantidadDePedidosEntregados = 1;

    } while (cantidadDePedidosEntregados < cantidadDePedidosRealizados);

    log->log(logINFO,">> Deteniendo procesos...");
    pararTodas(recepcionistas);
    pararTodas(cocineras);
    pararTodas(cadetas);

    recepcionistas.clear();
    cocineras.clear();
    cadetas.clear();
    Proceso::parar(supervisora);

    SignalHandler::destruir();

    log->log(logDEBUG,"Simulacion terminada");
}

void eliminarRecursos() {
    Logger::destroy();
}

int main(int argc, char **argv) {
    if (setupCLI(argc, argv)) {
        return ( 0 );
    }

    inicializar();
    loggearParametros();

    comenzarTrabajo();

    eliminarRecursos();
    return ( 0 );
}
