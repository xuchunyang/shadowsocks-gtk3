PREFIX=/usr

shadowsocks-gtk3: main.c
	gcc main.c `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1` -o shadowsocks-gtk3

install: shadowsocks-gtk3
	@echo "Installing icons..."
	cp shadowsocks-gtk3.png ${PREFIX}/share/icons/hicolor/512x512/apps/
	cp shadowsocks-gtk3-menu-icon.png ${PREFIX}/share/icons/hicolor/16x16/apps/
	gtk-update-icon-cache -f -t ${PREFIX}/share/icons/hicolor/

	@echo "Installing desktop files..."
	cp shadowsocks-gtk3.desktop ${PREFIX}/share/applications/
	cp shadowsocks-gtk3-startup.desktop /etc/xdg/autostart/

	@echo "Installing binary..."
	cp shadowsocks-gtk3 ${PREFIX}/bin

uninstall:
	@echo "Removing icons..."
	rm ${PREFIX}/share/icons/hicolor/16x16/apps/shadowsocks-gtk3-menu-icon.png
	rm ${PREFIX}/share/icons/hicolor/512x512/apps/shadowsocks-gtk3.png
	gtk-update-icon-cache -f -t ${PREFIX}/share/icons/hicolor/16x16/

	@echo "Removing desktop files..."
	rm ${PREFIX}/share/applications/shadowsocks-gtk3.desktop
	rm /etc/xdg/autostart/shadowsocks-gtk3-startup.desktop

	@echo "Removing binary..."
	rm ${PREFIX}/bin/shadowsocks-gtk3
