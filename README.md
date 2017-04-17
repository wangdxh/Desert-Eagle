# Desert-Eagle
boost asio flv video  http-flv http flv html5 flv.js

启动2个server，再启动client，chrome网页输入ip地址即可。

### boost

使用了boost的asio模块，需要下载安装boost的dll

[boost 二进制安装地址](https://sourceforge.net/projects/boost/files/boost-binaries/)

配置头文件和lib库到vc的工程路径。


### support
http-flv

rtsp，just rtp over rtsp,sdp暂时没有生成sps和pps,会在码流里面传输

hls

### test
use live555 test rtp over rtsp

test rtp over rtsp, 在 工具-> 首选项 -> 输入/编解码器  

live555流传输 选择 rtp over rtsp(tcp)

use this url ：  rtsp://127.0.0.1/realplay?deviceid=123abcdef32153421



### 待完成

* hls+
* 增加webrtc的服务器支持，进行单向码流转发


thisismaster
