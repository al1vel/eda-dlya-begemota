#!/bin/bash

# Функция для подсчета числа строк и суммарной длины в строках
count_lines() {
    local dir="$1"
    local file_ext=".h .cpp"
    local total_lines=0
    local total_length=0

    # Перебираем файлы в указанной директории и ее подкаталогах
    find "$dir" -type f \( -name "*.h" -o -name "*.cpp" \) | while read -r file; do
        # Подсчитываем число строк и суммарную длину в строках в каждом файле
        lines=$(wc -l < "$file")
        length=$(wc -c < "$file")

        # Добавляем число строк и суммарную длину к общим значениям
        total_lines=$((total_lines + lines))
        total_length=$((total_length + length))
    done

    echo "Всего строк: $total_lines"
    echo "Суммарная длина в строках: $total_length байт"
}

# Задаем директорию, для которой нужно выполнить подсчет
directory="/path/to/directory"

# Вызываем функцию для подсчета числа строк и суммарной длины
count_lines "$directory"
