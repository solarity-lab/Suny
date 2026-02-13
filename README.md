# The Suny Programming Language

<p align="center">
  <img src="assets/LOGO.png" alt="logo" width="200"/>
</p>

## Introduction

**Suny** is a lightweight scripting language designed to be **small but powerful**, combining the minimalism and efficiency of **Lua** with the clarity and readability of **Python**.

Suny was created as an experiment: *Can one person mind design and build a language from scratch that is both minimal and useful?*   

Many modern languages are large, with complex standard libraries, heavy runtime systems, and thousands of pages of documentation. While this makes them powerful, it also makes them intimidating to beginners and difficult to fully understand.

Lua inspired Suny by showing that a small, elegant core can be extremely flexible. Python inspired Suny with its philosophy of readability and simplicity. Suny combines both ideas: a minimal implementation with a syntax that feels natural and beginner-friendly.

## Example

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

---

## Build & Run

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

## Documentation

* **Language specification & internals**
  [SRM1 Document (PDF)](./SRM/SRM1/document.pdf)

* **Implementation walkthroughs & dev logs**
  [https://www.youtube.com/@haidinhson](https://www.youtube.com/@SolarityLabOfficial)

