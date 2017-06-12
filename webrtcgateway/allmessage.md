---------------------------------------------------
  Starting Meetecho Janus (WebRTC Gateway) v0.2.0
---------------------------------------------------

[janus.cfg]
    [general]
        configs_folder: ./file
        plugins_folder: ./plugins
        transports_folder: ./transports
        debug_level: 7
        admin_secret: janusoverlord
    [certificates]
        cert_pem: .\file\certs\mycert.pem
        cert_key: .\file\certs\mycert.key
    [media]
    [nat]
        nice_debug: false
        ice_ignore_list: vmnet
    [plugins]
    [transports]
Checking command line arguments...
[janus.cfg]
    [general]
        configs_folder: ./file
        plugins_folder: ./plugins
        transports_folder: ./transports
        debug_level: 7
        admin_secret: janusoverlord
    [certificates]
        cert_pem: .\file\certs\mycert.pem
        cert_key: .\file\certs\mycert.key
    [media]
    [nat]
        nice_debug: false
        ice_ignore_list: vmnet
    [plugins]
    [transports]
Debug/log level is 7
Debug/log timestamps are disabled
Debug/log colors are enabled
Adding 'vmnet' to the ICE ignore list...
Selecting local IP address...
Autodetecting local IP...
Using 172.16.64.92 as local IP...
Token based authentication disabled
Initializing ICE stuff (Full mode, ICE-TCP candidates disabled, IPv6 support disabled)
Janus is deployed on a private address (172.16.64.92) but you didn't specify any STUN server! Expect trouble if this is
supposed to work over the internet and not just in a LAN...
BUNDLE is NOT going to be forced
rtcp-mux is NOT going to be forced
  -- RTCP component blackhole socket bound to port 56444
Using certificates:
        .\file\certs\mycert.pem
        .\file\certs\mycert.key
ICE handles watchdog started
OpenSSL pre-1.1.0
PEM_read_X509 will start
Fingerprint of our certificate: D2:B9:31:8F:DF:24:D8:0E:ED:D2:EF:25:9E:AF:6F:B8:34:AE:53:9C:E6:F3:8F:F2:64:15:FA:E8:7F:53:2D:38
Data Channels support not compiled
Plugins folder: ./plugins
JANUS Streaming plugin created!
Configuration file: ./file/janus.plugin.streaming.cfg
[janus.plugin.streaming.cfg]
    [general]
    [gstreamer-sample]
        type: rtp
        id: 1
        description: h264 live stream coming from gstreamer
        audio: no
        video: yes
        videoport: 5004
        videopt: 96
        videortpmap: H264/90000
        secret: adminpwd
    [file-live-sample]
        type: live
        id: 2
        description: a-law file source (radio broadcast)
        filename: ./file/radio.alaw
        audio: yes
        video: no
        secret: adminpwd
    [file-ondemand-sample]
        type: ondemand
        id: 3
        description: mu-law file source (music)
        filename: ./file/music.mulaw
        audio: yes
        video: no
        secret: adminpwd
Adding stream 'gstreamer-sample'
Audio NOT enabled, Video enabled
Audio NOT enabled, Video enabled
Adding stream 'file-live-sample'
Sessions watchdog started
Adding stream 'file-ondemand-sample'
Starting streaming relay thread
  ::: [1][gstreamer-sample] h264 live stream coming from gstreamer (live, RTP source, public, pin: no pin)
  ::: [2][file-live-sample] a-law file source (radio broadcast) (live, file source, public, pin: no pin)
  ::: [3][file-ondemand-sample] mu-law file source (music) (on demand, file source, public, pin: no pin)
JANUS Streaming plugin initialized!
        Version: 6 (0.0.6)
           [janus.plugin.streaming] JANUS Streaming plugin
           This is a streaming plugin for Janus, allowing WebRTC peers to watch/listen to pre-recorded files or media ge
nerated by gstreamer.
           Plugin API version: 7
Transport plugins folder: ./transports
JANUS REST (HTTP/HTTPS) transport plugin created!
Configuration file: ./file/janus.transport.http.cfg
[janus.transport.http.cfg]
    [general]
        json: indented
        base_path: /janus
        threads: unlimited
        http: yes
        port: 8088
        https: no
    [admin]
        admin_base_path: /admin
        admin_threads: unlimited
        admin_http: no
        admin_port: 7088
        admin_https: no
    [certificates]
        cert_pem: ./file/certs/mycert.pem
        cert_key: ./file/certs/mycert.key
Using a thread per connection for the Janus API HTTP webserver
Binding to all interfaces for the Janus API HTTP webserver
Filesource (live) thread starting...
[file-live-sample] Opening file source ./file/radio.alaw...
[file-live-sample] Streaming audio file: ./file/radio.alaw
Joining Streaming handler thread
Streaming watchdog started
HTTP webserver started (port 8088, /janus path listener)...
Using certificates:
        ./file/certs/mycert.pem
        ./file/certs/mycert.key
HTTPS webserver disabled
Admin/monitor HTTP webserver disabled
Admin/monitor HTTPS webserver disabled
JANUS REST (HTTP/HTTPS) transport plugin initialized!
        Version: 2 (0.0.2)
           [janus.transport.http] JANUS REST (HTTP/HTTPS) transport plugin
           This transport plugin adds REST (HTTP/HTTPS) support to the Janus API via libmicrohttpd.
           Plugin API version: 5
           Janus API: enabled
           Admin API: disabled
HTTP/Janus sessions watchdog started


New connection on REST API: ::ffff:172.16.64.92
Got a HTTP OPTIONS request on /janus...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 47
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus...
 ... parsing request...
Processing POST data (application/json) (47 bytes)...
  -- Data we have now (47 bytes)
Processing HTTP POST request on /janus...
 ... parsing request...
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"create","transaction":"mzLbAMkH15IM"}
Forwarding request to the core (004BCE98)
Got a Janus API request from janus.transport.http (004BCE98)
Transport task pool, serving request
Creating new session: 5507369527856718
Session created (5507369527856718), create a queue for the long poll
Sending Janus API response to janus.transport.http (004BCE98)
Got a Janus API response to send (004BCE98)
Request completed, freeing data
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (004BCE98)
Session 5507369527856718 found... returning up to 1 messages
... handling long poll...
Transport task pool, serving request
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (004BCE98)
Got a Janus API response to send (004BCE98)
New connection on REST API: ::ffff:172.16.64.92
Got a HTTP OPTIONS request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 81
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Processing POST data (application/json) (81 bytes)...
  -- Data we have now (81 bytes)
Processing HTTP POST request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"attach","plugin":"janus.plugin.streaming","transaction":"MAm860DVlVTZ"}
Forwarding request to the core (00E24A90)
Got a Janus API request from janus.transport.http (00E24A90)
Transport task pool, serving request
Creating new handle in session 5507369527856718: 861716883046715
Sending Janus API response to janus.transport.http (00E24A90)
Got a Janus API response to send (00E24A90)
Request completed, freeing data
Got a HTTP OPTIONS request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 74
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (74 bytes)...
  -- Data we have now (74 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"message","body":{"request":"list"},"transaction":"ZuIyyBGpgzdM"}
Forwarding request to the core (00E24A90)
Got a Janus API request from janus.transport.http (00E24A90)
Transport task pool, serving request
[861716883046715] There's a message for JANUS Streaming plugin
Request for the list of mountpoints
Creating plugin result...
Sending Janus API response to janus.transport.http (00E24A90)
Got a Janus API response to send (00E24A90)
Destroying plugin result...
Request completed, freeing data
Got a HTTP OPTIONS request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 82
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (82 bytes)...
  -- Data we have now (82 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"message","body":{"request":"watch","id":3},"transaction":"pNcztAvNzy4K"}
Forwarding request to the core (00E24A90)
Got a Janus API request from janus.transport.http (00E24A90)
Transport task pool, serving request
[861716883046715] There's a message for JANUS Streaming plugin
Creating plugin result...
Sending Janus API response to janus.transport.http (00E24A90)
Got a Janus API response to send (00E24A90)
Request completed, freeing data
Destroying plugin result...
Request to watch mountpoint/stream 3
Going to offer this SDP:
v=0
o=- 1495799811084970 1495799811084970 IN IP4 127.0.0.1
s=Streaming Test
t=0 0
m=audio 1 RTP/SAVPF 0
c=IN IP4 1.1.1.1
a=rtpmap:0 PCMU/8000
a=sendonly

[861716883046715] Audio has been negotiated
[861716883046715] Video has NOT been negotiated
[861716883046715] SCTP/DataChannels have NOT been negotiated
[861716883046715] Setting ICE locally: got ANSWER (1 audios, 0 videos)
[861716883046715] Creating ICE agent (ICE Full mode, controlling)
[861716883046715] Adding 172.16.64.92 to the addresses to gather candidates for
[861716883046715] Gathering done for stream 1
 -------------------------------------------
  >> Anonymized (161 --> 161 bytes)
 -------------------------------------------
v=0
o=- 1495799811084970 1495799811084970 IN IP4 127.0.0.1
s=Streaming Test
t=0 0
m=audio 1 RTP/SAVPF 0
c=IN IP4 1.1.1.1
a=sendonly
a=rtpmap:0 PCMU/8000

[861716883046715] No RTP maps?? trying formats...
[861716883046715] We have 3 candidates for Stream #1, Component #1
[861716883046715] Stream #1, Component #1
[861716883046715]   Address:    172.16.64.92:58724
[861716883046715]   Priority:   2013266431
[861716883046715]   Foundation: 1
[861716883046715]     a=candidate:1 1 udp 2013266431 172.16.64.92 58724 typ host
[861716883046715] Stream #1, Component #1
[861716883046715]   Address:    172.16.64.92:0
[861716883046715]   Priority:   1019216383
[861716883046715]   Foundation: 2
[861716883046715] Skipping host TCP candidate, ICE-TCP support disabled...
[861716883046715] Stream #1, Component #1
[861716883046715]   Address:    172.16.64.92:19781
[861716883046715]   Priority:   1015022079
[861716883046715]   Foundation: 3
[861716883046715] Skipping host TCP candidate, ICE-TCP support disabled...
[861716883046715] We have 3 candidates for Stream #1, Component #2
[861716883046715] Stream #1, Component #2
[861716883046715]   Address:    172.16.64.92:58725
[861716883046715]   Priority:   2013266430
[861716883046715]   Foundation: 1
[861716883046715]     a=candidate:1 2 udp 2013266430 172.16.64.92 58725 typ host
[861716883046715] Stream #1, Component #2
[861716883046715]   Address:    172.16.64.92:0
[861716883046715]   Priority:   1019216382
[861716883046715]   Foundation: 2
[861716883046715] Skipping host TCP candidate, ICE-TCP support disabled...
[861716883046715] Stream #1, Component #2
[861716883046715]   Address:    172.16.64.92:19782
[861716883046715]   Priority:   1015022078
[861716883046715]   Foundation: 3
[861716883046715] Skipping host TCP candidate, ICE-TCP support disabled...
 -------------------------------------------
  >> Merged (161 --> 733 bytes)
 -------------------------------------------
v=0
o=- 1495799811084970 1495799811084970 IN IP4 172.16.64.92
s=Streaming Test
t=0 0
a=group:BUNDLE audio
a=msid-semantic: WMS janus
m=audio 1 RTP/SAVPF 0
c=IN IP4 172.16.64.92
a=mid:audio
a=sendonly
a=rtcp-mux
a=ice-ufrag:KbYF
a=ice-pwd:UibYJJTuYcd7F/wmOseax6
a=ice-options:trickle
a=fingerprint:sha-256 D2:B9:31:8F:DF:24:D8:0E:ED:D2:EF:25:9E:AF:6F:B8:34:AE:53:9C:E6:F3:8F:F2:64:15:FA:E8:7F:53:2D:38
a=setup:actpass
a=connection:new
a=rtpmap:0 PCMU/8000
a=ssrc:-537150489 cname:janusaudio
a=ssrc:-537150489 msid:janus janusa0
a=ssrc:-537150489 mslabel:janus
a=ssrc:-537150489 label:janusa0
a=candidate:1 1 udp 2013266431 172.16.64.92 58724 typ host
a=candidate:1 2 udp 2013266430 172.16.64.92 58725 typ host

[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
  >> Pushing event: 0 (Success)
Filesource (on demand) RTP thread starting...
[file-ondemand-sample] Opening file source ./file/music.mulaw...
[file-ondemand-sample] Streaming audio file: ./file/music.mulaw
[861716883046715] ICE thread started
We have a message to serve...
        {
   "janus": "event",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "transaction": "pNcztAvNzy4K",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "preparing"
         }
      }
   },
   "jsep": {
      "type": "offer",
      "sdp": "v=0\r\no=- 1495799811084970 1495799811084970 IN IP4 172.16.64.92\r\ns=Streaming Test\r\nt=0 0\r\na=group:B
UNDLE audio\r\na=msid-semantic: WMS janus\r\nm=audio 1 RTP/SAVPF 0\r\nc=IN IP4 172.16.64.92\r\na=mid:audio\r\na=sendonly
\r\na=rtcp-mux\r\na=ice-ufrag:KbYF\r\na=ice-pwd:UibYJJTuYcd7F/wmOseax6\r\na=ice-options:trickle\r\na=fingerprint:sha-256
 D2:B9:31:8F:DF:24:D8:0E:ED:D2:EF:25:9E:AF:6F:B8:34:AE:53:9C:E6:F3:8F:F2:64:15:FA:E8:7F:53:2D:38\r\na=setup:actpass\r\na
=connection:new\r\na=rtpmap:0 PCMU/8000\r\na=ssrc:-537150489 cname:janusaudio\r\na=ssrc:-537150489 msid:janus janusa0\r\
na=ssrc:-537150489 mslabel:janus\r\na=ssrc:-537150489 label:janusa0\r\na=candidate:1 1 udp 2013266431 172.16.64.92 58724
 typ host\r\na=candidate:1 2 udp 2013266430 172.16.64.92 58725 typ host\r\n"
   }
}
Request completed, freeing data
[861716883046715] Looping (ICE)...
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 562
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (562 bytes)...
  -- Data we have now (562 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"message","body":{"request":"start"},"transaction":"yc7TZDFL3nJ0","jsep":{"type":"answer","sdp":"v=0\r\no=- 535
8450128380337511 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS\r\nm=audio 9 RTP/SAV
PF 0\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9 IN IP4 0.0.0.0\r\na=ice-ufrag:PeSQ\r\na=ice-pwd:EsKyz4fKNx4tvlsbx1uM1UQ/\r\na=finge
rprint:sha-256 3F:66:59:88:8A:01:60:F3:17:23:F7:DA:7B:C9:DA:A4:37:A8:4F:B8:64:BC:1A:2D:AF:0E:DC:BE:E4:E7:19:8C\r\na=setu
p:active\r\na=mid:audio\r\na=recvonly\r\na=rtcp-mux\r\na=rtpmap:0 PCMU/8000\r\n"}}
Forwarding request to the core (004BCE98)
Got a Janus API request from janus.transport.http (004BCE98)
Transport task pool, serving request
[861716883046715] There's a message for JANUS Streaming plugin
[861716883046715] Remote SDP:
v=0
o=- 5358450128380337511 2 IN IP4 127.0.0.1
s=-
t=0 0
a=group:BUNDLE audio
a=msid-semantic: WMS
m=audio 9 RTP/SAVPF 0
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-ufrag:PeSQ
a=ice-pwd:EsKyz4fKNx4tvlsbx1uM1UQ/
a=fingerprint:sha-256 3F:66:59:88:8A:01:60:F3:17:23:F7:DA:7B:C9:DA:A4:37:A8:4F:B8:64:BC:1A:2D:AF:0E:DC:BE:E4:E7:19:8C
a=setup:active
a=mid:audio
a=recvonly
a=rtcp-mux
a=rtpmap:0 PCMU/8000
[861716883046715] Audio has been negotiated, Video has NOT been negotiated, SCTP/DataChannels have NOT been negotiated
[861716883046715] The browser: supports BUNDLE, supports rtcp-mux, is doing Trickle ICE
[861716883046715] Parsing audio candidates (stream=1)...
[861716883046715] ICE ufrag (local):   PeSQ
[861716883046715] ICE pwd (local):     EsKyz4fKNx4tvlsbx1uM1UQ/
[861716883046715] Fingerprint (local) : sha-256 3F:66:59:88:8A:01:60:F3:17:23:F7:DA:7B:C9:DA:A4:37:A8:4F:B8:64:BC:1A:2D:
AF:0E:DC:BE:E4:E7:19:8C
[861716883046715] DTLS setup (local):  active
[861716883046715] Audio mid: audio
[861716883046715]   -- bundle is supported by the browser, getting rid of one of the RTP/RTCP components, if any...
[861716883046715]   -- rtcp-mux is supported by the browser, getting rid of RTCP components, if any...
[861716883046715] Discovered new remote candidate for component 2 in stream 1: foundation=1
[861716883046715]   -- ICE Trickling is supported by the browser, waiting for remote candidates...
 -------------------------------------------
  >> Anonymized (419 --> 136 bytes)
 -------------------------------------------
v=0
o=- 5358450128380337511 2 IN IP4 127.0.0.1
s=-
t=0 0
m=audio 1 RTP/SAVPF 0
c=IN IP4 1.1.1.1
a=recvonly
a=rtpmap:0 PCMU/8000

Creating plugin result...
Sending Janus API response to janus.transport.http (004BCE98)
Got a Janus API response to send (004BCE98)
Destroying plugin result...
Starting the streaming
This is involving a negotiation (answer) as well (but we really don't care):
v=0
o=- 5358450128380337511 2 IN IP4 127.0.0.1
s=-
t=0 0
m=audio 1 RTP/SAVPF 0
c=IN IP4 1.1.1.1
a=recvonly
a=rtpmap:0 PCMU/8000

[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
  >> Pushing event: 0 (Success)
[861716883046715] Still waiting for the DTLS stack for component 1 in stream 1...
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 217
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (217 bytes)...
  -- Data we have now (217 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"trickle","candidate":{"candidate":"candidate:2153010912 1 udp 2113937151 172.16.64.92 58726 typ host generatio
n 0 ufrag PeSQ network-cost 50","sdpMid":"audio","sdpMLineIndex":0},"transaction":"guKGT3pjXuXP"}
Forwarding request to the core (004BCE98)
Got a Janus API request from janus.transport.http (004BCE98)
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 79
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (79 bytes)...
  -- Data we have now (79 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"trickle","candidate":{"completed":true},"transaction":"jxVt80dHA2G6"}
Forwarding request to the core (00E24B70)
Got a Janus API request from janus.transport.http (00E24B70)
Transport task pool, serving request
[861716883046715] Trickle candidate (audio): candidate:2153010912 1 udp 2113937151 172.16.64.92 58726 typ host generatio
n 0 ufrag PeSQ network-cost 50
Transport task pool, serving request
[861716883046715]  Adding remote candidate component:1 stream:1 type:host 172.16.64.92:58726
[861716883046715]    Candidate added to the list! (1 elements for 1/1)
[861716883046715] ICE already started for this component, setting candidates we have up to now
[861716883046715] ## Setting remote candidates: stream 1, component 1 (1 in the list)
[861716883046715] >> Remote Stream #1, Component #1
[861716883046715]   Address:    172.16.64.92:58726
[861716883046715]   Priority:   2113937151
[861716883046715]   Foundation: 2153010912
[861716883046715]   Username:   PeSQ
[861716883046715]   Password:   EsKyz4fKNx4tvlsbx1uM1UQ/
[861716883046715]  Setting remote credentials...
[861716883046715] Component state changed for component 1 in stream 1: 2 (connecting)
[861716883046715] Remote candidates set!
Sending Janus API response to janus.transport.http (004BCE98)
Got a Janus API response to send (004BCE98)
Request completed, freeing data
No more remote candidates for handle 861716883046715!
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
Request completed, freeing data
[861716883046715] New selected pair for component 1 in stream 1: 1 <-> 2153010912
[861716883046715]   Component is ready enough, starting DTLS handshake...
janus_dtls_bio_filter_ctrl: 6
[861716883046715]   Setting accept state (DTLS server)
janus_dtls_bio_filter_ctrl: 6
[861716883046715] Creating retransmission timer with ID 7
[861716883046715] Component state changed for component 1 in stream 1: 3 (connected)
[861716883046715] ICE send thread started...
[861716883046715] Component state changed for component 1 in stream 1: 4 (ready)
[861716883046715] Looks like DTLS!
[861716883046715]     Written 163 bytes on the read BIO...
janus_dtls_bio_filter_ctrl: 49
Advertizing MTU: 1472
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_write: 00E5EF90, 1116
  -- 1116
New list length: 1
[861716883046715]     ... and read -1 of them from SSL...
[861716883046715] >> Going to send DTLS data: 1116 bytes
[861716883046715] >> >> Read 1116 bytes from the write_BIO...
[861716883046715] >> >> ... and sent 1116 of those bytes on the socket
[861716883046715] Initialization not finished yet...
[861716883046715] Looks like DTLS!
[861716883046715]     Written 592 bytes on the read BIO...
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_ctrl: 49
janus_dtls_bio_filter_write: 00E5EF90, 570
  -- 570
New list length: 1
janus_dtls_bio_filter_ctrl: 7
[861716883046715]     ... and read -1 of them from SSL...
[861716883046715] >> Going to send DTLS data: 570 bytes
[861716883046715] >> >> Read 570 bytes from the write_BIO...
[861716883046715] >> >> ... and sent 570 of those bytes on the socket
[861716883046715] DTLS established, yay!
[861716883046715] Computing sha-256 fingerprint of remote certificate...
[861716883046715] Remote fingerprint (sha-256) of the client is 3F:66:59:88:8A:01:60:F3:17:23:F7:DA:7B:C9:DA:A4:37:A8:4F
:B8:64:BC:1A:2D:AF:0E:DC:BE:E4:E7:19:8C
[861716883046715]  Fingerprint is a match!
[861716883046715] Created inbound SRTP session for component 1 in stream 1
[861716883046715] Created outbound SRTP session for component 1 in stream 1
[861716883046715] The DTLS handshake for the component 1 in stream 1 has been completed
[861716883046715] The DTLS handshake has been completed
[861716883046715] Telling the plugin about it (JANUS Streaming plugin)
WebRTC media is now available
[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
  >> Pushing event: 0 (Success)
[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
[861716883046715]  Got an RTCP packet (bundled stream)!
[861716883046715] Incoming RTCP, bundling: this is audio (no video has been negotiated)
[861716883046715]  DTLS already set up, disabling retransmission timer!
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
Transport task pool, serving request
Request completed, freeing data
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
Request completed, freeing data
Transport task pool, serving request
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
Transport task pool, serving request
Got a keep-alive on session 5507369527856718
Request completed, freeing data
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
Transport task pool, serving request
... handling long poll...
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715]  Got an RTCP packet (bundled stream)!
[861716883046715] Incoming RTCP, bundling: this is audio (no video has been negotiated)
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
[861716883046715] Fixing SSRCs (local 3757816807, peer 0)
   Parsing compound packet (total of 48 bytes)
     #1 SR (200)
       RTCP PT length: 28 bytes
     #2 SDES (202)
       RTCP PT length: 20 bytes
  End of compound packet
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 74
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (74 bytes)...
  -- Data we have now (74 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"message","body":{"request":"stop"},"transaction":"WJlO61YAQgAe"}
Forwarding request to the core (00E24BA8)
Got a Janus API request from janus.transport.http (00E24BA8)
Transport task pool, serving request
[861716883046715] There's a message for JANUS Streaming plugin
Creating plugin result...
Sending Janus API response to janus.transport.http (00E24BA8)
Got a Janus API response to send (00E24BA8)
Request completed, freeing data
Destroying plugin result...
Stopping the streaming
  -- Removing the session from the mountpoint listeners
  -- -- Found!
[861716883046715] Plugin asked to hangup PeerConnection: sending alert
[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
  >> Pushing event: 0 (Success)
janus_dtls_bio_filter_write: 00E44A60, 61
  -- 61
New list length: 1
[861716883046715] DTLS alert triggered on stream 1 (component 1), closing...
[861716883046715] Telling the plugin about it (JANUS Streaming plugin)
No WebRTC media anymore
[861716883046715] Notifying WebRTC hangup
[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
[861716883046715] >> Going to send DTLS data: 61 bytes
[861716883046715] >> >> Read 61 bytes from the write_BIO...
[861716883046715] >> >> ... and sent 61 of those bytes on the socket
[861716883046715] ICE send thread leaving...
[861716883046715] ICE thread ended!
[file-ondemand-sample] Leaving filesource (ondemand) thread
We have a message to serve...
        {
   "janus": "event",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "transaction": "WJlO61YAQgAe",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "stopping"
         }
      }
   }
}
Request completed, freeing data
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
Request completed, freeing data
Transport task pool, serving request
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
Got a HTTP GET request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP GET request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Got a Janus API request from janus.transport.http (00E24B70)
Session 5507369527856718 found... returning up to 1 messages
... handling long poll...
Transport task pool, serving request
Got a keep-alive on session 5507369527856718
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
[861716883046715] WebRTC resources freed
Got a HTTP OPTIONS request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718/861716883046715...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 47
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (47 bytes)...
  -- Data we have now (47 bytes)
Processing HTTP POST request on /janus/5507369527856718/861716883046715...
 ... parsing request...
Session: 5507369527856718
Handle: 861716883046715
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"detach","transaction":"zqpgyc5ZTw3O"}
Forwarding request to the core (00E24A90)
Got a Janus API request from janus.transport.http (00E24A90)
Transport task pool, serving request
Detaching handle from JANUS Streaming plugin
Removing streaming session...
[861716883046715] Sending event to transport...
Sending event to janus.transport.http (004BCE98)
Got a Janus API event to send (004BCE98)
[861716883046715] Handle detached (error=0), scheduling destruction
Sending Janus API response to janus.transport.http (00E24A90)
Got a Janus API response to send (00E24A90)
Request completed, freeing data
Checking 1 old Streaming sessions...
We have a message to serve...
        {
   "janus": "detached",
   "session_id": 5507369527856718,
   "sender": 861716883046715
}
Request completed, freeing data
Got a HTTP OPTIONS request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8
Request completed, freeing data
Got a HTTP POST request on /janus/5507369527856718...
 ... Just parsing headers for now...
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 48
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.30
29.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8
Processing HTTP POST request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Processing POST data (application/json) (48 bytes)...
  -- Data we have now (48 bytes)
Processing HTTP POST request on /janus/5507369527856718...
 ... parsing request...
Session: 5507369527856718
Processing POST data (application/json) (0 bytes)...
Done getting payload, we can answer
{"janus":"destroy","transaction":"eELm7PYNVmfZ"}
Forwarding request to the core (00E24B70)
Got a Janus API request from janus.transport.http (00E24B70)
Transport task pool, serving request
Session is over (5507369527856718), getting rid of the queue for the long poll
Sending Janus API response to janus.transport.http (00E24B70)
Got a Janus API response to send (00E24B70)
Request completed, freeing data
Checking 1 old HTTP/Janus sessions sessions...
Checking 1 old Streaming sessions...
Checking 1 old HTTP/Janus sessions sessions...
Checking 1 old Streaming sessions...
Checking 1 old HTTP/Janus sessions sessions...
Freeing old HTTP/Janus session
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Cleaning up handle 861716883046715...
Cleaning up session 5507369527856718...
Destroying session 5507369527856718
[861716883046715] WebRTC resources freed
[861716883046715] Handle and related resources freed
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Checking 1 old Streaming sessions...
Freeing old Streaming session
