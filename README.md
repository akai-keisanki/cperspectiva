# cperspectiva
![created at](https://img.shields.io/github/created-at/akai-keisanki/cperspectiva)
![license](https://img.shields.io/github/license/akai-keisanki/cperspectiva)
![release](https://img.shields.io/github/v/release/akai-keisanki/cperspectiva)
![top language](https://img.shields.io/github/languages/top/akai-keisanki/cperspectiva)
![forks](https://img.shields.io/github/forks/akai-keisanki/cperspectiva)
![stars](https://img.shields.io/github/stars/akai-keisanki/cperspectiva)

POSIX-oriented TUI abstracter library.

The continued Perspectiva Project (https://github.com/Science52101/Perspectiva) for C and more organised.

## Features

- PCMC -- Perspectiva Character Matrix Container
  - Character matrix container;
  - Buffered/formated and raw drawing/rendering support;
  - Background layer support;

- PCMC Tricks
  - Drawing utils/functionality for PCMC;
  - Character string writing support with vectorial direction;
  - PCMC to PCMC drawing suport;

- TUI Utils
  - TUI-related IO utils (specially for PTUIA);
  - Execute callback with std. IO in TUI mode;

- PTUIA -- Perspectiva TUI Abstracter
  - Stateful TUI application abstracter object with lifecycle callbacks;
  - User-defined application state;
  - Raw keyboard input processing support;
  - ANSI-oriented TUI drawing/rendering;
  - Components (secondary PCMCs) with automatic size support;

- Perspectiva Types
  - Integer coordinates type for point and basic vector description;
  - Basic linear algebra utils for coordinates;

## Examples

Check the [`examples/`](examples/) directory on this repository.

## Usage

### Testing

For test purposes, write a script like one of the examples to a `test.c` file into this project's working directory.

Execute `make clean run_test` to build the library, compile your script and link both. The resulting binary will be created as `test`.

### Linking

First, to build the shared library, run `make clean build/libcpers.so`.
Then, your source must be compiled with the flag for the library includes (e.g. `-Ipath/to/cperspectiva/include`) and linked against the shared library (e.g. `-Lpath/to/cperspectiva/build -lcpers`).
