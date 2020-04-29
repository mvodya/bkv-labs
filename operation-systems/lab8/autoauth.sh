#!/bin/bash
# Operation systems / Auto key spread - Lection 16.04 / by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev
input="hosts.txt"
#     перебор строк в файле    копируем ключ    не проверяем fingerprint
while IFS=" " read -r addr; do ssh-copy-id -o "StrictHostKeyChecking no" $addr -f; done < "$input"