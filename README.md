![GitHub Stars](https://img.shields.io/github/stars/solarity-lab/Suny?style=social)
![GitHub Forks](https://img.shields.io/github/forks/solarity-lab/Suny?style=social)
![Repo Size](https://img.shields.io/github/repo-size/solarity-lab/Suny)
![Issues](https://img.shields.io/github/issues/solarity-lab/Suny)
![License](https://img.shields.io/github/license/solarity-lab/Suny)
![C](https://img.shields.io/badge/Made%20with-C-red.svg)

<p align="center">
  <img src="assets/LOGO.png" alt="logo" width="200"/>
</p>

# 🌇 Suny Programming Language

**Suny** is a programming language built from scratch in **pure C**,
designed to help you understand **how interpreters, VMs, and language runtimes actually work** — without magic.

> If you enjoy reading compiler or VM source code more than tutorials,
> **Suny is for you.**

Suny is not a wrapper around existing tools. It does not rely on parser generators. It does not use external VMs. Everything — from lexer to memory manager — is implemented manually.

---

## What is Suny? 👀

Suny is a small but complete language implementation focused on transparency and language internals.

It is designed to be:

* readable
* hackable
* breakable
* understandable

There are no hidden layers, no external runtime, and no black boxes.

---

## Goals 🎯

Suny focuses on the core ideas behind real languages:

* Lexing & parsing
* AST construction
* Execution / VM-like runtime
* Object system & memory management
* Manual implementation of language features

Everything is implemented **by hand**, in C.

* Suny prioritizes clarity over performance and simplicity over features.

---

## Architecture Overview 🧠

Suny follows a traditional language pipeline:

Source Code  
→ Lexer  

→ Parser  

→ AST

→ Compiler

→ Runtime Execution  

→ Garbage Collection  

All implemented manually in C.

---

## Example ✨

```suny
function bubble_sort(a) do
    n = size(a)
    i = 0
    while i < n do
        swapped = false
        j = 0
        while j < n - i - 1 do
            if a[j] > a[j + 1] then
                tmp = a[j]
                a[j] = a[j + 1]
                a[j + 1] = tmp
                swapped = true
            end
            j = j + 1
        end

        if not swapped then
            break
        end

        i = i + 1
    end
    return a
end

print(bubble_sort([2, 3, 1, 5, 3]))
```

Output:

```
[1, 2, 3, 3, 5]
```

Simple syntax.
Explicit behavior.
No hidden magic.

---

## Why Suny? 🚀

* Written in **pure C**
* No external runtime or VM dependencies
* Easy-to-read source code
* Designed for experimentation
* Encourages breaking and modifying internals

Suny exists to answer questions like:

> *“How does a language actually work under the hood?”*

---

## Who is this for? 👨‍💻👩‍💻

### ✅ You should use Suny if:

* You want to learn how programming languages are implemented
* You are curious about interpreters, VMs, or compilers
* You enjoy low-level or systems programming
* You like reading real runtime code

### ❌ You should NOT use Suny if:

* You want a fast, production-ready language
* You want high-level abstractions without touching memory
* Pointers and manual memory management scare you

---

## Build & Run 🛠️

### Requirements

* **OS**: Windows 10+ or Linux
* **Compiler**: GCC (MinGW on Windows) or MSVC
* **Tools**: `make`

### Build

```bash
git clone https://github.com/solarity-lab/Suny.git
cd Suny/Suny
make
```

After building, you’ll get:

```
suny
```

(or `suny.exe` on Windows)

This is the Suny interpreter entry point.

Or you can install binary version of Suny using [this link](https://github.com/dinhsonhai132/Suny/raw/refs/heads/main/SunySetup.exe)

---

## Documentation 📚

* 📄 **Language specification & internals**
  [SRM1 Document (PDF)](./SRM/SRM1/document.pdf)

* 🎥 **Implementation walkthroughs & dev logs**
  [https://www.youtube.com/@haidinhson](https://www.youtube.com/@SolarityLabOfficial)




