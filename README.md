# Desert-Eagle

启动2个server，再启动client。

### boost

使用了boost的asio模块，需要下载安装boost的dll

[boost 二进制安装地址](https://sourceforge.net/projects/boost/files/boost-binaries/)

配置头文件和lib库到vc的工程路径。


### support

* http-flv

* rtsp，just rtp over rtsp

* hls

### test

#### http-flv

``` 
http-flv 使用flv.js进行浏览，启动webserver，默认有个链接

就是client推上来的流，直接点击链接浏览。
```

#### rtsp

```
use live555 test rtp over rtsp

test rtp over rtsp, 在 工具-> 首选项 -> 输入/编解码器  

live555流传输 选择 rtp over rtsp(tcp)

use this url ：  rtsp://127.0.0.1/realplay?deviceid=123abcdef32153421 
```

#### hls

``` 
stream_hub 里面的这两个变量，设置了m3u8文件切片目录，以及ts文件的http前缀

m_m3u8_ts_directory = "D:\\github\\Desert-Eagle\\webserver\\static\\";
m_m3u8_ts_prefix = "http://172.16.64.92/static/"; 

http://172.16.64.92/static/123abcdef32153421.m3u8

m3u8文件的文件名称就是推流的时候，自定义的流名称。

使用vlc直接访问m3u8文件播放正常。 
```

### 待完成

* 增加webrtc的服务器支持，进行单向码流转发
* hls+


先研究下janus，在搞hls+吧


thisismaster
