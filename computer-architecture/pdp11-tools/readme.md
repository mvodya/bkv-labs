# Набор инструментов для удобного билда MACRO11

Состоит из:

* Ассемблер [MACRO11 (win32)](https://github.com/j-hoppe/MACRO11) (уже в комплекте)
* Retroscoper (@mvodya) - превращает листинг в набор команд для симулятора pdp11
* run.sh / wsl_run.sh - скрипты автоматизации

## Как использовать

Для Linux с установленным wine:

```bash
pdp11-tools/run.sh <имя_исходников_macro11>
```

Для Windows с установленным wsl:

```bash
pdp11-tools/wsl_run.sh <имя_исходников_macro11>
```

В случае использования Windows, simh должен быть установлен в подсистеме linux (`apt install simh`)

Программа создаст листинг из ассемблера (`listing.txt`) и файл комманд для симулятора (`code.txt`). После этого будет автоматически запущен pdp11 и в него будет загружено содержимое файла `code.txt`.

---

А почему так все сложно и странно? Потому что MACRO11 (точнее его нормальная реплика) есть только под Windows, а симулятор PDP11 гораздо удобнее стартовать из под Linux (или wsl). От того такие странные кросс запуски программ в разных средах. 