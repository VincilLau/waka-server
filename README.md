# waka-server

自托管的编码时长计时工具，基于[wakatime API](https://wakatime.com/developers)。

## 构建

```bash
git clone https://github.com/VincilLau/waka-server.git
xmake build waka-server
```

## 安装和运行

```bash
xmake run waka-server -i
xmake run waka-server
```

访问[http://127.0.0.1:8080](http://127.0.0.1:8080)查看waka-server是否正常工作。

修改wakatime插件配置，添加`api_url = "http://127.0.0.1:8080"`。

更多用法请执行`xmake run waka-server --help`

## 维护者

[@Vincil Lau](https://github.com/VincilLau)

## 使用许可

[Apache 2.0](./LICENSE)
