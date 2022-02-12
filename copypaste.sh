#!/bin/bash
# https://habr.com/ru/post/266375
# xwininfo -root -tree
# xdotool key --window "$(xdotool getactivewindow)" F5

#W=`xdotool getactivewindow` # bug with idea
S1=$(xdotool getwindowfocus getwindowname)
S2="st"                                                  # Мой любимый терминал
#S3="XTerm"                                              # Мой второй терминал

if [ $1 = "copy" ]; then                                   # Проверяем аргумент на copy
#    if [ $S1 = $S2 ] || [ $S1 = $S3 ]; then               # Если это терминал
    if [ $S1 = $S2 ]; then               # Если это терминал
        xvkbd -xsendevent -text '\[Alt_L]\[Shift_L]\[C]'   # Отправляем [Alt]+[Shift]+[C]
    else                                                   # Если нет
        xvkbd -xsendevent -text '\[Control_L]c'            # Отправляем [Ctrl]+[C]
    fi
elif [ $1 = "paste" ]; then                                # Тоже самое для аргумента paste
#    if [ $S1 = $S2 ] || [ $S1 = $S3 ]; then
    if [ $S1 = $S2 ]; then
        xvkbd -xsendevent -text '\[Alt_L]\[Shift_L]\[V]'
    else
        xvkbd -xsendevent -text '\[Control_L]v'
    fi
fi
