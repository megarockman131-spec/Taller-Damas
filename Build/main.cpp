#include "tablero.h"
#include <iostream>
#include <ctime>
#include <limits>

int main() {
    srand((unsigned)time(nullptr));
    initBoard();

    mostrarMenuRetro();
    int modo;
    while (!(std::cin >> modo) || (modo != 1 && modo != 2)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[31m" << "Ingrese solamente el numero 1 o 2.\n\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std:: cout << "\a\n";

    bool vsCPU = (modo == 1);
    char turno = 'r';

    while (true) {
        printBoard();

        if (!playerHasPieces('r') || !hasAnyMoves('r')) {
            std::cout << "Negro (b) gana. Fin.\n";
            break;
        }
        if (!playerHasPieces('b') || !hasAnyMoves('b')) {
            std::cout << "Rojo (r) gana. Fin.\n";
            break;
        }

        std::cout << "\nTurno: " << (turno == 'r' ? "Rojo (r)" : "Negro (b)") << "\n";
        std::vector<Move> moves = getAllMoves(turno);
        if (moves.empty()) {
            std::cout << "Sin movimientos posibles para " << turno << ".\n";
            turno = (turno == 'r' ? 'b' : 'r');
            continue;
        }

        Move chosen;
        bool ok = false;

        if (vsCPU && turno == 'b') {
            chosen = chooseAIMove('b');
            if (!inside(chosen.x1, chosen.y1) || !inside(chosen.x2, chosen.y2)) {
                std::cout << "La CPU no tiene movimientos validos.\n";
                turno = 'r';
                continue;
            }
            std::cout << "CPU movió: " << chosen.x1 << " " << chosen.y1 << " -> " << chosen.x2 << " " << chosen.y2 << "\n";
            ok = true;
        } else {
            std::cout << "Ingrese movimiento: x1 y1 x2 y2 (ej: 5 0 4 1)\n";
            if (!parseHumanMove(chosen)) {
                std::cout << "Entrada no valida. Intente otra vez.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            bool ok = false;

            for (auto &m : moves) {
                if (m.x1 == chosen.x1 && m.y1 == chosen.y1 && m.x2 == chosen.x2 && m.y2 == chosen.y2) {
                    chosen.isCapture = m.isCapture;
                    ok = true;
                    break;
                }
            }

            if (!ok) {
                std::cout << "Movimiento invalido. Intente otra vez.\n";
                continue;
            }
        }

        applyMoves(chosen);

        // Captura en cadena
        if (chosen.isCapture) {
            bool cadena = true;
            while (cadena) {
                std::vector<Move> siguiente = getAllMoves(turno);
                cadena = false;
                for (auto &m : siguiente) {
                    if (m.isCapture && m.x1 == chosen.x2 && m.y1 == chosen.y2) {
                        applyMoves(m);
                        chosen = m;
                        cadena = true;
                        break;
                    }
                }
            }
        }

        turno = (turno == 'r' ? 'b' : 'r');
    }

    do {
        char respuesta;
        char salir;
        std::cout << "\nQuieres jugar otravez? (s/n): ";
        std::cin >> respuesta;
        if( respuesta == 'n' || respuesta == 'N') {
            std::cout << "\n Gracias por jugar.";
            std::cout << "Oprime cualquier tecla para salir.";
            std::cin >> salir;
            return 0;
        }else if (respuesta == 's'|| respuesta == 'S'){
            break;
        }else {
            std::cout << "\033[31m" << "Ingrese solamente S o N \n";
        }
    }while (true);
    

    while (true);
}
 






