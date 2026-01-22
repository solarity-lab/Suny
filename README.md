<p align="center">
  <img src="assets/logo.png" alt="logo" width="200"/>
</p>


# 🌇 Suny Programming Language

**Suny** is a small programming language built from scratch in **pure C**,
designed to help you understand **how interpreters, VMs, and language runtimes actually work** — without magic.

> If you enjoy reading compiler or VM source code more than tutorials,
> **Suny is for you.**

---

## What is Suny? 👀

Suny is a **minimal, educational programming language** with a strong focus on **language internals**, not end-user features.

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

This is **not** a production language.
This **is** a language you read to learn.

---

## Example ✨

```suny
import "HashMap"

a = map([
    "Alex", 15,
    "Max", 10
])

a["Bob"] = 4
print(a)
```

Output:

```
{Alex: 15, Max: 10, Bob: 4}
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
cd Suny
make
```

After building, you’ll get:

```
suny
```

(or `suny.exe` on Windows)

This is the Suny interpreter entry point.

---

## Documentation 📚

* 📄 **Language specification & internals**
  [SRM1 Document (PDF)](./SRM/SRM1/document.pdf)

* 🎥 **Implementation walkthroughs & dev logs**
  [https://www.youtube.com/@haidinhson](https://www.youtube.com/@haidinhson)


