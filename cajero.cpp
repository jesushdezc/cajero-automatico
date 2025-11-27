#include <iostream>
#include <string>
#include <stdexcept>  // Para std::runtime_error
#include <limits>     // Para std::numeric_limits

using namespace std;

// ----------------------
// ESTRUCTURA DE CUENTA
// ----------------------
struct Cuenta {
    int id;
    string titular;
    double saldo;
};

// ----------------------
// EXCEPCIÓN PROPIA
// ----------------------
class EntradaInvalida : public runtime_error {
public:
    EntradaInvalida(const string& msg) : runtime_error(msg) {}
};

// ----------------------
// PROTOTIPOS DE FUNCIONES
// ----------------------

// Menú principal
void mostrarMenu();

// Lectura segura de datos (usa excepciones)
int leerEntero(const string& mensaje);
double leerDouble(const string& mensaje);

// Operaciones con cuentas
void consultarSaldo(const Cuenta& cuenta);
void depositar(Cuenta* cuenta, double monto);
void retirar(Cuenta* cuenta, double monto);
void transferir(Cuenta* origen, Cuenta* destino, double monto);

// Búsqueda de cuenta en el arreglo
Cuenta* buscarCuenta(Cuenta cuentas[], int numCuentas, int idCuenta);


// ----------------------
// FUNCIONES
// ----------------------

// Menú de opciones
void mostrarMenu() {
    cout << "\n====== CAJERO AUTOMATICO ======\n";
    cout << "1. Consultar saldo\n";
    cout << "2. Depositar\n";
    cout << "3. Retirar\n";
    cout << "4. Transferir\n";
    cout << "5. Salir\n";
    cout << "Seleccione una opcion: ";
}

// Lectura de entero con validación y excepción
int leerEntero(const string& mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw EntradaInvalida("Entrada invalida, se esperaba un numero entero.");
    }

    return valor;
}

// Lectura de double con validación y excepción
double leerDouble(const string& mensaje) {
    double valor;
    cout << mensaje;
    cin >> valor;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw EntradaInvalida("Entrada invalida, se esperaba un numero.");
    }

    return valor;
}

// Consultar saldo (no modifica la cuenta, por eso recibe const &)
void consultarSaldo(const Cuenta& cuenta) {
    cout << "\n--- Consulta de saldo ---\n";
    cout << "Cuenta: " << cuenta.id << " | Titular: " << cuenta.titular << "\n";
    cout << "Saldo actual: $" << cuenta.saldo << "\n";
}

// Depositar (usa puntero a Cuenta)
void depositar(Cuenta* cuenta, double monto) {
    if (monto <= 0) {
        throw runtime_error("El monto de deposito debe ser mayor a 0.");
    }
    cuenta->saldo += monto;
    cout << "Deposito exitoso. Nuevo saldo: $" << cuenta->saldo << "\n";
}

// Retirar (usa puntero a Cuenta)
void retirar(Cuenta* cuenta, double monto) {
    if (monto <= 0) {
        throw runtime_error("El monto de retiro debe ser mayor a 0.");
    }
    if (monto > cuenta->saldo) {
        throw runtime_error("Saldo insuficiente para realizar el retiro.");
    }
    cuenta->saldo -= monto;
    cout << "Retiro exitoso. Nuevo saldo: $" << cuenta->saldo << "\n";
}

// Transferencia entre cuentas (punteros a origen y destino)
void transferir(Cuenta* origen, Cuenta* destino, double monto) {
    if (origen == destino) {
        throw runtime_error("No se puede transferir a la misma cuenta.");
    }
    if (monto <= 0) {
        throw runtime_error("El monto de transferencia debe ser mayor a 0.");
    }
    if (monto > origen->saldo) {
        throw runtime_error("Saldo insuficiente en la cuenta de origen.");
    }

    origen->saldo -= monto;
    destino->saldo += monto;

    cout << "Transferencia exitosa.\n";
    cout << "Saldo nuevo cuenta origen (" << origen->id << "): $" << origen->saldo << "\n";
    cout << "Saldo nuevo cuenta destino (" << destino->id << "): $" << destino->saldo << "\n";
}

// Buscar cuenta en el arreglo (devuelve puntero o nullptr)
Cuenta* buscarCuenta(Cuenta cuentas[], int numCuentas, int idCuenta) {
    for (int i = 0; i < numCuentas; i++) {
        if (cuentas[i].id == idCuenta) {
            return &cuentas[i]; // Devolvemos puntero al elemento del arreglo
        }
    }
    return nullptr;
}


// ----------------------
// FUNCIÓN PRINCIPAL
// ----------------------
int main() {
    const int NUM_CUENTAS = 3;
    // Arreglo de estructuras (mínimo 3 cuentas)
    Cuenta cuentas[NUM_CUENTAS] = {
        {1, "Alejandro", 1500.0},
        {2, "Maria",     2500.0},
        {3, "Carlos",     800.0}
    };

    int opcion = 0;

    // Bucle principal del menú (repetición)
    do {
        try {
            mostrarMenu();
            cin >> opcion;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw EntradaInvalida("Entrada invalida en la opcion del menu.");
            }

            if (opcion == 5) {
                cout << "Saliendo del sistema...\n";
                break;  // salto para salir del bucle
            }

            // Seleccionar la cuenta con la que trabajar
            int idCuenta = leerEntero("\nIngrese el ID de la cuenta con la que desea operar: ");
            Cuenta* cuentaActual = buscarCuenta(cuentas, NUM_CUENTAS, idCuenta);

            if (cuentaActual == nullptr) {
                throw runtime_error("La cuenta no existe.");
            }

            switch (opcion) {
                case 1: { // Consultar saldo
                    consultarSaldo(*cuentaActual);
                    break;
                }
                case 2: { // Depositar
                    double monto = leerDouble("Monto a depositar: ");
                    depositar(cuentaActual, monto);
                    break;
                }
                case 3: { // Retirar
                    double monto = leerDouble("Monto a retirar: ");
                    retirar(cuentaActual, monto);
                    break;
                }
                case 4: { // Transferir
                    int idDestino = leerEntero("Ingrese el ID de la cuenta destino: ");
                    Cuenta* cuentaDestino = buscarCuenta(cuentas, NUM_CUENTAS, idDestino);
                    if (cuentaDestino == nullptr) {
                        throw runtime_error("La cuenta destino no existe.");
                    }
                    double monto = leerDouble("Monto a transferir: ");
                    transferir(cuentaActual, cuentaDestino, monto);
                    break;
                }
                default:
                    cout << "Opcion no valida.\n";
                    break;
            }

        } catch (const EntradaInvalida& e) {
            cout << "\n[ERROR DE ENTRADA] " << e.what() << "\n";
        } catch (const runtime_error& e) {
            cout << "\n[ERROR] " << e.what() << "\n";
        } catch (...) {
            cout << "\n[ERROR DESCONOCIDO]\n";
        }

        // Limpiar buffer de entrada por si quedaron cosas
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (true);

    return 0;
}
