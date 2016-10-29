一只用来控制 shadowsocks 的托盘小程序。

![shadowsocks-gtk3-v0.2.0](http://i.imgur.com/eLOwwnE.png)
## 依赖

库依赖：
- GTK+ 3.0
- appindicator3

安装依赖：
- gcc, make, pkg-config

运行依赖：
- 使用 GSettings 配置代理的 GNU/Linux 桌面环境，比如 Unity 和 Gnome。

## 安装

    sudo make install

## 使用

使用系统设置配置代理，比如在 Ubuntu 下

    System Settings -> Network -> Network proxy

使用 shadowsocks-gtk3 切换代理。

## 卸载

    sudo make uninstall

