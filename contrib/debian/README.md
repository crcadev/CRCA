
Debian
====================
This directory contains files used to package crcad/crca-qt
for Debian-based Linux systems. If you compile crcad/crca-qt yourself, there are some useful files here.

## crca: URI support ##


crca-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install crca-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your crcaqt binary to `/usr/bin`
and the `../../share/pixmaps/crca128.png` to `/usr/share/pixmaps`

crca-qt.protocol (KDE)
