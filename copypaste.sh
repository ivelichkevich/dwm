#!/bin/bash
# https://habr.com/ru/post/266375


W=`xdotool getactivewindow`
S1=`xprop -id ${W} |awk '/WM_CLASS/{print $4}'`
S2='"st-256color"'                                         # Мой любимый терминал
S3='"XTerm"'                                               # Мой второй терминал

if [ $1 = "copy" ]; then                                   # Проверяем аргумент на copy                                    
    if [ $S1 = $S2 ] || [ $S1 = $S3 ]; then                # Если это терминал
        xvkbd -xsendevent -text '\[Alt_L]\[Shift_L]\[C]'   # Отправляем [Alt]+[Shift]+[C]
    else                                                   # Если нет
        xvkbd -xsendevent -text '\[Control_L]c'            # Отправляем [Ctrl]+[C]
    fi  
elif [ $1 = "paste" ]; then                                # Тоже самое для аргумента paste
    if [ $S1 = $S2 ] || [ $S1 = $S3 ]; then
        xvkbd -xsendevent -text '\[Alt_L]\[Shift_L]\[V]'
    else
        xvkbd -xsendevent -text '\[Control_L]v'
    fi  
fi