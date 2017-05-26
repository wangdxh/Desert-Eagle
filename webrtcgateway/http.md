OPTIONS /janus HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache
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
Date: Fri, 26 May 2017 10:25:22 GMT

POST /janus HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 47
Pragma: no-cache
Cache-Control: no-cache
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"create","transaction":"Ehasi7C83Y5r"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 107
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:22 GMT

{
   "janus": "success",
   "transaction": "Ehasi7C83Y5r",
   "data": {
      "id": 7501368377167677
   }
}GET /janus/7501368377167677?rid=1495794322420&maxev=1&_=1495794322420 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8

HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 1154
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:25 GMT

{
   "janus": "event",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "transaction": "RWgETkNPzyrP",
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
      "sdp": "v=0\r\no=- 1495794325655221 1495794325655221 IN IP4 172.16.64.92\r\ns=Streaming Test\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS janus\r\nm=audio 1 RTP/SAVPF 0\r\nc=IN IP4 172.16.64.92\r\na=mid:audio\r\na=sendonly\r\na=rtcp-mux\r\na=ice-ufrag:0ysH\r\na=ice-pwd:vA+q4BN1UFIOvnTUDYelDt\r\na=ice-options:trickle\r\na=fingerprint:sha-256 D2:B9:31:8F:DF:24:D8:0E:ED:D2:EF:25:9E:AF:6F:B8:34:AE:53:9C:E6:F3:8F:F2:64:15:FA:E8:7F:53:2D:38\r\na=setup:actpass\r\na=connection:new\r\na=rtpmap:0 PCMU/8000\r\na=ssrc:-1056430916 cname:janusaudio\r\na=ssrc:-1056430916 msid:janus janusa0\r\na=ssrc:-1056430916 mslabel:janus\r\na=ssrc:-1056430916 label:janusa0\r\na=candidate:1 1 udp 2013266431 172.16.64.92 56453 typ host\r\na=candidate:1 2 udp 2013266430 172.16.64.92 56454 typ host\r\n"
   }
}POST /janus/7501368377167677/8495892224103881 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 561
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"start"},"transaction":"d7J4sx3wn5qK","jsep":{"type":"answer","sdp":"v=0\r\no=- 709022342679883081 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS\r\nm=audio 9 RTP/SAVPF 0\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9 IN IP4 0.0.0.0\r\na=ice-ufrag:ic3F\r\na=ice-pwd:kvlFu+ABWDu/2MXoJXnIHJ1w\r\na=fingerprint:sha-256 D7:B9:54:91:AF:C2:02:A6:26:16:85:A1:8A:83:7B:8D:08:F4:74:AA:D3:82:EA:5A:AA:DC:A0:E2:BE:C2:FD:17\r\na=setup:active\r\na=mid:audio\r\na=recvonly\r\na=rtcp-mux\r\na=rtpmap:0 PCMU/8000\r\n"}}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:25 GMT

{
   "janus": "ack",
   "session_id": 7501368377167677,
   "transaction": "d7J4sx3wn5qK"
}POST /janus/7501368377167677/8495892224103881 HTTP/1.1
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

{"janus":"trickle","candidate":{"candidate":"candidate:2153010912 1 udp 2113937151 172.16.64.92 56457 typ host generation 0 ufrag ic3F network-cost 50","sdpMid":"audio","sdpMLineIndex":0},"transaction":"BKmuIWWRbuEx"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:25 GMT

{
   "janus": "ack",
   "session_id": 7501368377167677,
   "transaction": "BKmuIWWRbuEx"
}GET /janus/7501368377167677?rid=1495794325962&maxev=1&_=1495794325962 HTTP/1.1
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
Content-Length: 310
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:25 GMT

{
   "janus": "event",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "transaction": "d7J4sx3wn5qK",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "starting"
         }
      }
   }
}POST /janus/7501368377167677/8495892224103881 HTTP/1.1
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

{"janus":"trickle","candidate":{"completed":true},"transaction":"XYjQnsPSNghc"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:26 GMT

{
   "janus": "ack",
   "session_id": 7501368377167677,
   "transaction": "XYjQnsPSNghc"
}GET /janus/7501368377167677?rid=1495794326169&maxev=1&_=1495794326169 HTTP/1.1
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
Content-Length: 275
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:26 GMT

{
   "janus": "event",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "started"
         }
      }
   }
}GET /janus/7501368377167677?rid=1495794326475&maxev=1&_=1495794326475 HTTP/1.1
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
Content-Length: 92
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:26 GMT

{
   "janus": "webrtcup",
   "session_id": 7501368377167677,
   "sender": 8495892224103881
}GET /janus/7501368377167677?rid=1495794326681&maxev=1&_=1495794326681 HTTP/1.1
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
Content-Length: 117
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:53 GMT

{
   "janus": "hangup",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "reason": "DTLS alert"
}POST /janus/7501368377167677/8495892224103881 HTTP/1.1
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

{"janus":"message","body":{"request":"stop"},"transaction":"LQsYTBKyAtBv"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:53 GMT

{
   "janus": "ack",
   "session_id": 7501368377167677,
   "transaction": "LQsYTBKyAtBv"
}GET /janus/7501368377167677?rid=1495794353632&maxev=1&_=1495794353632 HTTP/1.1
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
Content-Length: 276
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:53 GMT

{
   "janus": "event",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "event",
         "result": {
            "status": "stopping"
         }
      }
   }
}GET /janus/7501368377167677?rid=1495794353840&maxev=1&_=1495794353840 HTTP/1.1
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
Content-Length: 27
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:54 GMT

