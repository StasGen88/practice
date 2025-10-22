#!/bin/bash

SCRIPT_NAME=$(basename "$0")

if [ "$SCRIPT_NAME" = "template_task.sh" ]; then
echo "я бригадир, сам не работаю"
exit 1
fi

REPORT_FILE="report_${SCRIPT_NAME}.log"

PID=$$

echo "[$PID] $(date '+%Y-%m-%d %H:%M:%S') Скрипт запущен" >> "$REPORT_FILE"
RANDOM_SECONDS=$(( RANDOM % 1770 + 30))
sleep $RANDOM_SECONDS
MINUTES=$(( (RANDOM_SECONDS + 59) / 60 ))

echo "[$PID] $(date '+%Y-%m-%d %H:%M:%S') Скрипт завершился, работал $MINUTES минут" >> "$REPORT_FILE"
