# Fibonacci series for i386

## Задача

Сделать расчет чисел Фибоначчи (до тех пор, пока не произойдет переполнение), используя 32 bit вычисления.

## How to build

Create object file, using `nasm`:

```bash
nasm -f elf task7.asm -o task7.o
```

Run linker with object file and make executable:

```bash
ld -m elf_i386 task7.o -o task7
```

Then just run `task7`.
