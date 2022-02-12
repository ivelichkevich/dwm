#!/bin/bash

#xprop -id $(xdotool getactivewindow) |awk '/WM_CLASS/{print $4}' >> /home/ihar/act_win
#xdotool getactivewindow >> /home/ihar/act_win
#echo "|" >> /home/ihar/act_win
#xprop -id $(xdotool getactivewindow) >> /home/ihar/act_win
#echo "|" >> /home/ihar/act_win
xdotool getwindowfocus getwindowname  >> /home/ihar/act_win
