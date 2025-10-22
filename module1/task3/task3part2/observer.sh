#!/bin/bash

CONFIG_FILE="observer.conf"
LOG_FILE="observer.log"

log_message() {
local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
echo "[$timestamp] $1" >> "$LOG_FILE"
}

check_config_file() {
if [[ ! -f "$CONFIG_FILE" ]]; then
log_message "ОШИБКА: Файл конфигурации $CONFIG_FILE не найден"
exit 1
fi
}

is_process_running() {
local script_path="$1"
local script_name=$(basename "$script_path")
local found=false

for pid_dir in /proc/[0-9]*; do
if [[ -d "$pid_dir" && -r "$pid_dir/cmdline" ]]; then
local cmdline=$(tr -d '\0' < "$pid_dir/cmdline" 2>/dev/null)
if [[ "$cmdline" == *"$script_name"* ]] || [[ "$cmdline" == *"$script_path"* ]]; then
log_message "Процессс $script_name найден в $cmdline"
return 0
fi
fi
done
return 1
}


start_script() {
local script_path="$1"
if [[ ! -f "$script_path" ]]; then
log_message "ОШИБКА: $script_path не найден"
return 1
fi

if [[ ! -x "$script_path" ]]; then
chmod +x "$script_path"
fi

local script_dir=$(dirname "$script_path")
cd "$script_dir" || {
log_message "Не удалось найти директорию $script_dir"
return 1
}

if nohup "./$(basename "$script_path")" > /dev/null 2>&1 & then
local new_pid=$!
log_message "Успех скрипт $script_path запущен PID: $new_pid"
cd - > /dev/null 2>&1
return 0
else
log_message "Ошибка не удалось запустить $script_path"
cd - > /dev/null 2>&1
return 1
fi
}


main() {
log_message "Запуск observer.sh"
check_config_file

while IFS= read -r line || [[ -n "$line" ]]; do
if [[ -z "$line" || "$line" =~ ^[[:space:]]*#  ]]; then
continue
fi

script_path=$(echo "$line" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
script_name=$(basename "$script_path")

log_message "Проверка $script_path"

if is_process_running "$script_path"; then
log_message "СКРИПТ РАБОТАЕТ: $script_path"
else
log_message "ПЕРЕЗАПУСК $script_path не найден в процессах"
start_script "$script_path"
fi

done < "$CONFIG_FILE"

log_message "Завершение observer.sh"
echo "" >> "$LOG_FILE"
}

main

