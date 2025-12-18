Here is an expanded **English explanation** you can insert under the *Closure* section:

---

### **Closure Explanation**

A **closure** is a function that can *remember and access* variables from the scope in which it was created, even after that outer scope has finished executing.

In other words:

> **Closure = Function + Its captured environment (the variables it keeps).**

This allows functions to “carry” state and behave like objects with private data.

---

### **Detailed Example**

```suny
function foo() do
    count = 0
    return function() do
        count = count + 1
        return count
    end
end

a = foo()
for i in range(10) do
    print(a())
end
```

Here’s what happens:

1. `foo()` is called. Inside it, the variable `count` is created.
2. `foo()` returns an inner function that **uses `count`**.
3. Even though `foo()` has finished, `count` does **not** disappear.
4. The returned function `a()` keeps a reference to `count`.
   This saved environment is the **closure**.
5. Each time `a()` is called, it updates the same `count` value.

So the function “remembers” its state:

```
1
2
3
...
10
```