// #include "UsuarioPremiumLimitado.h"
// #include <iostream>

// UsuarioPremiumLimitado::UsuarioPremiumLimitado(int id, const std::string& palabra, double saldoInicial) 
//     : UsuarioPremium(id, palabra, saldoInicial) {}

// void UsuarioPremiumLimitado::buscar() {
//     std::cout << "Buscar con saldo: " << saldo << std::endl;
// }

//      +------------------+
//      |     Usuario      |
//      +------------------+
//      | - id: int        |
//      | - palabraBusqueda: string |
//      +------------------+
//      | + buscar(): void |
//      +--------^---------+
//               |
//     +---------+-------------------+
//     |                             |
// +---+-------+-----------+     +--------+---------+
// | UsuarioGratuito       |     |  UsuarioPremium  |
// +-----------+-----+-----+     +--------^---------+
// | - limitePalabras: int |     |                  |
// | + buscar(): void      |     | - saldo: double  |
// +-----------------------+     +----------+-------+
//                                          | 
//                          +---------------------------------+
//                          |                                 |
//                  +-------+-------+----+----------------------------------+
//                  | UsuarioPremiumLimitado |   | UsuarioPremiumIlimitado |
//                  +--------------+---------+   +-------------+-----------+
//                  | + buscar(): void       |   | + buscar(): void        |
//                  +------------------------+   +-------------------------+
