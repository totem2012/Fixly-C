# Fixly - Sistema Integral de Gestión de Taller Mecánico

**Materia:** Programación I  
**Institución:** Instituto Nacional Superior del Profesorado Técnico (INSPT - UTN)  
**Comisión:** 1.605 — 2026  
**Cátedra:** Prof. Ing. Alejandro Jorge Behringer / Ay. Prof. Juan Carlos Capia  

### Integrantes del Proyecto:
- Alexander Ezequiel Albani
- Nahuel Agustín Gatica
- Marcos Javier Baez Aguilera
- Patricia Alejandra Martínez

---

## 1. Descripción del Proyecto

**Fixly** es un sistema integral por consola diseñado para digitalizar y centralizar la operación diaria de un taller mecánico. El proyecto surge como una implementación unificada para las materias **Laboratorio** (desarrollado en pseudocódigo PSeInt) y **Programación I** (desarrollado en lenguaje C).

El sistema reemplaza las planillas sueltas en papel por registros estructurados, permitiendo trazabilidad completa desde el ingreso del vehículo hasta la liquidación final de la factura.

---

## 2. Estructura y Filosofía en Lenguaje C

Siguiendo estrictamente las pautas de la cátedra de Programación I:
- **No se sobrecarga el código con complejidades innecesarias**: se utiliza programación estructurada pura, arreglos estáticos dimensionados mediante directivas `#define`, y paso de parámetros por valor y por referencia (punteros).
- **Biblioteca `utils.h`**: Contiene las funciones utilitarias provistas por la cátedra (`limpiaPantalla()`, `pausa()`, `utf8()`, `linea()`, `leerEntero()`, `leerFlotante()`, `leerCadena()`, `sonIguales()`, etc.).
  - **Estructura solicitada por la cátedra:** Se anuncian los prototipos de las funciones al inicio del archivo `utils.h` y su desarrollo real se encuentra implementado debajo dentro del mismo archivo.
  - Se incluye también `utiles.h` como interfaz compatible.
- **Modelado con Estructuras (`struct`)**:
  - Se modelan las entidades principales (`Cliente`, `Vehiculo`, `Repuesto`, `DetalleOrden`, `OrdenTrabajo`, `Factura`, `Usuario`).
  - **Estructura anidada obligatoria:** La entidad `Factura` incorpora una estructura anidada `Liquidacion` para encapsular los costos detallados (mano de obra, repuestos, subtotal, IVA y total general).

---

## 3. Módulos del Sistema

1. **Módulo 1: Clientes**
   - Alta de clientes con validación estricta de DNI único.
   - Listado formateado con cálculo de totales.
   - Modificación de campos individuales (Nombre, Apellido, Celular, DNI).
   - Baja física con reordenamiento de vector.

2. **Módulo 2: Vehículos**
   - Alta de vehículos (Patente única, Marca, Modelo, DNI del dueño).
   - Integración automática: si el DNI del dueño no existe, permite registrarlo en el acto.
   - Listado vinculando los datos del vehículo con la ficha del titular.
   - Modificación y baja de unidades.

3. **Módulo 3: Órdenes de Trabajo**
   - Control de capacidad del taller (máximo 20 vehículos en reparación simultánea).
   - Validación cruzada: verificación de existencia del cliente, existencia de la patente, y coincidencia de titularidad.
   - Control de exclusividad: un vehículo no puede tener más de una orden en estado "Pendiente".
   - Asignación de repuestos con control y descuento automático de stock en tiempo real.
   - Modificación de motivo, diagnóstico, agregación y retiro de repuestos (restituyendo stock al catálogo).
   - Búsqueda por patente o número de orden.
   - Cierre de orden con registro de trabajo mecánico realizado y fecha de entrega.

4. **Módulo 4: Repuestos y Stock**
   - Alta en catálogo con código alfanumérico único.
   - Modificación selectiva de precio o stock.
   - Búsqueda por código o descripción.
   - Catálogo general formateado en columnas.

5. **Módulo 5: Facturación**
   - Emisión de facturas únicamente para órdenes en estado "Finalizada" que no hayan sido facturadas previamente.
   - Liquidación automática: suma de repuestos utilizados + cálculo de mano de obra (horas × valor/hora) + IVA (21%).
   - Impresión de comprobante oficial detallado con desglose de ítems e impuestos.
   - Consulta de facturación general o filtrada por DNI de cliente, con acumulación de montos totales.

6. **Módulo 6: Historial de Reparaciones**
   - Consulta detallada por patente o por DNI con ficha histórica de reparaciones finalizadas, repuestos y estado de cobro.
   - Listado general de todas las reparaciones concluidas por el taller.

7. **Módulo 7: Usuarios y Seguridad**
   - Sistema de inicio de sesión con 3 roles diferenciados:
     - **Administrador:** Acceso irrestricto a los 7 módulos del sistema.
     - **Mecánico:** Acceso operativo (Órdenes de Trabajo, Repuestos y Stock, Historial).
     - **Cliente:** Acceso exclusivo de consulta (Historial de Reparaciones).
   - Registro de nuevas cuentas restringido exclusivamente al rol Administrador.

---

## 4. Usuarios y Credenciales Precargadas para Pruebas

Al iniciar, el sistema precarga datos demostrativos (5 clientes, 5 vehículos, 5 repuestos, 2 órdenes, 1 factura y 3 usuarios):

| Usuario | Contraseña | Rol | Acceso |
|---|---|---|---|
| `admin` | `admin123` | `administrador` | Acceso total (Módulos 1 al 7) |
| `mecanico` | `meca123` | `mecanico` | Módulos 3, 4 y 6 |
| `cliente` | `cliente123` | `cliente` | Módulo 6 (Historial) |

---

## 5. Compilación y Ejecución

### Opción 1: Con `make`
```bash
make
./fixly
```

### Opción 2: Con `gcc` directamente
```bash
gcc -Wall -Wextra -std=c99 main.c -o fixly
./fixly
```
