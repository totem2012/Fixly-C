#define _DEFAULT_SOURCE

/* =====================================================================
 * Fixly - Sistema Integral de Gestión de Taller Mecánico
 * Versión en Lenguaje C para Programación I (UTN INSPT - Comisión 1.605)
 * ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utiles.h"

/* =====================================================================
 * CONSTANTES Y LÍMITES DEL SISTEMA
 * ===================================================================== */
#define MAX_CLIENTES         1000
#define MAX_VEHICULOS        1000
#define MAX_REPUESTOS         100
#define MAX_ORDENES          1000
#define MAX_DETALLES          300
#define MAX_FACTURAS         1000
#define MAX_USUARIOS           20
#define CAPACIDAD_TALLER       20

#define LARGO_TEXTO            50
#define LARGO_PATENTE          15
#define LARGO_CODIGO           15
#define LARGO_FECHA            20
#define LARGO_DESCRIPCION     100
#define LARGO_DETALLE         200
#define LARGO_ESTADO           20

#define PORCENTAJE_IVA       21.0f

/* =====================================================================
 * ESTRUCTURAS DE DATOS (STRUCTS)
 * ===================================================================== */

/* Estructura anidada para la liquidación de costos de facturación */
typedef struct {
    float repuestos;
    float horas;
    float valorHora;
    float manoObra;
    float subtotal;
    float iva;
    float total;
} Liquidacion;

/* Entidad Cliente */
typedef struct {
    char nombre[LARGO_TEXTO];
    char apellido[LARGO_TEXTO];
    char celular[LARGO_TEXTO];
    int dni;
} Cliente;

/* Entidad Vehículo */
typedef struct {
    char marca[LARGO_TEXTO];
    char modelo[LARGO_TEXTO];
    char patente[LARGO_PATENTE];
    int dniDueno;
} Vehiculo;

/* Entidad Repuesto */
typedef struct {
    char codigo[LARGO_CODIGO];
    char nombre[LARGO_DESCRIPCION];
    float precio;
    int stock;
} Repuesto;

/* Entidad Detalle de Repuesto en Orden */
typedef struct {
    int nroOrden;
    char codRepuesto[LARGO_CODIGO];
    int cantidad;
    float precioUnitario;
} DetalleOrden;

/* Entidad Orden de Trabajo */
typedef struct {
    int numero;
    int dniCliente;
    char patente[LARGO_PATENTE];
    char fechaInicio[LARGO_FECHA];
    char fechaFin[LARGO_FECHA];
    char fechaEntrega[LARGO_FECHA];
    char descripcion[LARGO_DETALLE];
    char diagnostico[LARGO_DETALLE];
    char trabajo[LARGO_DETALLE];
    char estado[LARGO_ESTADO]; /* "Pendiente" o "Finalizada" */
} OrdenTrabajo;

/* Entidad Factura (con estructura anidada Liquidacion) */
typedef struct {
    int numero;
    int nroOrden;
    int dni;
    char patente[LARGO_PATENTE];
    char fecha[LARGO_FECHA];
    Liquidacion montos; /* Estructura anidada */
} Factura;

/* Entidad Usuario */
typedef struct {
    char nombre[LARGO_TEXTO];
    char clave[LARGO_TEXTO];
    char rol[LARGO_ESTADO]; /* "administrador", "mecanico", "cliente" */
} Usuario;

/* =====================================================================
 * PROTOTIPOS DE FUNCIONES (ANUNCIADAS ARRIBA)
 * ===================================================================== */

/* Sistema y Datos Iniciales */
void mostrarLogo(void);
void cargarDatosPrueba(Usuario usuarios[], int *cantUsuarios,
                      Cliente clientes[], int *cantClientes,
                      Vehiculo vehiculos[], int *cantVehiculos,
                      Repuesto repuestos[], int *cantRepuestos,
                      OrdenTrabajo ordenes[], int *cantOrdenes, int *nroOrdenSig,
                      DetalleOrden detalles[], int *cantDetalles,
                      Factura facturas[], int *cantFacturas, int *nroFacturaSig);

/* Búsquedas Utilitarias */
int buscarClientePorDni(const Cliente clientes[], int cantidad, int dniBuscado);
int buscarVehiculoPorPatente(const Vehiculo vehiculos[], int cantidad, const char *patenteBuscada);
int buscarRepuestoPorCodigo(const Repuesto repuestos[], int cantidad, const char *codigoBuscado);
int buscarRepuestoPorNombre(const Repuesto repuestos[], int cantidad, const char *nombreBuscado);
int buscarOrdenPorNumero(const OrdenTrabajo ordenes[], int cantidad, int nroBuscado);
int buscarFacturaPorNumero(const Factura facturas[], int cantidad, int nroBuscado);
int buscarFacturaPorNroOrden(const Factura facturas[], int cantidad, int nroOrdenBuscado);
int buscarUsuarioPorNombre(const Usuario usuarios[], int cantidad, const char *nombreBuscado);

/* Módulo 1: Clientes */
void ingresarCliente(Cliente clientes[], int *contadorClientes);
void mostrarDatosClientes(const Cliente clientes[], int contadorClientes);
void eliminarCliente(Cliente clientes[], int *contadorClientes);
void modificarCliente(Cliente clientes[], int contadorClientes);
void menuClientes(Cliente clientes[], int *contadorClientes);

/* Módulo 2: Vehículos */
void ingresarVehiculo(Vehiculo vehiculos[], int *contadorVehiculos, Cliente clientes[], int *contadorClientes);
void mostrarDatosVehiculos(const Vehiculo vehiculos[], int contadorVehiculos, const Cliente clientes[], int contadorClientes);
void eliminarVehiculo(Vehiculo vehiculos[], int *contadorVehiculos);
void modificarVehiculo(Vehiculo vehiculos[], int contadorVehiculos, const Cliente clientes[], int contadorClientes);
void menuVehiculos(Vehiculo vehiculos[], int *contadorVehiculos, Cliente clientes[], int *contadorClientes);

/* Módulo 3: Órdenes de Trabajo */
void cargarOrden(OrdenTrabajo ordenes[], int *cantidadOrdenes, int *nroOrdenSiguiente,
                 DetalleOrden detalles[], int *cantidadDetalles,
                 const Cliente clientes[], int contadorClientes,
                 const Vehiculo vehiculos[], int contadorVehiculos,
                 Repuesto repuestos[], int cantidadRepuestos);
void modificarOrden(OrdenTrabajo ordenes[], int cantidadOrdenes,
                    DetalleOrden detalles[], int *cantidadDetalles,
                    Repuesto repuestos[], int cantidadRepuestos);
void buscarOrden(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                 const DetalleOrden detalles[], int cantidadDetalles);
void cerrarOrden(OrdenTrabajo ordenes[], int cantidadOrdenes,
                 const DetalleOrden detalles[], int cantidadDetalles);
void menuOrdenes(OrdenTrabajo ordenes[], int *cantidadOrdenes, int *nroOrdenSiguiente,
                 DetalleOrden detalles[], int *cantidadDetalles,
                 const Cliente clientes[], int contadorClientes,
                 const Vehiculo vehiculos[], int contadorVehiculos,
                 Repuesto repuestos[], int cantidadRepuestos);

/* Módulo 4: Repuestos y Stock */
void cargarRepuesto(Repuesto repuestos[], int *cantidadRepuestos);
void eliminarRepuesto(Repuesto repuestos[], int *cantidadRepuestos);
void buscarRepuesto(const Repuesto repuestos[], int cantidadRepuestos);
void modificarRepuesto(Repuesto repuestos[], int cantidadRepuestos);
void listarRepuestos(const Repuesto repuestos[], int cantidadRepuestos);
void menuRepuestos(Repuesto repuestos[], int *cantidadRepuestos);

/* Módulo 5: Facturación */
void emitirFactura(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                   const DetalleOrden detalles[], int cantidadDetalles,
                   Factura facturas[], int *cantidadFacturas, int *nroFacturaSiguiente,
                   const Cliente clientes[], int contadorClientes);
void verComprobanteFactura(const Factura facturas[], int cantidadFacturas,
                           const DetalleOrden detalles[], int cantidadDetalles,
                           const Cliente clientes[], int contadorClientes);
void consultarFacturas(const Factura facturas[], int cantidadFacturas);
void menuFacturacion(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                     const DetalleOrden detalles[], int cantidadDetalles,
                     Factura facturas[], int *cantidadFacturas, int *nroFacturaSiguiente,
                     const Cliente clientes[], int contadorClientes);

/* Módulo 6: Historial de Reparaciones */
void consultarHistorialDetallado(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                                 const DetalleOrden detalles[], int cantidadDetalles,
                                 const Factura facturas[], int cantidadFacturas,
                                 const Cliente clientes[], int contadorClientes);
void listarHistorialGeneral(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                            const Factura facturas[], int cantidadFacturas);
void menuHistorial(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                   const DetalleOrden detalles[], int cantidadDetalles,
                   const Factura facturas[], int cantidadFacturas,
                   const Cliente clientes[], int contadorClientes);

/* Módulo 7: Usuarios y Sesiones */
void listarUsuarios(const Usuario usuarios[], int cantidadUsuarios);
void registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, const char *rolActivo);
void menuUsuarios(Usuario usuarios[], int *cantidadUsuarios, const char *rolActivo);

/* =====================================================================
 * ALGORITMO PRINCIPAL (MAIN)
 * ===================================================================== */
int main(void)
{
    /* Variables de Sesión y Usuarios */
    Usuario usuarios[MAX_USUARIOS];
    int cantidadUsuarios = 0;
    char usuarioActivo[LARGO_TEXTO] = "";
    char rolActivo[LARGO_ESTADO] = "";
    char rolElegido[LARGO_ESTADO] = "";
    char userIngresado[LARGO_TEXTO];
    char passIngresada[LARGO_TEXTO];
    int tipoUsuario = -1;
    int intentos = 0;
    int posUser = -1;
    int opcionMenu = -1;

    /* Variables de Clientes */
    Cliente clientes[MAX_CLIENTES];
    int contadorClientes = 0;

    /* Variables de Vehículos */
    Vehiculo vehiculos[MAX_VEHICULOS];
    int contadorVehiculo = 0;

    /* Variables de Repuestos */
    Repuesto repuestos[MAX_REPUESTOS];
    int cantidadRepuestos = 0;

    /* Variables de Órdenes de Trabajo */
    OrdenTrabajo ordenes[MAX_ORDENES];
    int cantidadOrdenes = 0;
    int nroOrdenSiguiente = 1;
    DetalleOrden detalles[MAX_DETALLES];
    int cantidadDetalles = 0;

    /* Variables de Facturación */
    Factura facturas[MAX_FACTURAS];
    int cantidadFacturas = 0;
    int nroFacturaSiguiente = 1;

    /* Configuración de Consola */
    utf8();
    activarColoresANSI();

    /* Carga de datos de prueba integrados tal como en Fixly.psc */
    cargarDatosPrueba(usuarios, &cantidadUsuarios,
                      clientes, &contadorClientes,
                      vehiculos, &contadorVehiculo,
                      repuestos, &cantidadRepuestos,
                      ordenes, &cantidadOrdenes, &nroOrdenSiguiente,
                      detalles, &cantidadDetalles,
                      facturas, &cantidadFacturas, &nroFacturaSiguiente);

    /* Pantalla de Bienvenida */
    limpiaPantalla();
    mostrarLogo();
    printf("  Sistema Integral de Gestión de Taller Mecánico\n\n\n");
    pausa();

    /* Bucle principal del sistema */
    do
    {
        /* 1. Selección del tipo de usuario */
        do
        {
            limpiaPantalla();
            mostrarLogo();
            printf("                 SELECCIONE EL TIPO DE USUARIO                    \n");
            printf("==================================================================\n");
            printf("  [1] Administrador\n");
            printf("  [2] Mecanico\n");
            printf("  [3] Cliente\n");
            printf("  ----------------------------------------------------------------\n");
            printf("  [0] Salir del Sistema\n");
            printf("==================================================================\n");
            tipoUsuario = leerEntero(">> Ingrese opción deseada: ");

            if (tipoUsuario < 0 || tipoUsuario > 3)
            {
                printf("  [!] Opción no válida.\n");
                pausa();
            }
        } while (tipoUsuario < 0 || tipoUsuario > 3);

        if (tipoUsuario != 0)
        {
            switch (tipoUsuario)
            {
                case 1:
                    strcpy(rolElegido, "administrador");
                    break;
                case 2:
                    strcpy(rolElegido, "mecanico");
                    break;
                case 3:
                    strcpy(rolElegido, "cliente");
                    break;
            }

            usuarioActivo[0] = '\0';
            rolActivo[0] = '\0';
            intentos = 0;

            /* 2. Inicio de sesión con 3 intentos */
            do
            {
                intentos++;
                limpiaPantalla();
                printf("==================================================================\n");
                printf("                     INICIAR SESIÓN EN FIXLY                      \n");
                printf("==================================================================\n");
                printf("  Tipo de usuario: %s | Intento %d de 3\n", rolElegido, intentos);
                printf("------------------------------------------------------------------\n");
                leerCadena(userIngresado, LARGO_TEXTO, ">> Nombre de usuario: ");

                posUser = buscarUsuarioPorNombre(usuarios, cantidadUsuarios, userIngresado);
                if (posUser == -1)
                {
                    printf("------------------------------------------------------------------\n");
                    printf("  [!] Error: Usuario no registrado.\n");
                    printf("==================================================================\n");
                    pausa();
                }
                else
                {
                    leerCadena(passIngresada, LARGO_TEXTO, ">> Contraseña: ");
                    if (!sonIguales(passIngresada, usuarios[posUser].clave))
                    {
                        printf("------------------------------------------------------------------\n");
                        printf("  [!] Error: Contraseña incorrecta.\n");
                        printf("==================================================================\n");
                    }
                    else
                    {
                        if (!sonIguales(usuarios[posUser].rol, rolElegido))
                        {
                            printf("------------------------------------------------------------------\n");
                            printf("  [!] Error: El usuario no corresponde al tipo seleccionado.\n");
                            printf("==================================================================\n");
                        }
                        else
                        {
                            strcpy(usuarioActivo, usuarios[posUser].nombre);
                            strcpy(rolActivo, usuarios[posUser].rol);
                            printf("------------------------------------------------------------------\n");
                            printf("[OK] Sesión iniciada correctamente.\n");
                            printf("Bienvenido/a %s! Rol activo: %s\n", usuarioActivo, rolActivo);
                            printf("==================================================================\n");
                        }
                    }
                    pausa();
                }
            } while (strlen(usuarioActivo) == 0 && intentos < 3);

            /* 3. Acceso a módulos según rol */
            if (strlen(usuarioActivo) == 0)
            {
                limpiaPantalla();
                printf("==================================================================\n");
                printf("  [!] Se superaron los 3 intentos permitidos.\n");
                printf("  Volviendo a la selección de tipo de usuario...\n");
                printf("==================================================================\n");
                pausa();
            }
            else
            {
                switch (tipoUsuario)
                {
                    case 1:
                        /* Rol: Administrador */
                        do
                        {
                            limpiaPantalla();
                            mostrarLogo();
                            printf("  >> USUARIO: %s | ROL: %s\n", usuarioActivo, rolActivo);
                            printf("==================================================================\n");
                            printf("  [1] Módulo Clientes              [5] Módulo Facturación\n");
                            printf("  [2] Módulo Vehículos             [6] Historial de Reparaciones\n");
                            printf("  [3] Ordenes de Trabajo           [7] Usuarios y Sesiones\n");
                            printf("  [4] Repuestos y Stock\n");
                            printf("  ----------------------------------------------------------------\n");
                            printf("  [0] Cerrar Sesion\n");
                            printf("==================================================================\n");
                            opcionMenu = leerEntero(">> Ingrese opción deseada: ");

                            switch (opcionMenu)
                            {
                                case 1:
                                    menuClientes(clientes, &contadorClientes);
                                    break;
                                case 2:
                                    menuVehiculos(vehiculos, &contadorVehiculo, clientes, &contadorClientes);
                                    break;
                                case 3:
                                    menuOrdenes(ordenes, &cantidadOrdenes, &nroOrdenSiguiente,
                                                detalles, &cantidadDetalles,
                                                clientes, contadorClientes,
                                                vehiculos, contadorVehiculo,
                                                repuestos, cantidadRepuestos);
                                    break;
                                case 4:
                                    menuRepuestos(repuestos, &cantidadRepuestos);
                                    break;
                                case 5:
                                    menuFacturacion(ordenes, cantidadOrdenes,
                                                    detalles, cantidadDetalles,
                                                    facturas, &cantidadFacturas, &nroFacturaSiguiente,
                                                    clientes, contadorClientes);
                                    break;
                                case 6:
                                    menuHistorial(ordenes, cantidadOrdenes,
                                                  detalles, cantidadDetalles,
                                                  facturas, cantidadFacturas,
                                                  clientes, contadorClientes);
                                    break;
                                case 7:
                                    menuUsuarios(usuarios, &cantidadUsuarios, rolActivo);
                                    break;
                                case 0:
                                    usuarioActivo[0] = '\0';
                                    rolActivo[0] = '\0';
                                    limpiaPantalla();
                                    printf("==================================================================\n");
                                    printf("  [OK] Sesión cerrada correctamente.\n");
                                    printf("==================================================================\n");
                                    pausa();
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                                    pausa();
                                    break;
                            }
                        } while (opcionMenu != 0);
                        break;

                    case 2:
                        /* Rol: Mecánico */
                        do
                        {
                            limpiaPantalla();
                            mostrarLogo();
                            printf("  >> USUARIO: %s | ROL: %s\n", usuarioActivo, rolActivo);
                            printf("==================================================================\n");
                            printf("  [1] Órdenes de Trabajo\n");
                            printf("  [2] Repuestos y Stock\n");
                            printf("  [3] Historial de Reparaciones\n");
                            printf("  ----------------------------------------------------------------\n");
                            printf("  [0] Cerrar Sesión\n");
                            printf("==================================================================\n");
                            opcionMenu = leerEntero(">> Ingrese opción deseada: ");

                            switch (opcionMenu)
                            {
                                case 1:
                                    menuOrdenes(ordenes, &cantidadOrdenes, &nroOrdenSiguiente,
                                                detalles, &cantidadDetalles,
                                                clientes, contadorClientes,
                                                vehiculos, contadorVehiculo,
                                                repuestos, cantidadRepuestos);
                                    break;
                                case 2:
                                    menuRepuestos(repuestos, &cantidadRepuestos);
                                    break;
                                case 3:
                                    menuHistorial(ordenes, cantidadOrdenes,
                                                  detalles, cantidadDetalles,
                                                  facturas, cantidadFacturas,
                                                  clientes, contadorClientes);
                                    break;
                                case 0:
                                    usuarioActivo[0] = '\0';
                                    rolActivo[0] = '\0';
                                    limpiaPantalla();
                                    printf("==================================================================\n");
                                    printf("  [OK] Sesión cerrada correctamente.\n");
                                    printf("==================================================================\n");
                                    pausa();
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                                    pausa();
                                    break;
                            }
                        } while (opcionMenu != 0);
                        break;

                    case 3:
                        /* Rol: Cliente */
                        do
                        {
                            limpiaPantalla();
                            mostrarLogo();
                            printf("  >> USUARIO: %s | ROL: %s\n", usuarioActivo, rolActivo);
                            printf("==================================================================\n");
                            printf("  [1] Historial de Reparaciones\n");
                            printf("  ----------------------------------------------------------------\n");
                            printf("  [0] Cerrar Sesión\n");
                            printf("==================================================================\n");
                            opcionMenu = leerEntero(">> Ingrese opción deseada: ");

                            switch (opcionMenu)
                            {
                                case 1:
                                    menuHistorial(ordenes, cantidadOrdenes,
                                                  detalles, cantidadDetalles,
                                                  facturas, cantidadFacturas,
                                                  clientes, contadorClientes);
                                    break;
                                case 0:
                                    usuarioActivo[0] = '\0';
                                    rolActivo[0] = '\0';
                                    limpiaPantalla();
                                    printf("==================================================================\n");
                                    printf("  [OK] Sesión cerrada correctamente.\n");
                                    printf("==================================================================\n");
                                    pausa();
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                                    pausa();
                                    break;
                            }
                        } while (opcionMenu != 0);
                        break;
                }
            }
        }
    } while (tipoUsuario != 0);

    limpiaPantalla();
    mostrarLogo();
    printf("        Muchas gracias por utilizar el Sistema Fixly!             \n");
    printf("            Sesión cerrada de manera segura.                      \n");
    printf("==================================================================\n");

    return 0;
}

/* =====================================================================
 * FUNCIONES GENERALES Y CARGA DE DATOS DE PRUEBA
 * ===================================================================== */

void mostrarLogo(void)
{
    printf("==================================================================\n");
    printf("  ______   _____  __     __  _      __     __\n");
    printf(" |  ____| |_   _| \\ \\   / / | |     \\ \\   / /\n");
    printf(" | |__      | |    \\ \\ / /  | |      \\ \\_/ / \n");
    printf(" |  __|     | |     \\   /   | |       \\   /  \n");
    printf(" | |        | |     /   \\   | |        | |   \n");
    printf(" | |       _| |_   / / \\ \\  | |____    | |   \n");
    printf(" |_|      |_____| /_/   \\_\\ |______|   |_|    GESTION DE TALLER\n");
    printf("==================================================================\n");
}

void cargarDatosPrueba(Usuario usuarios[], int *cantUsuarios,
                      Cliente clientes[], int *cantClientes,
                      Vehiculo vehiculos[], int *cantVehiculos,
                      Repuesto repuestos[], int *cantRepuestos,
                      OrdenTrabajo ordenes[], int *cantOrdenes, int *nroOrdenSig,
                      DetalleOrden detalles[], int *cantDetalles,
                      Factura facturas[], int *cantFacturas, int *nroFacturaSig)
{
    /* 1. Usuarios de prueba */
    *cantUsuarios = 3;
    strcpy(usuarios[0].nombre, "admin");
    strcpy(usuarios[0].clave, "admin123");
    strcpy(usuarios[0].rol, "administrador");

    strcpy(usuarios[1].nombre, "mecanico");
    strcpy(usuarios[1].clave, "meca123");
    strcpy(usuarios[1].rol, "mecanico");

    strcpy(usuarios[2].nombre, "cliente");
    strcpy(usuarios[2].clave, "cliente123");
    strcpy(usuarios[2].rol, "cliente");

    /* 2. Clientes de prueba */
    *cantClientes = 5;
    strcpy(clientes[0].nombre, "Carlos");
    strcpy(clientes[0].apellido, "Gomez");
    strcpy(clientes[0].celular, "1144556677");
    clientes[0].dni = 30111222;

    strcpy(clientes[1].nombre, "Mariana");
    strcpy(clientes[1].apellido, "Lopez");
    strcpy(clientes[1].celular, "1155667788");
    clientes[1].dni = 28555666;

    strcpy(clientes[2].nombre, "Jorge");
    strcpy(clientes[2].apellido, "Fernandez");
    strcpy(clientes[2].celular, "1166778899");
    clientes[2].dni = 35222111;

    strcpy(clientes[3].nombre, "Lucia");
    strcpy(clientes[3].apellido, "Diaz");
    strcpy(clientes[3].celular, "1177889900");
    clientes[3].dni = 40333444;

    strcpy(clientes[4].nombre, "Martin");
    strcpy(clientes[4].apellido, "Alvarez");
    strcpy(clientes[4].celular, "1188990011");
    clientes[4].dni = 27888999;

    /* 3. Vehículos de prueba */
    *cantVehiculos = 5;
    strcpy(vehiculos[0].marca, "Ford");
    strcpy(vehiculos[0].modelo, "Fiesta");
    strcpy(vehiculos[0].patente, "AB123CD");
    vehiculos[0].dniDueno = 30111222;

    strcpy(vehiculos[1].marca, "Chevrolet");
    strcpy(vehiculos[1].modelo, "Onix");
    strcpy(vehiculos[1].patente, "AC456EF");
    vehiculos[1].dniDueno = 28555666;

    strcpy(vehiculos[2].marca, "Toyota");
    strcpy(vehiculos[2].modelo, "Corolla");
    strcpy(vehiculos[2].patente, "AD789GH");
    vehiculos[2].dniDueno = 35222111;

    strcpy(vehiculos[3].marca, "Renault");
    strcpy(vehiculos[3].modelo, "Sandero");
    strcpy(vehiculos[3].patente, "AE012IJ");
    vehiculos[3].dniDueno = 40333444;

    strcpy(vehiculos[4].marca, "Volkswagen");
    strcpy(vehiculos[4].modelo, "Gol");
    strcpy(vehiculos[4].patente, "AF345KL");
    vehiculos[4].dniDueno = 27888999;

    /* 4. Repuestos de prueba */
    *cantRepuestos = 5;
    strcpy(repuestos[0].codigo, "ACE-01");
    strcpy(repuestos[0].nombre, "Aceite Sintetico 5W30");
    repuestos[0].precio = 35000.0f;
    repuestos[0].stock = 20;

    strcpy(repuestos[1].codigo, "FIL-01");
    strcpy(repuestos[1].nombre, "Filtro de Aceite");
    repuestos[1].precio = 12000.0f;
    repuestos[1].stock = 15;

    strcpy(repuestos[2].codigo, "PAS-01");
    strcpy(repuestos[2].nombre, "Pastillas de Freno Delanteras");
    repuestos[2].precio = 48000.0f;
    repuestos[2].stock = 10;

    strcpy(repuestos[3].codigo, "BUJ-01");
    strcpy(repuestos[3].nombre, "Juego de Bujias");
    repuestos[3].precio = 28000.0f;
    repuestos[3].stock = 12;

    strcpy(repuestos[4].codigo, "COR-01");
    strcpy(repuestos[4].nombre, "Correa de Distribucion");
    repuestos[4].precio = 65000.0f;
    repuestos[4].stock = 8;

    /* 5. Órdenes de prueba */
    *cantOrdenes = 2;
    *nroOrdenSig = 3;

    ordenes[0].numero = 1;
    ordenes[0].dniCliente = 30111222;
    strcpy(ordenes[0].patente, "AB123CD");
    strcpy(ordenes[0].fechaInicio, "01/10/2026");
    strcpy(ordenes[0].fechaFin, "03/10/2026");
    strcpy(ordenes[0].fechaEntrega, "03/10/2026");
    strcpy(ordenes[0].descripcion, "Service programado y cambio de fluidos");
    strcpy(ordenes[0].diagnostico, "Mantenimiento preventivo general");
    strcpy(ordenes[0].trabajo, "Cambio de aceite y filtro realizado");
    strcpy(ordenes[0].estado, "Finalizada");

    ordenes[1].numero = 2;
    ordenes[1].dniCliente = 28555666;
    strcpy(ordenes[1].patente, "AC456EF");
    strcpy(ordenes[1].fechaInicio, "10/10/2026");
    strcpy(ordenes[1].fechaFin, "");
    strcpy(ordenes[1].fechaEntrega, "");
    strcpy(ordenes[1].descripcion, "Ruido al frenar en rueda delantera");
    strcpy(ordenes[1].diagnostico, "Desgaste de pastillas de freno");
    strcpy(ordenes[1].trabajo, "");
    strcpy(ordenes[1].estado, "Pendiente");

    /* Detalles de la orden 1 */
    *cantDetalles = 2;
    detalles[0].nroOrden = 1;
    strcpy(detalles[0].codRepuesto, "ACE-01");
    detalles[0].cantidad = 1;
    detalles[0].precioUnitario = 35000.0f;

    detalles[1].nroOrden = 1;
    strcpy(detalles[1].codRepuesto, "FIL-01");
    detalles[1].cantidad = 1;
    detalles[1].precioUnitario = 12000.0f;

    /* Factura de prueba */
    *cantFacturas = 1;
    *nroFacturaSig = 2;
    facturas[0].numero = 1;
    facturas[0].nroOrden = 1;
    facturas[0].dni = 30111222;
    strcpy(facturas[0].patente, "AB123CD");
    strcpy(facturas[0].fecha, "03/10/2026");
    facturas[0].montos.repuestos = 47000.0f;
    facturas[0].montos.horas = 2.0f;
    facturas[0].montos.valorHora = 15000.0f;
    facturas[0].montos.manoObra = 30000.0f;
    facturas[0].montos.subtotal = 77000.0f;
    facturas[0].montos.iva = 16170.0f;
    facturas[0].montos.total = 93170.0f;
}

/* =====================================================================
 * FUNCIONES DE BÚSQUEDA
 * ===================================================================== */

int buscarClientePorDni(const Cliente clientes[], int cantidad, int dniBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (clientes[i].dni == dniBuscado)
            return i;
    }
    return -1;
}

int buscarVehiculoPorPatente(const Vehiculo vehiculos[], int cantidad, const char *patenteBuscada)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (sonIguales(vehiculos[i].patente, patenteBuscada))
            return i;
    }
    return -1;
}

int buscarRepuestoPorCodigo(const Repuesto repuestos[], int cantidad, const char *codigoBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (sonIguales(repuestos[i].codigo, codigoBuscado))
            return i;
    }
    return -1;
}

int buscarRepuestoPorNombre(const Repuesto repuestos[], int cantidad, const char *nombreBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (sonIguales(repuestos[i].nombre, nombreBuscado))
            return i;
    }
    return -1;
}

int buscarOrdenPorNumero(const OrdenTrabajo ordenes[], int cantidad, int nroBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (ordenes[i].numero == nroBuscado)
            return i;
    }
    return -1;
}

int buscarFacturaPorNumero(const Factura facturas[], int cantidad, int nroBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (facturas[i].numero == nroBuscado)
            return i;
    }
    return -1;
}

int buscarFacturaPorNroOrden(const Factura facturas[], int cantidad, int nroOrdenBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (facturas[i].nroOrden == nroOrdenBuscado)
            return i;
    }
    return -1;
}

int buscarUsuarioPorNombre(const Usuario usuarios[], int cantidad, const char *nombreBuscado)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (sonIguales(usuarios[i].nombre, nombreBuscado))
            return i;
    }
    return -1;
}

/* =====================================================================
 * MÓDULO 1: CLIENTES
 * ===================================================================== */

void ingresarCliente(Cliente clientes[], int *contadorClientes)
{
    int dniNuevo, posDniExistente;
    limpiaPantalla();
    printf("==================================================================\n");
    printf("                     REGISTRAR NUEVO CLIENTE                      \n");
    printf("==================================================================\n");

    if (*contadorClientes >= MAX_CLIENTES)
    {
        printf("  [!] Capacidad máxima de clientes alcanzada (límite %d).\n", MAX_CLIENTES);
        pausa();
        return;
    }

    printf(">> Ingrese los datos del nuevo cliente:\n\n");
    leerCadena(clientes[*contadorClientes].nombre, LARGO_TEXTO, "Nombre del cliente: ");
    leerCadena(clientes[*contadorClientes].apellido, LARGO_TEXTO, "Apellido del cliente: ");
    leerCadena(clientes[*contadorClientes].celular, LARGO_TEXTO, "Nro de contacto / Celular: ");

    do
    {
        dniNuevo = leerDni("DNI del cliente (Sin puntos): ");
        posDniExistente = buscarClientePorDni(clientes, *contadorClientes, dniNuevo);
        if (posDniExistente != -1)
        {
            printf("  [!] Ya existe un cliente registrado con el DNI %d (%s %s).\n",
                   dniNuevo, clientes[posDniExistente].apellido, clientes[posDniExistente].nombre);
        }
    } while (posDniExistente != -1);

    clientes[*contadorClientes].dni = dniNuevo;
    (*contadorClientes)++;

    printf("------------------------------------------------------------------\n");
    printf("[OK] Cliente registrado correctamente en el sistema.\n");
    printf("==================================================================\n");
    pausa();
}

void mostrarDatosClientes(const Cliente clientes[], int contadorClientes)
{
    int i;
    limpiaPantalla();
    printf("==================================================================\n");
    printf("                       LISTADO DE CLIENTES                        \n");
    printf("==================================================================\n");

    if (contadorClientes == 0)
    {
        printf("  [!] No hay clientes registrados actualmente.\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("%-4s| %-10s| %-24s| %-20s\n", "Nro", "DNI", "NOMBRE Y APELLIDO", "CONTACTO / CELULAR");
        printf("----+-----------+-------------------------+-----------------------\n");
        for (i = 0; i < contadorClientes; i++)
        {
            char nombreCompleto[LARGO_TEXTO * 2];
            snprintf(nombreCompleto, sizeof(nombreCompleto), "%s %s", clientes[i].nombre, clientes[i].apellido);
            printf("%-4d| %-10d| %-24s| %-20s\n",
                   i + 1, clientes[i].dni, nombreCompleto, clientes[i].celular);
        }
        printf("==================================================================\n");
        printf("Total de clientes registrados: %d\n", contadorClientes);
        printf("==================================================================\n");
    }
}

void eliminarCliente(Cliente clientes[], int *contadorClientes)
{
    int dniEliminar, i, lugarVacio = -1;
    limpiaPantalla();
    printf("==================================================================\n");
    printf("                         ELIMINAR CLIENTE                         \n");
    printf("==================================================================\n");

    dniEliminar = leerDni("Escriba el DNI del cliente que desea eliminar: ");

    for (i = 0; i < *contadorClientes; i++)
    {
        if (clientes[i].dni == dniEliminar)
        {
            lugarVacio = i;
            break;
        }
    }

    if (lugarVacio != -1)
    {
        for (i = lugarVacio; i < *contadorClientes - 1; i++)
        {
            clientes[i] = clientes[i + 1];
        }
        (*contadorClientes)--;
        printf("------------------------------------------------------------------\n");
        printf("[OK] Cliente eliminado correctamente del sistema.\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("[!] Error: El DNI no se encuentra en el registro.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void modificarCliente(Cliente clientes[], int contadorClientes)
{
    int datoBusquedaMod, datoCampoMod, dniNuevo, posDniExistente;
    if (contadorClientes == 0)
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                     MODIFICAR CLIENTE                            \n");
        printf("==================================================================\n");
        printf("  [!] No hay clientes registrados para modificar.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    do
    {
        mostrarDatosClientes(clientes, contadorClientes);
        printf("Seleccione el cliente que desea modificar:\n");
        datoBusquedaMod = leerEntero(">> Nro de cliente: ");
    } while (datoBusquedaMod <= 0 || datoBusquedaMod > contadorClientes);

    int idx = datoBusquedaMod - 1;

    printf("+----------------------------------------------------------------+\n");
    printf("|                   FICHA DEL CLIENTE SELECCIONADO               |\n");
    printf("+----------------------------------------------------------------+\n");
    printf("| NOMBRE:   %s\n", clientes[idx].nombre);
    printf("| APELLIDO: %s\n", clientes[idx].apellido);
    printf("| CELULAR:  %s\n", clientes[idx].celular);
    printf("| DNI:      %d\n", clientes[idx].dni);
    printf("+----------------------------------------------------------------+\n");

    do
    {
        printf("Seleccione el dato que desea modificar:\n");
        printf("  [1] Nombre\n");
        printf("  [2] Apellido\n");
        printf("  [3] Celular\n");
        printf("  [4] DNI\n");
        datoCampoMod = leerEntero(">> Opción: ");
    } while (datoCampoMod < 1 || datoCampoMod > 4);

    switch (datoCampoMod)
    {
        case 1:
            leerCadena(clientes[idx].nombre, LARGO_TEXTO, "Ingrese el nuevo NOMBRE: ");
            break;
        case 2:
            leerCadena(clientes[idx].apellido, LARGO_TEXTO, "Ingrese el nuevo APELLIDO: ");
            break;
        case 3:
            leerCadena(clientes[idx].celular, LARGO_TEXTO, "Ingrese el nuevo CELULAR: ");
            break;
        case 4:
            do
            {
                dniNuevo = leerDni("Ingrese el nuevo DNI: ");
                posDniExistente = buscarClientePorDni(clientes, contadorClientes, dniNuevo);
                if (posDniExistente != -1 && posDniExistente != idx)
                {
                    printf("  [!] Ya existe un cliente registrado con el DNI %d (%s %s).\n",
                           dniNuevo, clientes[posDniExistente].apellido, clientes[posDniExistente].nombre);
                }
            } while (posDniExistente != -1 && posDniExistente != idx);
            clientes[idx].dni = dniNuevo;
            break;
    }

    printf("------------------------------------------------------------------\n");
    printf("[OK] Dato del cliente actualizado correctamente.\n");
    printf("==================================================================\n");
    pausa();
}

void menuClientes(Cliente clientes[], int *contadorClientes)
{
    int opcionClientes;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                       GESTIÓN DE CLIENTES                        \n");
        printf("==================================================================\n");
        printf("  [1] Ingresar un nuevo cliente\n");
        printf("  [2] Ver lista de clientes\n");
        printf("  [3] Eliminar un cliente\n");
        printf("  [4] Modificar datos de un cliente\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionClientes = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionClientes)
        {
            case 1:
                ingresarCliente(clientes, contadorClientes);
                break;
            case 2:
                mostrarDatosClientes(clientes, *contadorClientes);
                pausa();
                break;
            case 3:
                eliminarCliente(clientes, contadorClientes);
                break;
            case 4:
                modificarCliente(clientes, *contadorClientes);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no valida.\n");
                pausa();
                break;
        }
    } while (opcionClientes != 0);
}

/* =====================================================================
 * MÓDULO 2: VEHÍCULOS
 * ===================================================================== */

void ingresarVehiculo(Vehiculo vehiculos[], int *contadorVehiculos, Cliente clientes[], int *contadorClientes)
{
    char patenteNueva[LARGO_PATENTE];
    char respCliente[10];
    int dniIngresado, posCliente, posPatenteExistente;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                    REGISTRAR NUEVO VEHÍCULO                      \n");
    printf("==================================================================\n");

    if (*contadorVehiculos >= MAX_VEHICULOS)
    {
        printf("  [!] Capacidad máxima de vehículos alcanzada (límite %d).\n", MAX_VEHICULOS);
        pausa();
        return;
    }

    printf(">> Ingrese los datos del vehículo a registrar:\n\n");
    leerCadena(patenteNueva, LARGO_PATENTE, "Patente del vehículo: ");

    posPatenteExistente = buscarVehiculoPorPatente(vehiculos, *contadorVehiculos, patenteNueva);
    if (posPatenteExistente != -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: Ya existe un vehículo registrado con la patente %s\n", patenteNueva);
        printf("==================================================================\n");
        pausa();
        return;
    }

    strcpy(vehiculos[*contadorVehiculos].patente, patenteNueva);
    leerCadena(vehiculos[*contadorVehiculos].marca, LARGO_TEXTO, "Marca del vehículo: ");
    leerCadena(vehiculos[*contadorVehiculos].modelo, LARGO_TEXTO, "Modelo del vehículo: ");

    dniIngresado = leerDni("DNI del titular / dueño (Sin puntos): ");
    posCliente = buscarClientePorDni(clientes, *contadorClientes, dniIngresado);

    if (posCliente == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Aviso: El DNI %d no corresponde a un cliente registrado.\n", dniIngresado);
        leerCadena(respCliente, sizeof(respCliente), "  Desea dar de alta al cliente en este momento? (S/N): ");

        if (respCliente[0] == 'S' || respCliente[0] == 's')
        {
            if (*contadorClientes < MAX_CLIENTES)
            {
                leerCadena(clientes[*contadorClientes].nombre, LARGO_TEXTO, "Nombre del titular: ");
                leerCadena(clientes[*contadorClientes].apellido, LARGO_TEXTO, "Apellido del titular: ");
                leerCadena(clientes[*contadorClientes].celular, LARGO_TEXTO, "Celular de contacto: ");
                clientes[*contadorClientes].dni = dniIngresado;
                (*contadorClientes)++;
                printf("  [OK] Titular registrado y asociado exitosamente.\n");
            }
        }
        else
        {
            printf("  Aviso: El vehículo queda registrado con DNI asignado (sin ficha personal).\n");
        }
    }
    else
    {
        printf("  >> Titular identificado: %s %s\n", clientes[posCliente].apellido, clientes[posCliente].nombre);
    }

    vehiculos[*contadorVehiculos].dniDueno = dniIngresado;
    (*contadorVehiculos)++;

    printf("------------------------------------------------------------------\n");
    printf("[OK] Vehículo registrado correctamente en el sistema.\n");
    printf("==================================================================\n");
    pausa();
}

void mostrarDatosVehiculos(const Vehiculo vehiculos[], int contadorVehiculos, const Cliente clientes[], int contadorClientes)
{
    int i, posCliente;
    limpiaPantalla();
    printf("==================================================================================\n");
    printf("                              LISTADO DE VEHÍCULOS                                \n");
    printf("==================================================================================\n");

    if (contadorVehiculos == 0)
    {
        printf("  [!] No hay vehículos registrados en el sistema.\n");
        printf("==================================================================================\n");
    }
    else
    {
        printf("%-4s| %-9s| %-13s| %-13s| %-10s| %-24s\n", "Nro", "PATENTE", "MARCA", "MODELO", "DNI DUEÑO", "TITULAR / DUEÑO");
        printf("----+----------+--------------+--------------+-----------+------------------------\n");
        for (i = 0; i < contadorVehiculos; i++)
        {
            posCliente = buscarClientePorDni(clientes, contadorClientes, vehiculos[i].dniDueno);
            if (posCliente != -1)
            {
                char nombreCompleto[LARGO_TEXTO * 2];
                snprintf(nombreCompleto, sizeof(nombreCompleto), "%s %s", clientes[posCliente].apellido, clientes[posCliente].nombre);
                printf("%-4d| %-9s| %-13s| %-13s| %-10d| %-24s\n",
                       i + 1, vehiculos[i].patente, vehiculos[i].marca, vehiculos[i].modelo,
                       vehiculos[i].dniDueno, nombreCompleto);
            }
            else
            {
                printf("%-4d| %-9s| %-13s| %-13s| %-10d| (Sin ficha de cliente)\n",
                       i + 1, vehiculos[i].patente, vehiculos[i].marca, vehiculos[i].modelo,
                       vehiculos[i].dniDueno);
            }
        }
        printf("==================================================================================\n");
        printf("Total de vehículos registrados: %d\n", contadorVehiculos);
        printf("==================================================================================\n");
    }
}

void eliminarVehiculo(Vehiculo vehiculos[], int *contadorVehiculos)
{
    char patenteEliminar[LARGO_PATENTE];
    int posVehiculo, j;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                       ELIMINAR VEHÍCULO                          \n");
    printf("==================================================================\n");
    leerCadena(patenteEliminar, LARGO_PATENTE, "Escriba la patente del vehículo que desea eliminar: ");
    posVehiculo = buscarVehiculoPorPatente(vehiculos, *contadorVehiculos, patenteEliminar);

    if (posVehiculo != -1)
    {
        for (j = posVehiculo; j < *contadorVehiculos - 1; j++)
        {
            vehiculos[j] = vehiculos[j + 1];
        }
        (*contadorVehiculos)--;
        printf("------------------------------------------------------------------\n");
        printf("[OK] Vehículo eliminado correctamente del registro.\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("[!] Error: La patente no se encuentra registrada.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void modificarVehiculo(Vehiculo vehiculos[], int contadorVehiculos, const Cliente clientes[], int contadorClientes)
{
    int datoBusquedaMod, datoVehiculoMod, nuevoDni;
    if (contadorVehiculos == 0)
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                     MODIFICAR VEHÍCULO                           \n");
        printf("==================================================================\n");
        printf("  [!] No hay vehículos registrados para modificar.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    do
    {
        mostrarDatosVehiculos(vehiculos, contadorVehiculos, clientes, contadorClientes);
        printf("Seleccione el vehículo que desea modificar:\n");
        datoBusquedaMod = leerEntero(">> Nro de vehículo: ");
    } while (datoBusquedaMod <= 0 || datoBusquedaMod > contadorVehiculos);

    int idx = datoBusquedaMod - 1;

    printf("+----------------------------------------------------------------+\n");
    printf("| Vehículo: %s %s | Patente: %s\n", vehiculos[idx].marca, vehiculos[idx].modelo, vehiculos[idx].patente);
    printf("+----------------------------------------------------------------+\n");

    do
    {
        printf("Seleccione el campo que desea modificar:\n");
        printf("  [1] Marca\n");
        printf("  [2] Modelo\n");
        printf("  [3] Patente\n");
        printf("  [4] DNI del titular\n");
        datoVehiculoMod = leerEntero(">> Opción: ");
    } while (datoVehiculoMod < 1 || datoVehiculoMod > 4);

    switch (datoVehiculoMod)
    {
        case 1:
            leerCadena(vehiculos[idx].marca, LARGO_TEXTO, "Ingrese la nueva MARCA: ");
            break;
        case 2:
            leerCadena(vehiculos[idx].modelo, LARGO_TEXTO, "Ingrese el nuevo MODELO: ");
            break;
        case 3:
            leerCadena(vehiculos[idx].patente, LARGO_PATENTE, "Ingrese la nueva PATENTE: ");
            break;
        case 4:
            nuevoDni = leerDni("Ingrese el nuevo DNI del titular: ");
            vehiculos[idx].dniDueno = nuevoDni;
            break;
    }

    printf("------------------------------------------------------------------\n");
    printf("[OK] Datos del vehículo actualizados correctamente.\n");
    printf("==================================================================\n");
    pausa();
}

void menuVehiculos(Vehiculo vehiculos[], int *contadorVehiculos, Cliente clientes[], int *contadorClientes)
{
    int opcionVehiculos;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                      GESTIÓN DE VEHICULOS                        \n");
        printf("==================================================================\n");
        printf("  [1] Ingresar un nuevo vehículo\n");
        printf("  [2] Ver lista de vehículos\n");
        printf("  [3] Eliminar un vehículo\n");
        printf("  [4] Modificar datos de un vehículo\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionVehiculos = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionVehiculos)
        {
            case 1:
                ingresarVehiculo(vehiculos, contadorVehiculos, clientes, contadorClientes);
                break;
            case 2:
                mostrarDatosVehiculos(vehiculos, *contadorVehiculos, clientes, *contadorClientes);
                pausa();
                break;
            case 3:
                eliminarVehiculo(vehiculos, contadorVehiculos);
                break;
            case 4:
                modificarVehiculo(vehiculos, *contadorVehiculos, clientes, *contadorClientes);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionVehiculos != 0);
}

/* =====================================================================
 * MÓDULO 3: ÓRDENES DE TRABAJO
 * ===================================================================== */

void cargarOrden(OrdenTrabajo ordenes[], int *cantidadOrdenes, int *nroOrdenSiguiente,
                 DetalleOrden detalles[], int *cantidadDetalles,
                 const Cliente clientes[], int contadorClientes,
                 const Vehiculo vehiculos[], int contadorVehiculos,
                 Repuesto repuestos[], int cantidadRepuestos)
{
    int posicionCliente, posicionVehiculo, posicionRepuesto, dniBuscar;
    int cantidadRepuestosOt, vehiculosEnTaller = 0, i;
    char patenteBuscar[LARGO_PATENTE];
    char confirmaRepuesto[10];
    char fallaDetalle[LARGO_DETALLE];
    char diagDetalle[LARGO_DETALLE];
    char repuestoBuscar[LARGO_DESCRIPCION];
    char fInicio[LARGO_FECHA];
    char fEstimada[LARGO_FECHA];
    bool ordenPendiente = false;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                 CARGAR NUEVA ORDEN DE TRABAJO                    \n");
    printf("==================================================================\n");

    for (i = 0; i < *cantidadOrdenes; i++)
    {
        if (sonIguales(ordenes[i].estado, "Pendiente"))
            vehiculosEnTaller++;
    }

    if (vehiculosEnTaller >= CAPACIDAD_TALLER)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] No hay espacio disponible en el taller.\n");
        printf("  Vehículos actualmente en reparación: %d\n", vehiculosEnTaller);
        printf("  Capacidad máxima permitida: %d\n", CAPACIDAD_TALLER);
        printf("==================================================================\n");
        pausa();
        return;
    }

    dniBuscar = leerDni(">> DNI del cliente (Sin puntos): ");
    posicionCliente = buscarClientePorDni(clientes, contadorClientes, dniBuscar);

    if (posicionCliente == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] El DNI %d no existe en la base.\n", dniBuscar);
        printf("  Por favor, cargue primero el cliente en el Módulo 1.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    printf(">> Cliente encontrado: %s, %s\n", clientes[posicionCliente].apellido, clientes[posicionCliente].nombre);
    leerCadena(patenteBuscar, LARGO_PATENTE, ">> Patente del vehículo: ");

    posicionVehiculo = buscarVehiculoPorPatente(vehiculos, contadorVehiculos, patenteBuscar);
    if (posicionVehiculo == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] La patente %s no existe en la base.\n", patenteBuscar);
        printf("  Por favor, cargue primero el vehículo en el Módulo 2.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (vehiculos[posicionVehiculo].dniDueno != dniBuscar)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: El vehículo con patente %s no pertenece al DNI %d.\n", patenteBuscar, dniBuscar);
        printf("==================================================================\n");
        pausa();
        return;
    }

    printf(">> Vehículo: %s %s\n", vehiculos[posicionVehiculo].marca, vehiculos[posicionVehiculo].modelo);
    printf("------------------------------------------------------------------\n");

    for (i = 0; i < *cantidadOrdenes; i++)
    {
        if (sonIguales(ordenes[i].patente, patenteBuscar) && sonIguales(ordenes[i].estado, "Pendiente"))
        {
            ordenPendiente = true;
            break;
        }
    }

    if (ordenPendiente)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] No es posible registrar una nueva orden.\n");
        printf("  El vehículo ya posee una orden de trabajo pendiente.\n");
        printf("  Utilice la opción Modificar Orden para actualizar la reparación.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    leerCadena(fInicio, LARGO_FECHA, ">> Fecha de ingreso (DD/MM/AAAA) [ENTER para Hoy]: ");
    if (strlen(fInicio) == 0)
        strcpy(fInicio, "Hoy");

    leerCadena(fEstimada, LARGO_FECHA, ">> Fecha estimada de finalización (DD/MM/AAAA): ");
    leerCadena(fallaDetalle, LARGO_DETALLE, ">> Motivo de ingreso / Detalle de la falla: ");
    leerCadena(diagDetalle, LARGO_DETALLE, ">> Diagnóstico inicial: ");

    int idxOrden = *cantidadOrdenes;
    ordenes[idxOrden].numero = *nroOrdenSiguiente;
    (*nroOrdenSiguiente)++;

    ordenes[idxOrden].dniCliente = dniBuscar;
    strcpy(ordenes[idxOrden].patente, patenteBuscar);
    strcpy(ordenes[idxOrden].fechaInicio, fInicio);
    strcpy(ordenes[idxOrden].fechaFin, fEstimada);
    strcpy(ordenes[idxOrden].fechaEntrega, "");
    strcpy(ordenes[idxOrden].descripcion, fallaDetalle);
    strcpy(ordenes[idxOrden].diagnostico, diagDetalle);
    strcpy(ordenes[idxOrden].trabajo, "");
    strcpy(ordenes[idxOrden].estado, "Pendiente");

    printf("------------------------------------------------------------------\n");
    leerCadena(confirmaRepuesto, sizeof(confirmaRepuesto), ">> Desea agregar repuestos a la orden ahora? (S/N): ");

    while (confirmaRepuesto[0] == 'S' || confirmaRepuesto[0] == 's')
    {
        leerCadena(repuestoBuscar, LARGO_DESCRIPCION, ">> Código o nombre del repuesto (o FIN para terminar): ");
        if (!sonIguales(repuestoBuscar, "FIN") && !sonIguales(repuestoBuscar, "fin") && strlen(repuestoBuscar) > 0)
        {
            posicionRepuesto = buscarRepuestoPorCodigo(repuestos, cantidadRepuestos, repuestoBuscar);
            if (posicionRepuesto == -1)
                posicionRepuesto = buscarRepuestoPorNombre(repuestos, cantidadRepuestos, repuestoBuscar);

            if (posicionRepuesto == -1)
            {
                printf("  [!] El repuesto no existe en el catálogo. Cárguelo en el módulo 4.\n");
            }
            else
            {
                printf("  Artículo: %s | Stock disponible: %d | Precio: $%.2f\n",
                       repuestos[posicionRepuesto].nombre, repuestos[posicionRepuesto].stock, repuestos[posicionRepuesto].precio);
                cantidadRepuestosOt = leerEntero("  Cantidad requerida: ");

                if (cantidadRepuestosOt <= 0)
                {
                    printf("  [!] La cantidad debe ser mayor a cero.\n");
                }
                else if (cantidadRepuestosOt > repuestos[posicionRepuesto].stock)
                {
                    printf("  [!] Stock insuficiente. Disponible: %d\n", repuestos[posicionRepuesto].stock);
                }
                else if (*cantidadDetalles >= MAX_DETALLES)
                {
                    printf("  [!] Límite máximo de repuestos asignados a órdenes alcanzado (%d).\n", MAX_DETALLES);
                }
                else
                {
                    detalles[*cantidadDetalles].nroOrden = ordenes[idxOrden].numero;
                    strcpy(detalles[*cantidadDetalles].codRepuesto, repuestos[posicionRepuesto].codigo);
                    detalles[*cantidadDetalles].cantidad = cantidadRepuestosOt;
                    detalles[*cantidadDetalles].precioUnitario = repuestos[posicionRepuesto].precio;
                    repuestos[posicionRepuesto].stock -= cantidadRepuestosOt;
                    (*cantidadDetalles)++;
                    printf("  [OK] Repuesto asignado a la orden y descontado del stock.\n");
                }
            }
            leerCadena(confirmaRepuesto, sizeof(confirmaRepuesto), ">> Desea agregar otro repuesto? (S/N): ");
        }
        else
        {
            confirmaRepuesto[0] = 'N';
        }
    }

    (*cantidadOrdenes)++;
    printf("------------------------------------------------------------------\n");
    printf("[OK] Orden Nro %d registrada con éxito.\n", ordenes[idxOrden].numero);
    printf("==================================================================\n");
    pausa();
}

void modificarOrden(OrdenTrabajo ordenes[], int cantidadOrdenes,
                    DetalleOrden detalles[], int *cantidadDetalles,
                    Repuesto repuestos[], int cantidadRepuestos)
{
    char datoBuscar[LARGO_TEXTO];
    char codigoQuitar[LARGO_CODIGO];
    char fallaDetalle[LARGO_DETALLE];
    char diagDetalle[LARGO_DETALLE];
    char nombreRepuestoModificar[LARGO_DESCRIPCION];
    int posicionOrden = -1, posicionRepuestoModificar, posicionDetalle;
    int opcionModificarOt, i, j, cantidadRepuestosOt, tieneRepuestos;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                   MODIFICAR ORDEN DE TRABAJO                     \n");
    printf("==================================================================\n");
    leerCadena(datoBuscar, LARGO_TEXTO, ">> Ingrese patente o número de orden a modificar: ");

    for (i = 0; i < cantidadOrdenes; i++)
    {
        char nroTxt[20];
        snprintf(nroTxt, sizeof(nroTxt), "%d", ordenes[i].numero);
        if (sonIguales(ordenes[i].patente, datoBuscar) || sonIguales(nroTxt, datoBuscar))
        {
            posicionOrden = i;
            break;
        }
    }

    if (posicionOrden == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: No se encontró orden registrada para: %s\n", datoBuscar);
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (sonIguales(ordenes[posicionOrden].estado, "Finalizada"))
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] La orden Nro %d ya está Finalizada y no puede modificarse.\n", ordenes[posicionOrden].numero);
        printf("==================================================================\n");
        pausa();
        return;
    }

    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                 MODIFICAR ORDEN NRO %d\n", ordenes[posicionOrden].numero);
        printf("==================================================================\n");
        printf("  [1] Modificar motivo de falla\n");
        printf("  [2] Modificar diagnóstico\n");
        printf("  [3] Agregar repuestos a la orden\n");
        printf("  [4] Eliminar repuesto de la orden\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu de órdenes\n");
        printf("==================================================================\n");
        opcionModificarOt = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionModificarOt)
        {
            case 1:
                printf(">> Motivo actual: %s\n", ordenes[posicionOrden].descripcion);
                leerCadena(fallaDetalle, LARGO_DETALLE, ">> Nuevo motivo de falla: ");
                if (strlen(fallaDetalle) > 0)
                {
                    strcpy(ordenes[posicionOrden].descripcion, fallaDetalle);
                    printf("------------------------------------------------------------------\n");
                    printf("[OK] Motivo de falla actualizado correctamente.\n");
                    printf("==================================================================\n");
                }
                else
                {
                    printf("  [!] El detalle no puede estar vacío.\n");
                }
                pausa();
                break;

            case 2:
                printf(">> Diagnóstico actual: %s\n", ordenes[posicionOrden].diagnostico);
                leerCadena(diagDetalle, LARGO_DETALLE, ">> Nuevo diagnóstico: ");
                if (strlen(diagDetalle) > 0)
                {
                    strcpy(ordenes[posicionOrden].diagnostico, diagDetalle);
                    printf("------------------------------------------------------------------\n");
                    printf("[OK] Diagnóstico actualizado correctamente.\n");
                    printf("==================================================================\n");
                }
                else
                {
                    printf("  [!] El diagnóstico no puede estar vacío.\n");
                }
                pausa();
                break;

            case 3:
                leerCadena(nombreRepuestoModificar, LARGO_DESCRIPCION, ">> Código o nombre del repuesto a agregar: ");
                posicionRepuestoModificar = buscarRepuestoPorCodigo(repuestos, cantidadRepuestos, nombreRepuestoModificar);
                if (posicionRepuestoModificar == -1)
                    posicionRepuestoModificar = buscarRepuestoPorNombre(repuestos, cantidadRepuestos, nombreRepuestoModificar);

                if (posicionRepuestoModificar == -1)
                {
                    printf("  [!] Repuesto no encontrado en el catálogo.\n");
                }
                else
                {
                    printf("  Artículo: %s | Stock actual: %d | Precio: $%.2f\n",
                           repuestos[posicionRepuestoModificar].nombre,
                           repuestos[posicionRepuestoModificar].stock,
                           repuestos[posicionRepuestoModificar].precio);
                    cantidadRepuestosOt = leerEntero("  Cantidad requerida: ");

                    if (cantidadRepuestosOt <= 0)
                    {
                        printf("  [!] Cantidad inválida (debe ser mayor a cero).\n");
                    }
                    else if (cantidadRepuestosOt > repuestos[posicionRepuestoModificar].stock)
                    {
                        printf("  [!] Stock insuficiente en inventario.\n");
                    }
                    else if (*cantidadDetalles >= MAX_DETALLES)
                    {
                        printf("  [!] Límite máximo de repuestos asignados a órdenes alcanzado (%d).\n", MAX_DETALLES);
                    }
                    else
                    {
                        detalles[*cantidadDetalles].nroOrden = ordenes[posicionOrden].numero;
                        strcpy(detalles[*cantidadDetalles].codRepuesto, repuestos[posicionRepuestoModificar].codigo);
                        detalles[*cantidadDetalles].cantidad = cantidadRepuestosOt;
                        detalles[*cantidadDetalles].precioUnitario = repuestos[posicionRepuestoModificar].precio;
                        (*cantidadDetalles)++;
                        repuestos[posicionRepuestoModificar].stock -= cantidadRepuestosOt;
                        printf("  [OK] Repuesto agregado y descontado del stock general.\n");
                    }
                }
                pausa();
                break;

            case 4:
                tieneRepuestos = 0;
                for (i = 0; i < *cantidadDetalles; i++)
                {
                    if (detalles[i].nroOrden == ordenes[posicionOrden].numero)
                        tieneRepuestos++;
                }

                if (tieneRepuestos == 0)
                {
                    printf("  [!] La orden no tiene repuestos asignados.\n");
                }
                else
                {
                    printf("------------------------------------------------------------------\n");
                    printf("Repuestos asignados en la orden:\n");
                    for (i = 0; i < *cantidadDetalles; i++)
                    {
                        if (detalles[i].nroOrden == ordenes[posicionOrden].numero)
                        {
                            printf("  - Código: %s (x%d)\n", detalles[i].codRepuesto, detalles[i].cantidad);
                        }
                    }
                    printf("------------------------------------------------------------------\n");
                    leerCadena(codigoQuitar, LARGO_CODIGO, ">> Código de repuesto a eliminar: ");

                    posicionDetalle = -1;
                    for (i = 0; i < *cantidadDetalles; i++)
                    {
                        if (detalles[i].nroOrden == ordenes[posicionOrden].numero &&
                            sonIguales(detalles[i].codRepuesto, codigoQuitar))
                        {
                            posicionDetalle = i;
                            break;
                        }
                    }

                    if (posicionDetalle == -1)
                    {
                        printf("  [!] El repuesto no esta asignado a esta orden.\n");
                    }
                    else
                    {
                        posicionRepuestoModificar = buscarRepuestoPorCodigo(repuestos, cantidadRepuestos, codigoQuitar);
                        if (posicionRepuestoModificar != -1)
                        {
                            repuestos[posicionRepuestoModificar].stock += detalles[posicionDetalle].cantidad;
                        }

                        for (j = posicionDetalle; j < *cantidadDetalles - 1; j++)
                        {
                            detalles[j] = detalles[j + 1];
                        }
                        (*cantidadDetalles)--;
                        printf("------------------------------------------------------------------\n");
                        printf("[OK] Repuesto quitado de la orden y restaurado al stock.\n");
                        printf("==================================================================\n");
                    }
                }
                pausa();
                break;

            case 0:
                printf("Volviendo al menu de órdenes...\n");
                break;

            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionModificarOt != 0);
}

void buscarOrden(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                 const DetalleOrden detalles[], int cantidadDetalles)
{
    char datoBuscar[LARGO_TEXTO];
    int i, k, encontradas = 0;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                 BUSCAR ORDEN DE TRABAJO                          \n");
    printf("==================================================================\n");
    leerCadena(datoBuscar, LARGO_TEXTO, ">> Patente o número de orden a buscar: ");

    for (i = 0; i < cantidadOrdenes; i++)
    {
        char nroTxt[20];
        snprintf(nroTxt, sizeof(nroTxt), "%d", ordenes[i].numero);
        if (sonIguales(ordenes[i].patente, datoBuscar) || sonIguales(nroTxt, datoBuscar))
            encontradas++;
    }

    if (encontradas == 0)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: No hay orden registrada para: %s\n", datoBuscar);
        printf("==================================================================\n");
    }
    else
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("  ÓRDENES ENCONTRADAS PARA: %s\n", datoBuscar);
        printf("==================================================================\n");

        for (i = 0; i < cantidadOrdenes; i++)
        {
            char nroTxt[20];
            snprintf(nroTxt, sizeof(nroTxt), "%d", ordenes[i].numero);
            if (sonIguales(ordenes[i].patente, datoBuscar) || sonIguales(nroTxt, datoBuscar))
            {
                printf("+================================================================+\n");
                printf("|                   ORDEN DE TRABAJO Nro %d\n", ordenes[i].numero);
                printf("+================================================================+\n");
                printf("| PATENTE:        %s\n", ordenes[i].patente);
                printf("| DNI TITULAR:    %d\n", ordenes[i].dniCliente);
                printf("| ESTADO ACTUAL:  %s\n", ordenes[i].estado);
                printf("| FECHA INGRESO:  %s\n", ordenes[i].fechaInicio);
                if (strlen(ordenes[i].fechaFin) > 0)
                    printf("| FECHA ESTIMADA: %s\n", ordenes[i].fechaFin);
                if (sonIguales(ordenes[i].estado, "Finalizada"))
                    printf("| FECHA ENTREGA:  %s\n", ordenes[i].fechaEntrega);
                printf("+----------------------------------------------------------------+\n");
                printf("| MOTIVO INGRESO: %s\n", ordenes[i].descripcion);
                printf("| DIAGNÓSTICO:    %s\n", ordenes[i].diagnostico);
                if (strlen(ordenes[i].trabajo) > 0)
                    printf("| TRABAJO HECHO:  %s\n", ordenes[i].trabajo);
                printf("+----------------------------------------------------------------+\n");
                printf("| REPUESTOS UTILIZADOS:\n");
                for (k = 0; k < cantidadDetalles; k++)
                {
                    if (detalles[k].nroOrden == ordenes[i].numero)
                    {
                        printf("|   - %s (x%d) - $%.2f\n",
                               detalles[k].codRepuesto, detalles[k].cantidad,
                               detalles[k].precioUnitario * detalles[k].cantidad);
                    }
                }
                printf("+================================================================+\n\n");
            }
        }
        printf("==================================================================\n");
        printf("  Total de órdenes encontradas: %d\n", encontradas);
        printf("==================================================================\n");
    }
    pausa();
}

void cerrarOrden(OrdenTrabajo ordenes[], int cantidadOrdenes,
                 const DetalleOrden detalles[], int cantidadDetalles)
{
    char datoBuscar[LARGO_TEXTO];
    char confirmaCierre[10];
    char fechaIngresada[LARGO_FECHA];
    char trabajoRealizado[LARGO_DETALLE];
    int posicionOrden = -1, i;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                 CERRAR ORDEN DE TRABAJO                          \n");
    printf("==================================================================\n");
    leerCadena(datoBuscar, LARGO_TEXTO, ">> Patente o número de orden a cerrar: ");

    for (i = 0; i < cantidadOrdenes; i++)
    {
        char nroTxt[20];
        snprintf(nroTxt, sizeof(nroTxt), "%d", ordenes[i].numero);
        if (sonIguales(ordenes[i].patente, datoBuscar) || sonIguales(nroTxt, datoBuscar))
        {
            posicionOrden = i;
            break;
        }
    }

    if (posicionOrden == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: No hay orden registrada para: %s\n", datoBuscar);
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (sonIguales(ordenes[posicionOrden].estado, "Finalizada"))
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] La orden Nro %d ya se encuentra Finalizada.\n", ordenes[posicionOrden].numero);
        printf("==================================================================\n");
        pausa();
        return;
    }

    limpiaPantalla();
    printf("+================================================================+\n");
    printf("|             RESUMEN DE ORDEN NRO %d\n", ordenes[posicionOrden].numero);
    printf("+================================================================+\n");
    printf("| PATENTE:        %s\n", ordenes[posicionOrden].patente);
    printf("| DNI TITULAR:    %d\n", ordenes[posicionOrden].dniCliente);
    printf("| FECHA INGRESO:  %s\n", ordenes[posicionOrden].fechaInicio);
    if (strlen(ordenes[posicionOrden].fechaFin) > 0)
        printf("| FECHA ESTIMADA: %s\n", ordenes[posicionOrden].fechaFin);
    printf("| ESTADO ACTUAL:  %s\n", ordenes[posicionOrden].estado);
    printf("| MOTIVO INGRESO: %s\n", ordenes[posicionOrden].descripcion);
    printf("| DIAGNÓSTICO:    %s\n", ordenes[posicionOrden].diagnostico);
    printf("+----------------------------------------------------------------+\n");
    printf("| REPUESTOS UTILIZADOS:\n");
    for (i = 0; i < cantidadDetalles; i++)
    {
        if (detalles[i].nroOrden == ordenes[posicionOrden].numero)
        {
            printf("|   - %s (x%d) = $%.2f\n",
                   detalles[i].codRepuesto, detalles[i].cantidad,
                   detalles[i].precioUnitario * detalles[i].cantidad);
        }
    }
    printf("+================================================================+\n");

    leerCadena(fechaIngresada, LARGO_FECHA, ">> Fecha de finalización / entrega (DD/MM/AAAA): ");
    if (strlen(fechaIngresada) == 0)
        strcpy(fechaIngresada, "Hoy");

    leerCadena(trabajoRealizado, LARGO_DETALLE, ">> Detalle de trabajo realizado por el mecánico: ");

    printf("------------------------------------------------------------------\n");
    leerCadena(confirmaCierre, sizeof(confirmaCierre), "Confirmar cierre de la orden? (S/N): ");

    if (confirmaCierre[0] == 'S' || confirmaCierre[0] == 's')
    {
        strcpy(ordenes[posicionOrden].estado, "Finalizada");
        strcpy(ordenes[posicionOrden].fechaFin, fechaIngresada);
        strcpy(ordenes[posicionOrden].fechaEntrega, fechaIngresada);
        strcpy(ordenes[posicionOrden].trabajo, trabajoRealizado);

        printf("==================================================================\n");
        printf("  [OK] Orden cerrada correctamente.\n");
        printf("  Nro de orden:   %d\n", ordenes[posicionOrden].numero);
        printf("  Estado nuevo:   %s\n", ordenes[posicionOrden].estado);
        printf("  Fecha entrega:  %s\n", ordenes[posicionOrden].fechaEntrega);
        printf("==================================================================\n");
        printf("  La orden esta lista para facturación (Módulo 5).\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Cierre de orden cancelado.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void menuOrdenes(OrdenTrabajo ordenes[], int *cantidadOrdenes, int *nroOrdenSiguiente,
                 DetalleOrden detalles[], int *cantidadDetalles,
                 const Cliente clientes[], int contadorClientes,
                 const Vehiculo vehiculos[], int contadorVehiculos,
                 Repuesto repuestos[], int cantidadRepuestos)
{
    int opcionOrdenes;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                   ÓRDENES DE TRABAJO (TALLER)                    \n");
        printf("==================================================================\n");
        printf("  [1] Cargar nueva orden de trabajo\n");
        printf("  [2] Modificar orden de trabajo\n");
        printf("  [3] Buscar orden por patente o número\n");
        printf("  [4] Cerrar / Finalizar orden de trabajo\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionOrdenes = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionOrdenes)
        {
            case 1:
                cargarOrden(ordenes, cantidadOrdenes, nroOrdenSiguiente,
                            detalles, cantidadDetalles,
                            clientes, contadorClientes,
                            vehiculos, contadorVehiculos,
                            repuestos, cantidadRepuestos);
                break;
            case 2:
                modificarOrden(ordenes, *cantidadOrdenes,
                               detalles, cantidadDetalles,
                               repuestos, cantidadRepuestos);
                break;
            case 3:
                buscarOrden(ordenes, *cantidadOrdenes, detalles, *cantidadDetalles);
                break;
            case 4:
                cerrarOrden(ordenes, *cantidadOrdenes, detalles, *cantidadDetalles);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionOrdenes != 0);
}

/* =====================================================================
 * MÓDULO 4: REPUESTOS Y STOCK
 * ===================================================================== */

void cargarRepuesto(Repuesto repuestos[], int *cantidadRepuestos)
{
    char codNuevo[LARGO_CODIGO];
    int posRep;
    float precioNuevo;
    int stockNuevo;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                    CARGAR REPUESTO AL CATÁLOGO                   \n");
    printf("==================================================================\n");

    if (*cantidadRepuestos >= MAX_REPUESTOS)
    {
        printf("  [!] Capacidad máxima de repuestos alcanzada (límite %d).\n", MAX_REPUESTOS);
        pausa();
        return;
    }

    printf(">> Ingrese los datos del repuesto:\n\n");
    leerCadena(codNuevo, LARGO_CODIGO, "Código de artículo: ");

    posRep = buscarRepuestoPorCodigo(repuestos, *cantidadRepuestos, codNuevo);
    if (posRep != -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: Ya existe un repuesto con el código %s\n", codNuevo);
        printf("==================================================================\n");
        pausa();
        return;
    }

    strcpy(repuestos[*cantidadRepuestos].codigo, codNuevo);
    leerCadena(repuestos[*cantidadRepuestos].nombre, LARGO_DESCRIPCION, "Descripción / Nombre del repuesto: ");

    do
    {
        leerFlotante("Precio unitario: $", &precioNuevo);
        if (precioNuevo <= 0.0f)
            printf("  [!] El precio debe ser mayor a cero.\n");
    } while (precioNuevo <= 0.0f);
    repuestos[*cantidadRepuestos].precio = precioNuevo;

    do
    {
        stockNuevo = leerEntero("Stock inicial disponible: ");
        if (stockNuevo < 0)
            printf("  [!] El stock no puede ser negativo.\n");
    } while (stockNuevo < 0);
    repuestos[*cantidadRepuestos].stock = stockNuevo;

    (*cantidadRepuestos)++;
    printf("------------------------------------------------------------------\n");
    printf("[OK] Repuesto cargado exitosamente en el catálogo.\n");
    printf("==================================================================\n");
    pausa();
}

void eliminarRepuesto(Repuesto repuestos[], int *cantidadRepuestos)
{
    char codBuscar[LARGO_CODIGO];
    int posRep, j;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                   ELIMINAR REPUESTO DEL CATÁLOGO                 \n");
    printf("==================================================================\n");
    leerCadena(codBuscar, LARGO_CODIGO, "Seleccionar código para eliminar: ");
    posRep = buscarRepuestoPorCodigo(repuestos, *cantidadRepuestos, codBuscar);

    if (posRep != -1)
    {
        for (j = posRep; j < *cantidadRepuestos - 1; j++)
        {
            repuestos[j] = repuestos[j + 1];
        }
        (*cantidadRepuestos)--;
        printf("------------------------------------------------------------------\n");
        printf("[OK] Repuesto eliminado con éxito del inventario.\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: El código especificado no existe.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void buscarRepuesto(const Repuesto repuestos[], int cantidadRepuestos)
{
    char criterio[LARGO_DESCRIPCION];
    int posRep;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                   BUSCAR REPUESTO EN CATÁLOGO                    \n");
    printf("==================================================================\n");
    leerCadena(criterio, LARGO_DESCRIPCION, "Ingrese código o nombre del repuesto: ");

    posRep = buscarRepuestoPorCodigo(repuestos, cantidadRepuestos, criterio);
    if (posRep == -1)
        posRep = buscarRepuestoPorNombre(repuestos, cantidadRepuestos, criterio);

    if (posRep != -1)
    {
        printf("+----------------------------------------------------------------+\n");
        printf("|                       FICHA DE REPUESTO                        |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| CÓDIGO:      %s\n", repuestos[posRep].codigo);
        printf("| DESCRIPCIÓN: %s\n", repuestos[posRep].nombre);
        printf("| PRECIO UNIT: $%.2f\n", repuestos[posRep].precio);
        printf("| STOCK REAL:  %d unidades\n", repuestos[posRep].stock);
        printf("+----------------------------------------------------------------+\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Repuesto no encontrado en el catálogo.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void modificarRepuesto(Repuesto repuestos[], int cantidadRepuestos)
{
    char codBuscar[LARGO_CODIGO];
    int posRep, opcionMod, stockNuevo;
    float precioNuevo;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("               MODIFICAR REPUESTO (PRECIO / STOCK)                \n");
    printf("==================================================================\n");
    leerCadena(codBuscar, LARGO_CODIGO, "Ingrese código del repuesto a modificar: ");
    posRep = buscarRepuestoPorCodigo(repuestos, cantidadRepuestos, codBuscar);

    if (posRep != -1)
    {
        printf(">> Repuesto: %s\n", repuestos[posRep].nombre);
        printf(">> Precio actual: $%.2f | Stock actual: %d\n", repuestos[posRep].precio, repuestos[posRep].stock);
        printf("------------------------------------------------------------------\n");

        do
        {
            printf("Seleccione el dato a modificar:\n");
            printf("  [1] Precio unitario\n");
            printf("  [2] Stock disponible\n");
            opcionMod = leerEntero(">> Opción: ");
        } while (opcionMod != 1 && opcionMod != 2);

        if (opcionMod == 1)
        {
            do
            {
                leerFlotante("Ingrese el precio nuevo: $", &precioNuevo);
                if (precioNuevo <= 0.0f)
                    printf("  [!] El precio debe ser mayor a cero.\n");
            } while (precioNuevo <= 0.0f);
            repuestos[posRep].precio = precioNuevo;
            printf("------------------------------------------------------------------\n");
            printf("[OK] Precio actualizado exitosamente.\n");
        }
        else
        {
            do
            {
                stockNuevo = leerEntero("Ingrese el nuevo stock disponible: ");
                if (stockNuevo < 0)
                    printf("  [!] El stock no puede ser negativo.\n");
            } while (stockNuevo < 0);
            repuestos[posRep].stock = stockNuevo;
            printf("------------------------------------------------------------------\n");
            printf("[OK] Stock actualizado exitosamente.\n");
        }
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: Repuesto no encontrado.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void listarRepuestos(const Repuesto repuestos[], int cantidadRepuestos)
{
    int i;
    limpiaPantalla();
    printf("==================================================================\n");
    printf("                  CATÁLOGO GENERAL DE REPUESTOS                   \n");
    printf("==================================================================\n");

    if (cantidadRepuestos == 0)
    {
        printf("  [!] No hay repuestos cargados en el inventario.\n");
        printf("==================================================================\n");
    }
    else
    {
        printf("%-4s| %-9s| %-31s| %-10s| %-6s\n", "Nro", "CÓDIGO", "DESCRIPCIÓN", "PRECIO", "STOCK");
        printf("----+----------+--------------------------------+-----------+-------\n");
        for (i = 0; i < cantidadRepuestos; i++)
        {
            printf("%-4d| %-9s| %-31s| $%-9.2f| %-6d\n",
                   i + 1, repuestos[i].codigo, repuestos[i].nombre,
                   repuestos[i].precio, repuestos[i].stock);
        }
        printf("==================================================================\n");
        printf("Total de artículos en stock: %d\n", cantidadRepuestos);
        printf("==================================================================\n");
    }
    pausa();
}

void menuRepuestos(Repuesto repuestos[], int *cantidadRepuestos)
{
    int opcionRepuestos;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                   CONTROL DE REPUESTOS Y STOCK                   \n");
        printf("==================================================================\n");
        printf("  [1] Cargar repuesto nuevo\n");
        printf("  [2] Eliminar repuesto\n");
        printf("  [3] Buscar repuesto en inventario\n");
        printf("  [4] Modificar precio o stock de repuesto\n");
        printf("  [5] Listado general de catálogo\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionRepuestos = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionRepuestos)
        {
            case 1:
                cargarRepuesto(repuestos, cantidadRepuestos);
                break;
            case 2:
                eliminarRepuesto(repuestos, cantidadRepuestos);
                break;
            case 3:
                buscarRepuesto(repuestos, *cantidadRepuestos);
                break;
            case 4:
                modificarRepuesto(repuestos, *cantidadRepuestos);
                break;
            case 5:
                listarRepuestos(repuestos, *cantidadRepuestos);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionRepuestos != 0);
}

/* =====================================================================
 * MÓDULO 5: FACTURACIÓN
 * ===================================================================== */

void emitirFactura(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                   const DetalleOrden detalles[], int cantidadDetalles,
                   Factura facturas[], int *cantidadFacturas, int *nroFacturaSiguiente,
                   const Cliente clientes[], int contadorClientes)
{
    int ordenFacturar, posOrden, posCliente, k;
    float totalRepuestos = 0.0f, horasIngresadas = 0.0f, valorHora = 0.0f;
    float manoObra = 0.0f, subtotal = 0.0f, iva = 0.0f, total = 0.0f;
    char fechaFactura[LARGO_FECHA];
    char confirmaFactura[10];

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                   EMISIÓN DE NUEVA FACTURA                       \n");
    printf("==================================================================\n");

    if (*cantidadFacturas >= MAX_FACTURAS)
    {
        printf("  [!] Cantidad máxima de facturas alcanzada (límite %d).\n", MAX_FACTURAS);
        printf("==================================================================\n");
        pausa();
        return;
    }

    ordenFacturar = leerEntero(">> Número de orden a facturar: ");
    posOrden = buscarOrdenPorNumero(ordenes, cantidadOrdenes, ordenFacturar);

    if (posOrden == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: La orden ingresada no existe.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (!sonIguales(ordenes[posOrden].estado, "Finalizada"))
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] La orden todavía está pendiente.\n");
        printf("  Debe cerrarse en el Módulo 3 antes de poder emitir la factura.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (buscarFacturaPorNroOrden(facturas, *cantidadFacturas, ordenFacturar) != -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] La orden Nro %d ya fue facturada con anterioridad.\n", ordenFacturar);
        printf("==================================================================\n");
        pausa();
        return;
    }

    posCliente = buscarClientePorDni(clientes, contadorClientes, ordenes[posOrden].dniCliente);
    printf("------------------------------------------------------------------\n");
    printf(">> Orden Nro:  %d\n", ordenes[posOrden].numero);
    if (posCliente != -1)
        printf(">> Titular:    %s %s\n", clientes[posCliente].apellido, clientes[posCliente].nombre);
    printf(">> DNI:        %d\n", ordenes[posOrden].dniCliente);
    printf(">> Patente:    %s\n", ordenes[posOrden].patente);
    printf(">> Trabajo:    %s\n", ordenes[posOrden].trabajo);
    printf("------------------------------------------------------------------\n");
    printf("Repuestos utilizados:\n");

    for (k = 0; k < cantidadDetalles; k++)
    {
        if (detalles[k].nroOrden == ordenes[posOrden].numero)
        {
            float sub = detalles[k].precioUnitario * detalles[k].cantidad;
            printf("  - %s x %d = $%.2f\n", detalles[k].codRepuesto, detalles[k].cantidad, sub);
            totalRepuestos += sub;
        }
    }

    if (totalRepuestos == 0.0f)
        printf("  (sin repuestos utilizados)\n");
    printf("------------------------------------------------------------------\n");

    do
    {
        leerFlotante("Horas de mano de obra trabajadas: ", &horasIngresadas);
        if (horasIngresadas <= 0.0f)
            printf("  [!] Las horas deben ser mayores a cero.\n");
    } while (horasIngresadas <= 0.0f);

    do
    {
        leerFlotante("Valor por hora de mano de obra: $", &valorHora);
        if (valorHora <= 0.0f)
            printf("  [!] El valor debe ser mayor a cero.\n");
    } while (valorHora <= 0.0f);

    leerCadena(fechaFactura, LARGO_FECHA, "Fecha de emisión (DD/MM/AAAA): ");
    if (strlen(fechaFactura) == 0)
        strcpy(fechaFactura, "Hoy");

    manoObra = horasIngresadas * valorHora;
    subtotal = totalRepuestos + manoObra;
    iva = (subtotal * PORCENTAJE_IVA) / 100.0f;
    total = subtotal + iva;

    printf("+================================================================+\n");
    printf("|                   PRELIQUIDACIÓN DE FACTURA                    |\n");
    printf("+================================================================+\n");
    printf("| Subtotal Repuestos:           $%.2f\n", totalRepuestos);
    printf("| Mano de Obra (%.1f hs x $%.2f):  $%.2f\n", horasIngresadas, valorHora, manoObra);
    printf("| Subtotal Gravado:             $%.2f\n", subtotal);
    printf("| IVA (%.0f%%):                    $%.2f\n", PORCENTAJE_IVA, iva);
    printf("+----------------------------------------------------------------+\n");
    printf("| TOTAL A PAGAR:                $%.2f\n", total);
    printf("+================================================================+\n");

    leerCadena(confirmaFactura, sizeof(confirmaFactura), "Confirmar emisión definitiva? (S/N): ");
    if (confirmaFactura[0] == 'S' || confirmaFactura[0] == 's')
    {
        int idxFac = *cantidadFacturas;
        facturas[idxFac].numero = *nroFacturaSiguiente;
        (*nroFacturaSiguiente)++;

        facturas[idxFac].nroOrden = ordenes[posOrden].numero;
        facturas[idxFac].dni = ordenes[posOrden].dniCliente;
        strcpy(facturas[idxFac].patente, ordenes[posOrden].patente);
        strcpy(facturas[idxFac].fecha, fechaFactura);

        facturas[idxFac].montos.repuestos = totalRepuestos;
        facturas[idxFac].montos.horas = horasIngresadas;
        facturas[idxFac].montos.valorHora = valorHora;
        facturas[idxFac].montos.manoObra = manoObra;
        facturas[idxFac].montos.subtotal = subtotal;
        facturas[idxFac].montos.iva = iva;
        facturas[idxFac].montos.total = total;

        (*cantidadFacturas)++;
        printf("------------------------------------------------------------------\n");
        printf("[OK] Factura Nro %d emitida correctamente.\n", facturas[idxFac].numero);
        printf("==================================================================\n");
    }
    else
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Emisión de factura cancelada.\n");
        printf("==================================================================\n");
    }
    pausa();
}

void verComprobanteFactura(const Factura facturas[], int cantidadFacturas,
                           const DetalleOrden detalles[], int cantidadDetalles,
                           const Cliente clientes[], int contadorClientes)
{
    int facturaBuscar, posFactura, posCliente, k;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                CONSULTAR COMPROBANTE DE FACTURA                  \n");
    printf("==================================================================\n");
    facturaBuscar = leerEntero(">> Ingrese número de factura: ");
    posFactura = buscarFacturaPorNumero(facturas, cantidadFacturas, facturaBuscar);

    if (posFactura == -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: La factura Nro %d no existe.\n", facturaBuscar);
        printf("==================================================================\n");
    }
    else
    {
        posCliente = buscarClientePorDni(clientes, contadorClientes, facturas[posFactura].dni);
        limpiaPantalla();
        printf("+================================================================+\n");
        printf("|                  COMPROBANTE DE FACTURACIÓN                    |\n");
        printf("+================================================================+\n");
        printf("| FACTURA Nro: %-18d FECHA: %-19s\n", facturas[posFactura].numero, facturas[posFactura].fecha);
        printf("| ORDEN ASOCIADA: Nro %-12d PATENTE: %-17s\n",
               facturas[posFactura].nroOrden, facturas[posFactura].patente);

        if (posCliente != -1)
        {
            printf("| TITULAR: %s %s (DNI: %d)\n",
                   clientes[posCliente].apellido, clientes[posCliente].nombre, facturas[posFactura].dni);
        }
        else
        {
            printf("| TITULAR DNI: %d\n", facturas[posFactura].dni);
        }

        printf("+----------------------------------------------------------------+\n");
        printf("| DETALLE DE REPUESTOS Y MATERIALES:                             |\n");
        for (k = 0; k < cantidadDetalles; k++)
        {
            if (detalles[k].nroOrden == facturas[posFactura].nroOrden)
            {
                float sub = detalles[k].precioUnitario * detalles[k].cantidad;
                printf("|   - %s x %d = $%.2f\n", detalles[k].codRepuesto, detalles[k].cantidad, sub);
            }
        }
        printf("+----------------------------------------------------------------+\n");
        printf("| LIQUIDACIÓN DE COSTOS:                                         |\n");
        printf("|   Repuestos:                    $%.2f\n", facturas[posFactura].montos.repuestos);
        printf("|   Mano de Obra (%.1f hs x $%.2f): $%.2f\n",
               facturas[posFactura].montos.horas, facturas[posFactura].montos.valorHora,
               facturas[posFactura].montos.manoObra);
        printf("|   Subtotal (sin impuesto):      $%.2f\n", facturas[posFactura].montos.subtotal);
        printf("|   IVA (%.0f%%):                    $%.2f\n", PORCENTAJE_IVA, facturas[posFactura].montos.iva);
        printf("+----------------------------------------------------------------+\n");
        printf("| TOTAL DE FACTURA:               $%.2f\n", facturas[posFactura].montos.total);
        printf("+================================================================+\n");
    }
    pausa();
}

void consultarFacturas(const Factura facturas[], int cantidadFacturas)
{
    int modoConsulta, dniBuscar = 0, i, facturasEncontradas = 0;
    float totalFacturado = 0.0f;

    limpiaPantalla();
    printf("==================================================================================\n");
    printf("                             CONSULTA DE FACTURAS                                 \n");
    printf("==================================================================================\n");

    if (cantidadFacturas == 0)
    {
        printf("  [!] No hay facturas emitidas en el sistema.\n");
        printf("==================================================================================\n");
        pausa();
        return;
    }

    do
    {
        printf("Seleccione el tipo de consulta:\n");
        printf("  [1] Listar todas las facturas emitidas\n");
        printf("  [2] Filtrar por DNI de cliente\n");
        modoConsulta = leerEntero(">> Opción: ");
    } while (modoConsulta != 1 && modoConsulta != 2);

    if (modoConsulta == 2)
        dniBuscar = leerDni(">> Ingrese DNI del cliente a consultar: ");

    limpiaPantalla();
    printf("==================================================================================\n");
    if (modoConsulta == 1)
        printf("                            LISTADO DE FACTURAS EMITIDAS                          \n");
    else
        printf("                     FACTURAS EMITIDAS PARA EL DNI: %d\n", dniBuscar);
    printf("==================================================================================\n");
    printf("%-4s| %-6s| %-11s| %-9s| %-16s\n", "NRO", "ORDEN", "FECHA", "PATENTE", "TOTAL FACTURADO");
    printf("----+-------+------------+----------+---------------------------------------------\n");

    for (i = 0; i < cantidadFacturas; i++)
    {
        if (modoConsulta == 1 || facturas[i].dni == dniBuscar)
        {
            printf("%-4d| %-6d| %-11s| %-9s| $%-16.2f\n",
                   facturas[i].numero, facturas[i].nroOrden, facturas[i].fecha,
                   facturas[i].patente, facturas[i].montos.total);
            facturasEncontradas++;
            totalFacturado += facturas[i].montos.total;
        }
    }

    if (facturasEncontradas == 0)
    {
        printf("  [!] No se encontraron facturas para el criterio seleccionado.\n");
    }
    else
    {
        printf("==================================================================================\n");
        printf("Facturas listadas: %d | Monto total: $%.2f\n", facturasEncontradas, totalFacturado);
    }
    printf("==================================================================================\n");
    pausa();
}

void menuFacturacion(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                     const DetalleOrden detalles[], int cantidadDetalles,
                     Factura facturas[], int *cantidadFacturas, int *nroFacturaSiguiente,
                     const Cliente clientes[], int contadorClientes)
{
    int opcionFacturacion;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                      MÓDULO DE FACTURACIÓN                       \n");
        printf("==================================================================\n");
        printf("  [1] Emitir factura de orden finalizada\n");
        printf("  [2] Ver comprobante de factura\n");
        printf("  [3] Consultar facturas (todas o por DNI)\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionFacturacion = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionFacturacion)
        {
            case 1:
                emitirFactura(ordenes, cantidadOrdenes,
                              detalles, cantidadDetalles,
                              facturas, cantidadFacturas, nroFacturaSiguiente,
                              clientes, contadorClientes);
                break;
            case 2:
                verComprobanteFactura(facturas, *cantidadFacturas, detalles, cantidadDetalles, clientes, contadorClientes);
                break;
            case 3:
                consultarFacturas(facturas, *cantidadFacturas);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionFacturacion != 0);
}

/* =====================================================================
 * MÓDULO 6: HISTORIAL DE REPARACIONES
 * ===================================================================== */

void consultarHistorialDetallado(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                                 const DetalleOrden detalles[], int cantidadDetalles,
                                 const Factura facturas[], int cantidadFacturas,
                                 const Cliente clientes[], int contadorClientes)
{
    int criterioHistorial, dniBuscar = 0, i, k, posFactura, posCliente = -1, ordenesEncontradas = 0;
    char patBuscar[LARGO_PATENTE];

    limpiaPantalla();
    printf("==================================================================\n");
    printf("               HISTORIAL DETALLADO DE REPARACIONES                \n");
    printf("==================================================================\n");

    do
    {
        printf("Buscar reparaciones finalizadas por:\n");
        printf("  [1] Patente del vehículo\n");
        printf("  [2] DNI del cliente\n");
        criterioHistorial = leerEntero(">> Opción: ");
    } while (criterioHistorial != 1 && criterioHistorial != 2);

    if (criterioHistorial == 1)
    {
        leerCadena(patBuscar, LARGO_PATENTE, ">> Ingrese patente a consultar: ");
    }
    else
    {
        dniBuscar = leerDni(">> Ingrese DNI del cliente a consultar: ");
        posCliente = buscarClientePorDni(clientes, contadorClientes, dniBuscar);
    }

    limpiaPantalla();
    printf("==================================================================\n");
    if (criterioHistorial == 1)
    {
        printf("       HISTORIAL DE REPARACIONES - PATENTE: %s\n", patBuscar);
    }
    else
    {
        if (posCliente != -1)
            printf(" HISTORIAL: %s %s (DNI: %d)\n", clientes[posCliente].apellido, clientes[posCliente].nombre, dniBuscar);
        else
            printf(" HISTORIAL: DNI %d\n", dniBuscar);
    }
    printf("==================================================================\n");

    for (i = 0; i < cantidadOrdenes; i++)
    {
        if (sonIguales(ordenes[i].estado, "Finalizada") &&
            ((criterioHistorial == 1 && sonIguales(ordenes[i].patente, patBuscar)) ||
             (criterioHistorial == 2 && ordenes[i].dniCliente == dniBuscar)))
        {
            ordenesEncontradas++;
            printf("+----------------------------------------------------------------+\n");
            printf("| ORDEN NRO:      %-16d | ESTADO: FINALIZADA\n", ordenes[i].numero);
            printf("| PATENTE:        %-16s | DNI DUEÑO: %d\n", ordenes[i].patente, ordenes[i].dniCliente);
            printf("| FECHA INGRESO:  %-16s | FECHA ENTREGA: %s\n", ordenes[i].fechaInicio, ordenes[i].fechaEntrega);
            printf("| MOTIVO INGRESO: %s\n", ordenes[i].descripcion);
            printf("| DIAGNÓSTICO:    %s\n", ordenes[i].diagnostico);
            printf("| TRABAJO HECHO:  %s\n", ordenes[i].trabajo);
            printf("| Repuestos utilizados:\n");

            for (k = 0; k < cantidadDetalles; k++)
            {
                if (detalles[k].nroOrden == ordenes[i].numero)
                {
                    float sub = detalles[k].precioUnitario * detalles[k].cantidad;
                    printf("|   * %s (x%d) - $%.2f\n", detalles[k].codRepuesto, detalles[k].cantidad, sub);
                }
            }

            posFactura = buscarFacturaPorNroOrden(facturas, cantidadFacturas, ordenes[i].numero);
            if (posFactura != -1)
                printf("| FACTURA ASOCIADA: Factura Nro %d | Total: $%.2f\n",
                       facturas[posFactura].numero, facturas[posFactura].montos.total);
            else
                printf("| FACTURA ASOCIADA: Pendiente de cobro\n");

            printf("+----------------------------------------------------------------+\n");
        }
    }

    if (ordenesEncontradas == 0)
    {
        printf("  [!] No se registran reparaciones finalizadas para el criterio buscado.\n");
    }
    else
    {
        printf("Total de reparaciones finalizadas encontradas: %d\n", ordenesEncontradas);
    }
    printf("==================================================================\n");
    pausa();
}

void listarHistorialGeneral(const OrdenTrabajo ordenes[], int cantidadOrdenes, const Factura facturas[], int cantidadFacturas)
{
    int i, posFactura, totalFinalizadas = 0;

    limpiaPantalla();
    printf("==================================================================================\n");
    printf("                   LISTADO GENERAL DE REPARACIONES FINALIZADAS                    \n");
    printf("==================================================================================\n");
    printf("%-6s| %-9s| %-10s| %-11s| %-11s| %-16s\n", "ORDEN", "PATENTE", "DNI DUEÑO", "INGRESO", "ENTREGA", "TOTAL FACTURADO");
    printf("------+----------+-----------+------------+------------+--------------------------\n");

    for (i = 0; i < cantidadOrdenes; i++)
    {
        if (sonIguales(ordenes[i].estado, "Finalizada"))
        {
            totalFinalizadas++;
            posFactura = buscarFacturaPorNroOrden(facturas, cantidadFacturas, ordenes[i].numero);
            if (posFactura != -1)
            {
                printf("%-6d| %-9s| %-10d| %-11s| %-11s| $%-16.2f\n",
                       ordenes[i].numero, ordenes[i].patente, ordenes[i].dniCliente,
                       ordenes[i].fechaInicio, ordenes[i].fechaEntrega,
                       facturas[posFactura].montos.total);
            }
            else
            {
                printf("%-6d| %-9s| %-10d| %-11s| %-11s| (Pendiente)\n",
                       ordenes[i].numero, ordenes[i].patente, ordenes[i].dniCliente,
                       ordenes[i].fechaInicio, ordenes[i].fechaEntrega);
            }
        }
    }

    if (totalFinalizadas == 0)
    {
        printf("  [!] No hay reparaciones finalizadas registradas.\n");
    }
    else
    {
        printf("==================================================================================\n");
        printf("Total de reparaciones finalizadas en el taller: %d\n", totalFinalizadas);
    }
    printf("==================================================================================\n");
    pausa();
}

void menuHistorial(const OrdenTrabajo ordenes[], int cantidadOrdenes,
                   const DetalleOrden detalles[], int cantidadDetalles,
                   const Factura facturas[], int cantidadFacturas,
                   const Cliente clientes[], int contadorClientes)
{
    int opcionHistorial;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                    HISTORIAL DE REPARACIONES                     \n");
        printf("==================================================================\n");
        printf("  [1] Consultar historial detallado (por patente o DNI)\n");
        printf("  [2] Listado general de reparaciones finalizadas\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionHistorial = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionHistorial)
        {
            case 1:
                consultarHistorialDetallado(ordenes, cantidadOrdenes,
                                           detalles, cantidadDetalles,
                                           facturas, cantidadFacturas,
                                           clientes, contadorClientes);
                break;
            case 2:
                listarHistorialGeneral(ordenes, cantidadOrdenes, facturas, cantidadFacturas);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no válida.\n");
                pausa();
                break;
        }
    } while (opcionHistorial != 0);
}

/* =====================================================================
 * MÓDULO 7: USUARIOS Y SESIONES
 * ===================================================================== */

void listarUsuarios(const Usuario usuarios[], int cantidadUsuarios)
{
    int i;
    limpiaPantalla();
    printf("==================================================================\n");
    printf("                      USUARIOS REGISTRADOS                        \n");
    printf("==================================================================\n");
    printf("%-4s| %-22s| %-20s\n", "Nro", "NOMBRE DE USUARIO", "ROL ASIGNADO");
    printf("----+-----------------------+-------------------------------------\n");

    for (i = 0; i < cantidadUsuarios; i++)
    {
        printf("%-4d| %-22s| %-20s\n", i + 1, usuarios[i].nombre, usuarios[i].rol);
    }

    printf("==================================================================\n");
    printf("Total de cuentas activas: %d\n", cantidadUsuarios);
    printf("==================================================================\n");
    pausa();
}

void registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, const char *rolActivo)
{
    char nuevoUser[LARGO_TEXTO];
    int opcRol, posUser;

    limpiaPantalla();
    printf("==================================================================\n");
    printf("                    REGISTRAR NUEVO USUARIO                       \n");
    printf("==================================================================\n");

    if (!sonIguales(rolActivo, "administrador"))
    {
        printf("  [!] Acceso denegado: solo un administrador puede registrar usuarios.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    if (*cantidadUsuarios >= MAX_USUARIOS)
    {
        printf("  [!] Límite de usuarios alcanzado (máximo %d cuentas).\n", MAX_USUARIOS);
        printf("==================================================================\n");
        pausa();
        return;
    }

    leerCadena(nuevoUser, LARGO_TEXTO, ">> Nombre de usuario nuevo: ");
    posUser = buscarUsuarioPorNombre(usuarios, *cantidadUsuarios, nuevoUser);

    if (posUser != -1)
    {
        printf("------------------------------------------------------------------\n");
        printf("  [!] Error: El nombre de usuario ya existe en el sistema.\n");
        printf("==================================================================\n");
        pausa();
        return;
    }

    strcpy(usuarios[*cantidadUsuarios].nombre, nuevoUser);
    leerCadena(usuarios[*cantidadUsuarios].clave, LARGO_TEXTO, ">> Contraseña de acceso: ");

    do
    {
        printf("------------------------------------------------------------------\n");
        printf("Seleccione rol para el nuevo usuario:\n");
        printf("  [1] Administrador (Acceso total al sistema)\n");
        printf("  [2] Mecanico (Ordenes, Repuestos, Historial)\n");
        printf("  [3] Cliente (Historial de Reparaciones)\n");
        opcRol = leerEntero(">> Opción: ");
    } while (opcRol < 1 || opcRol > 3);

    switch (opcRol)
    {
        case 1:
            strcpy(usuarios[*cantidadUsuarios].rol, "administrador");
            break;
        case 2:
            strcpy(usuarios[*cantidadUsuarios].rol, "mecanico");
            break;
        case 3:
            strcpy(usuarios[*cantidadUsuarios].rol, "cliente");
            break;
    }

    (*cantidadUsuarios)++;
    printf("------------------------------------------------------------------\n");
    printf("[OK] Usuario registrado exitosamente en el sistema.\n");
    printf("==================================================================\n");
    pausa();
}

void menuUsuarios(Usuario usuarios[], int *cantidadUsuarios, const char *rolActivo)
{
    int opcionUsuarios;
    do
    {
        limpiaPantalla();
        printf("==================================================================\n");
        printf("                    GESTIÓN DE USUARIOS Y ROLES                   \n");
        printf("==================================================================\n");
        printf("  [1] Ver usuarios registrados\n");
        printf("  [2] Registrar nuevo usuario\n");
        printf("  ----------------------------------------------------------------\n");
        printf("  [0] Volver al menu principal\n");
        printf("==================================================================\n");
        opcionUsuarios = leerEntero(">> Ingrese opción deseada: ");

        switch (opcionUsuarios)
        {
            case 1:
                listarUsuarios(usuarios, *cantidadUsuarios);
                break;
            case 2:
                registrarUsuario(usuarios, cantidadUsuarios, rolActivo);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opción no valida.\n");
                pausa();
                break;
        }
    } while (opcionUsuarios != 0);
}
