// AquiTa - Sistema de gestión de objetos perdidos y encontrados (C++17, consola).
#include <cstdlib>

#include "Menu.h"
#include "Sistema.h"

int main() {
#ifdef _WIN32
    std::system("chcp 65001 > nul");  // para que la consola de Windows muestre tildes y ñ
#endif
    Sistema sistema;
    sistema.cargarTodo();  // SUPL02: cargar los archivos al iniciar
    menuPrincipal(sistema);
    return 0;
}
