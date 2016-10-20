PREFIX=/usr

shadowsocks-gtk3: main.c
	gcc main.c `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1` -o shadowsocks-gtk3

install: shadowsocks-gtk3
	cp -v shadowsocks-gtk3-menu-icon.png ${PREFIX}/share/icons/hicolor/16x16/apps/
	gtk-update-icon-cache -f -t ${PREFIX}/share/icons/hicolor/16x16/
	cp shadowsocks-gtk3 ${PREFIX}/bin

uninstall:
	rm -v ${PREFIX}/share/icons/hicolor/16x16/apps/shadowsocks-gtk3-menu-icon.png
	gtk-update-icon-cache -f -t ${PREFIX}/share/icons/hicolor/16x16/
	rm -v ${PREFIX}/bin/shadowsocks-gtk3
