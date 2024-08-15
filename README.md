# ACBS - Another C Build System

## Description

An easy to use build-system for C.

### Motivation

Makefile is pretty solid, but like C, it is very free, which means when someone who doesn't know what they are doing touches it, it can become horrendous.

Building C code is pretty simple tho, so making something a bit more restrictive is not a terrible idea.

Yet somehow everyone that has tried has failed horribly. Cmake is arcane magic. Bazel is a complicated mess.

I just want something that can build a project.

## Anatomy of a Project File

To specify a project, you provide some metadata via an `acbs.ini`

### INI Format

The .ini format is not properly specified, so I will note how these work:

- Support for String, Boolean, and List of String types for values (will add integers at some point if necessary)
- Case-sensitive key names
- Leading whitespace after '=' is ignored, but trailing and interspersed whitespace in strings is NOT ignored
- Values with defaults do not have to be specified unless you wish to change them.

### Compiler Settings

Under the `[compiler]` section, you can set the following options:

| Option      | Type   | Default           | Description                                                   |
|:-----------:|:------:|:-----------------:|:-------------------------------------------------------------:|
| compiler    | String | gcc               | Compiler to use                                               |
| flags       | String | -O2 -Wall -Werror | Flags to use when building individual C files in release mode |
| debugflags  | String | -g                | Flags to use when building individual C files in debug mode   |
| linker      | String | gcc               | Linker to use                                                 |
| linkerflags | String |                   | Flags to add to linker                                        |

### Project Settings

Under the `[project]` section, you can set the following options:

| Option     | Type   | Default            | Description                                                        |
|:----------:|:------:|:------------------:|:------------------------------------------------------------------:|
| name       | String | None. Must specify | Name of the project                                                |
| isbin      | Bool   | true               | True means it produces a binary. False means it produces a library |
| iscpp      | Bool   | false              | True means look for .cpp and .hpp. False means look for .c and .h  |
| include    | List   | include            | Where to look for .h files                                         |
| source     | List   | src                | Where to look for .c files                                         |
| build      | String | obj                | Where to put .o files                                              |

### Shell Commands

Every string value supports running shell code by placing it between `$(...)`.

Consider: `$(pkg-config --cflags SDL2_image)`

## Usage

From a folder with an `acbs.ini` file, run the following commands:

| Command | Description                                                        |
|:-------:|:------------------------------------------------------------------:|
| build   | Using release flags, build changed files into a binary or library. |
| debug   | Clean and build all C files using debug flags.                     |
| clean   | Clean up binaries and object files                                 |

You can optionally specify the folder containing an acbs.ini by providing it as a second argument.

