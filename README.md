# Proyecto Especial - `LaNgTeX`

Un compilador que convierte archivos LaNgTeX a LaTeX, facilitando la generación de documentos LaTeX con una sintaxis simplificada.

> [!NOTE]
>
> ## Integrantes
>
> - `Lautaro BONSEÑOR` <lbonsenor@itba.edu.ar> | Legajo `62842`
> - `Camila LEE` | <clee@itba.edu.ar> | Legajo `63382`
> - `Fernando LI` | <feli@itba.edu.ar> | Legajo `64382`
> - `Matias RINALDO` | <mrinaldo@itba.edu.ar> | Legajo `60357`

## Prerrequisitos

Antes de comenzar, asegúrate de tener instalado:

- [Docker](https://www.docker.com/get-started) (versión 20.10 o superior)
- [Git](https://git-scm.com/downloads) (versión 2.25 o superior)

## Instalación

### ① Clonar el Repositorio

```bash
git clone https://github.com/lbonsenor/tpe-tla.git
cd tpe-tla
```

### ② Construir la imagen de Docker

Ejecuta el comando correspondiente a tu sistema operativo. **Importante**: Para todos los comandos siguientes, utiliza siempre los scripts de tu sistema operativo correspondiente.

**Linux/MacOS:**

```bash
./script/ubuntu/docker-build.sh
```

**Windows:**

```bash
./script/windows/docker-build.sh
```

### ③ Instalar dependencias

Ejecuta los siguientes comandos en orden para completar la configuración:

**Instalar dependencias/aplicaciones necesarias:**

```bash
./script/ubuntu/install.sh    # Linux/MacOS
./script/windows/install.sh   # Windows
```

**Compilar el proyecto:**

```bash
./script/ubuntu/build.sh      # Linux/MacOS
./script/windows/build.sh     # Windows
```

## Uso

### Sintaxis del Comando Principal

```bash
./script/<os>/start.sh <inputFile> -d <outputDir> -o <mainOutputFile> [flags]
```

### Parámetros

| Parámetro | Descripción | Requerido |
|-----------|-------------|-----------|
| `inputFile` | Archivo correspondiente escrito en LaNgTeX que será procesado y transformado a LaTeX | ✅ Sí |
| `outputDir` | Directorio en el cual se van a almacenar los archivos correspondientes al proyecto | ❌ No |
| `mainOutputFile` | Nombre del archivo en el cual se va a almacenar el resultado | ❌ No |

### Flags Disponibles

| Flag | Descripción | Valor por defecto | Ejemplo |
|------|-------------|-------------------|---------|
| `-d <directorio>` | Nombre del directorio de salida | `stdout` (se imprime en terminal) | `-d output` |
| `-o <archivo>` | Nombre del archivo principal | `main.tex` | `-o documento` |
| `-i` o `--input` | Genera solo el contenido sin preámbulo ni epílogo cuando el código LaNgTeX va a ser insertado dentro de un documento LaTeX existente | `false` | `-i` |

### Comando Completo por Sistema Operativo

**Linux/MacOS:**

```bash
./script/ubuntu/start.sh <inputFile> -d <outputDir> -o <mainOutputFile> -i
```

**Windows:**

```bash
./script/windows/start.sh <inputFile> -d <outputDir> -o <mainOutputFile> -i
```

## Ejemplos

### Ejemplo Básico (Vista en Terminal)

```bash
# Convertir archivo y mostrar resultado en terminal
./script/ubuntu/start.sh src/test/c/accept/example
```

### Ejemplo con Directorio de Salida

```bash
# Generar archivo en directorio específico
./script/ubuntu/start.sh src/test/c/accept/try -d output
```

### Ejemplo con Contenido Insertable

```bash
# Generar contenido sin preámbulo para insertar en documento existente
./script/ubuntu/start.sh src/test/c/accept/fragment -i -d fragments -o content
```

### Ejemplo Completo

```bash
# Comando de ejemplo completo para correr el proyecto
./script/ubuntu/start.sh src/test/c/accept/try -d out -o output
```

**Este comando genera:**

- 📁 **Directorio `out/`** (se crea si no existe)
- 📄 **Archivo principal**: `output.tex` (o `main.tex` si no se especifica `-o`)
- 🔗 **Symlink**: `preamble.tex` que contiene todas las macros de LaNgTeX
- 📋 **Documento LaTeX completo** con preámbulo y epílogo incluidos

### Casos de Uso

| Situación | Comando Recomendado | Descripción |
|-----------|---------------------|-------------|
| **Documento completo** | `./script/ubuntu/start.sh input.langtex -d output -o documento` | Genera documento LaTeX completo con preamble |
| **Fragmento insertable** | `./script/ubuntu/start.sh input.langtex -i -o fragmento` | Solo contenido, sin preámbulo ni epílogo |
| **Vista rápida** | `./script/ubuntu/start.sh input.langtex` | Muestra resultado directamente en terminal |

### Estructura de Salida Generada

Cuando se ejecuta el comando con `-d` y `-o`, se genera la siguiente estructura:

```
<outputDir>/
├── 📄 <mainOutputFile>.tex    # Archivo principal LaTeX
└── 🔗 preamble.tex           # Symlink con macros de LaNgTeX
```

## Testing

Para ejecutar los archivos de prueba y visualizar los resultados progresivamente en terminal:

**Linux/MacOS:**

```bash
./script/ubuntu/test.sh
```

**Windows:**

```bash
./script/windows/test.sh
```

Los resultados irán apareciendo en la terminal, mostrando el estado de cada caso de prueba conforme se ejecutan.
