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

## 🔧 Prerrequisitos

Antes de comenzar, asegúrate de tener instalado:

- [Docker](https://www.docker.com/get-started) (versión 20.10 o superior)
- [Git](https://git-scm.com/downloads) (versión 2.25 o superior)

## 🚀 Instalación

### 1️⃣ Clonar el Repositorio

```bash
git clone https://github.com/lbonsenor/tpe-tla.git
cd tpe-tla
```

### 🐳 2️⃣ Construir la imagen de Docker

Ejecuta el comando correspondiente a tu sistema operativo. **Importante**: Para todos los comandos siguientes, utiliza siempre los scripts de tu sistema operativo correspondiente.

**Linux/MacOS:**
```bash
./script/ubuntu/docker-build.sh
```

**Windows:**
```bash
./script/windows/docker-build.sh
```

### ⚙️ 3️⃣ Instalar dependencias

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

## 📖 Uso

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
| `--isInput` | Genera solo el contenido sin preámbulo ni epílogo cuando el código LaNgTeX va a ser insertado dentro de un documento LaTeX existente | - | `--isInput` |

### Comando Completo por Sistema Operativo

**Linux/MacOS:**
```bash
./script/ubuntu/start.sh <inputFile> -d <outputDir> -o <mainOutputFile> [--isInput]
```

**Windows:**
```bash
./script/windows/start.sh <inputFile> -d <outputDir> -o <mainOutputFile> [--isInput]
```

## 💡 Ejemplos

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
./script/ubuntu/start.sh src/test/c/accept/fragment --isInput -d fragments -o content
```

### Ejemplo Completo (Recomendado)

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
| **Fragmento insertable** | `./script/ubuntu/start.sh input.langtex --isInput -o fragmento` | Solo contenido, sin preámbulo ni epílogo |
| **Vista rápida** | `./script/ubuntu/start.sh input.langtex` | Muestra resultado directamente en terminal |

### Estructura de Salida Generada

Cuando ejecutas el comando con `-d` y `-o`, se genera la siguiente estructura:

```
<outputDir>/
├── 📄 <mainOutputFile>.tex    # Archivo principal LaTeX
└── 🔗 preamble.tex           # Symlink con macros de LaNgTeX
```

## 🧪 Testing

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


> [!WARNING]
> 
> ## Notas
> 
> Subimos un nuevo documento de especificación (v2.0.0) en la carpeta doc, que refleja los cambios realizados en la sintaxis y diseño del lenguaje.

---

## Flex-Bison-Compiler

[![✗](https://img.shields.io/badge/Release-v1.1.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

[![✗](https://github.com/agustin-golmar/Flex-Bison-Compiler/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/agustin-golmar/Flex-Bison-Compiler/actions/workflows/pipeline.yaml)

A base compiler example, developed with Flex and Bison.

- [Proyecto Especial - `LaNgTeX`](#proyecto-especial---langtex)
  - [Flex-Bison-Compiler](#flex-bison-compiler)
  - [Environment](#environment)
  - [CI/CD](#cicd)
  - [Recommended Extensions](#recommended-extensions)

## Environment

Set the following environment variables to control and configure the behaviour of the application:

|Name|Default|Description|
|-|:-:|-|
|`LOG_IGNORED_LEXEMES`|`true`|When `true`, logs all of the ignored lexemes found with Flex at DEBUGGING level. To remove those logs from the console output set it to `false`.|
|`LOGGING_LEVEL`|`INFORMATION`|The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`.|

## CI/CD

To trigger an automatic integration on every push or PR (_Pull Request_), you must activate _GitHub Actions_ in the _Settings_ tab. Use the following configuration:

|Key|Value|
|-|-|
|Actions permissions|Allow all actions and reusable workflows|
|Artifact and log retention|`30 days`|
|Fork pull request workflows from outside collaborators|Require approval for all outside collaborators|
|Workflow permissions|Read repository contents and packages permissions|
|Allow GitHub Actions to create and approve pull requests|`false`|

After integration is done, change the badges of this `README.md` to point to the new repository.

## Recommended Extensions

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)
