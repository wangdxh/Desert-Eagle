OPTIONS http://172.16.64.92:8088/janus HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:45 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 47
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"create","transaction":"mzLbAMkH15IM"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 107
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:45 GMT

{
   "janus": "success",
   "transaction": "mzLbAMkH15IM",
   "data": {
      "id": 5507369527856718
   }
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799805512&maxev=1&_=1495799805512 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 1149
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

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
      "sdp": "v=0\r\no=- 1495799811084970 1495799811084970 IN IP4 172.16.64.92\r\ns=Streaming Test\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS janus\r\nm=audio 1 RTP/SAVPF 0\r\nc=IN IP4 172.16.64.92\r\na=mid:audio\r\na=sendonly\r\na=rtcp-mux\r\na=ice-ufrag:KbYF\r\na=ice-pwd:UibYJJTuYcd7F/wmOseax6\r\na=ice-options:trickle\r\na=fingerprint:sha-256 D2:B9:31:8F:DF:24:D8:0E:ED:D2:EF:25:9E:AF:6F:B8:34:AE:53:9C:E6:F3:8F:F2:64:15:FA:E8:7F:53:2D:38\r\na=setup:actpass\r\na=connection:new\r\na=rtpmap:0 PCMU/8000\r\na=ssrc:-537150489 cname:janusaudio\r\na=ssrc:-537150489 msid:janus janusa0\r\na=ssrc:-537150489 mslabel:janus\r\na=ssrc:-537150489 label:janusa0\r\na=candidate:1 1 udp 2013266431 172.16.64.92 58724 typ host\r\na=candidate:1 2 udp 2013266430 172.16.64.92 58725 typ host\r\n"
   }
}

------------------------------------------------------------------

OPTIONS http://172.16.64.92:8088/janus/5507369527856718 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:45 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 81
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"attach","plugin":"janus.plugin.streaming","transaction":"MAm860DVlVTZ"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 141
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:45 GMT

{
   "janus": "success",
   "session_id": 5507369527856718,
   "transaction": "MAm860DVlVTZ",
   "data": {
      "id": 861716883046715
   }
}

------------------------------------------------------------------

OPTIONS http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:45 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 74
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"list"},"transaction":"ZuIyyBGpgzdM"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 769
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:45 GMT

{
   "janus": "success",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "transaction": "ZuIyyBGpgzdM",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "list",
         "list": [
            {
               "id": 1,
               "description": "h264 live stream coming from gstreamer",
               "type": "live",
               "video_age_ms": 5491245
            },
            {
               "id": 2,
               "description": "a-law file source (radio broadcast)",
               "type": "live"
            },
            {
               "id": 3,
               "description": "mu-law file source (music)",
               "type": "on demand"
            }
         ]
      }
   }
}

------------------------------------------------------------------

GET http://172.16.64.92/fonts/fontawesome-webfont.woff2?v=4.6.2 HTTP/1.1
Host: 172.16.64.92
Connection: keep-alive
Cache-Control: max-age=0
Origin: http://172.16.64.92
If-Modified-Since: Tue, 15 Nov 2016 10:02:04 GMT
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Accept: */*
Referer: http://172.16.64.92/css/font-awesome.css
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.0 200 OK
Server: SimpleHTTP/0.6 Python/2.7.12
Date: Fri, 26 May 2017 11:56:45 GMT
Content-type: application/octet-stream
Content-Length: 71760
Last-Modified: Tue, 15 Nov 2016 10:02:04 GMT

------------------------------------------------------------------

OPTIONS http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:51 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 82
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"watch","id":3},"transaction":"pNcztAvNzy4K"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "ack",
   "session_id": 5507369527856718,
   "transaction": "pNcztAvNzy4K"
}

------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 562
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"start"},"transaction":"yc7TZDFL3nJ0","jsep":{"type":"answer","sdp":"v=0\r\no=- 5358450128380337511 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS\r\nm=audio 9 RTP/SAVPF 0\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9 IN IP4 0.0.0.0\r\na=ice-ufrag:PeSQ\r\na=ice-pwd:EsKyz4fKNx4tvlsbx1uM1UQ/\r\na=fingerprint:sha-256 3F:66:59:88:8A:01:60:F3:17:23:F7:DA:7B:C9:DA:A4:37:A8:4F:B8:64:BC:1A:2D:AF:0E:DC:BE:E4:E7:19:8C\r\na=setup:active\r\na=mid:audio\r\na=recvonly\r\na=rtcp-mux\r\na=rtpmap:0 PCMU/8000\r\n"}}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "ack",
   "session_id": 5507369527856718,
   "transaction": "yc7TZDFL3nJ0"
}

------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 217
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"trickle","candidate":{"candidate":"candidate:2153010912 1 udp 2113937151 172.16.64.92 58726 typ host generation 0 ufrag PeSQ network-cost 50","sdpMid":"audio","sdpMLineIndex":0},"transaction":"guKGT3pjXuXP"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "ack",
   "session_id": 5507369527856718,
   "transaction": "guKGT3pjXuXP"
}

------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 79
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"trickle","candidate":{"completed":true},"transaction":"jxVt80dHA2G6"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "ack",
   "session_id": 5507369527856718,
   "transaction": "jxVt80dHA2G6"
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799811379&maxev=1&_=1495799811379 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 309
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "event",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "transaction": "yc7TZDFL3nJ0",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "starting"
         }
      }
   }
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799811589&maxev=1&_=1495799811590 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 274
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "event",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "started"
         }
      }
   }
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799811802&maxev=1&_=1495799811802 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 91
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:51 GMT

{
   "janus": "webrtcup",
   "session_id": 5507369527856718,
   "sender": 861716883046715
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799812017&maxev=1&_=1495799812017 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 309
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:56 GMT

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

------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 74
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"stop"},"transaction":"WJlO61YAQgAe"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:55 GMT

{
   "janus": "ack",
   "session_id": 5507369527856718,
   "transaction": "WJlO61YAQgAe"
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799816230&maxev=1&_=1495799816230 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 116
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:56 GMT

{
   "janus": "hangup",
   "session_id": 5507369527856718,
   "sender": 861716883046715,
   "reason": "DTLS alert"
}

------------------------------------------------------------------

GET http://172.16.64.92:8088/janus/5507369527856718?rid=1495799816447&maxev=1&_=1495799816448 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 91
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:57 GMT

{
   "janus": "detached",
   "session_id": 5507369527856718,
   "sender": 861716883046715
}

------------------------------------------------------------------

OPTIONS http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:57 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718/861716883046715 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 47
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"detach","transaction":"zqpgyc5ZTw3O"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 94
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:57 GMT

{
   "janus": "success",
   "session_id": 5507369527856718,
   "transaction": "zqpgyc5ZTw3O"
}

------------------------------------------------------------------

OPTIONS http://172.16.64.92:8088/janus/5507369527856718 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Access-Control-Request-Method: POST
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Access-Control-Request-Headers: content-type
Accept: */*
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 0
Access-Control-Allow-Headers: content-type
Access-Control-Allow-Methods: POST
Access-Control-Allow-Origin: *
Date: Fri, 26 May 2017 11:56:57 GMT



------------------------------------------------------------------

POST http://172.16.64.92:8088/janus/5507369527856718 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 48
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"destroy","transaction":"eELm7PYNVmfZ"}
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 94
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 11:56:57 GMT

{
   "janus": "success",
   "session_id": 5507369527856718,
   "transaction": "eELm7PYNVmfZ"
}

------------------------------------------------------------------

