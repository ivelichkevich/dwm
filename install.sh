sudo pacman -S xorg-server xorg-xinit xorg-xrandr xorg-xsetroot
sudo pacman -S libxinerama ttf-liberation
yay -S xorg-xinput
yay -S xev
yay -S libxft-bgra
yay -S xdotool
yay -S xvkbd
yay -S scrot

make && sudo make clean install

