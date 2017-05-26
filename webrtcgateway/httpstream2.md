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
}


OPTIONS /janus/7501368377167677 HTTP/1.1
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

POST /janus/7501368377167677 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 81
Pragma: no-cache
Cache-Control: no-cache
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"attach","plugin":"janus.plugin.streaming","transaction":"eomnKm6WzLOz"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 142
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:22 GMT

{
   "janus": "success",
   "session_id": 7501368377167677,
   "transaction": "eomnKm6WzLOz",
   "data": {
      "id": 8495892224103881
   }
}

OPTIONS /janus/7501368377167677/8495892224103881 HTTP/1.1
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

POST /janus/7501368377167677/8495892224103881 HTTP/1.1
Host: 172.16.64.92:8088
Connection: keep-alive
Content-Length: 74
Pragma: no-cache
Cache-Control: no-cache
Accept: application/json, text/javascript, */*; q=0.01
Origin: http://172.16.64.92
User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Mobile Safari/537.36
Content-Type: application/json
Referer: http://172.16.64.92/streamingtest.html
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.8

{"janus":"message","body":{"request":"list"},"transaction":"leJgRPbpfPAh"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 767
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:22 GMT

{
   "janus": "success",
   "session_id": 7501368377167677,
   "sender": 8495892224103881,
   "transaction": "leJgRPbpfPAh",
   "plugindata": {
      "plugin": "janus.plugin.streaming",
      "data": {
         "streaming": "list",
         "list": [
            {
               "id": 1,
               "description": "h264 live stream coming from gstreamer",
               "type": "live",
               "video_age_ms": 8163
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

POST /janus/7501368377167677/8495892224103881 HTTP/1.1
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

{"janus":"message","body":{"request":"watch","id":3},"transaction":"RWgETkNPzyrP"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 90
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:25 GMT

{
   "janus": "ack",
   "session_id": 7501368377167677,
   "transaction": "RWgETkNPzyrP"
}OPTIONS /janus/7501368377167677/8495892224103881 HTTP/1.1
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
Date: Fri, 26 May 2017 10:25:53 GMT

POST /janus/7501368377167677/8495892224103881 HTTP/1.1
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

{"janus":"detach","transaction":"P1LI9Koi4Ry7"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 94
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:54 GMT

{
   "janus": "success",
   "session_id": 7501368377167677,
   "transaction": "P1LI9Koi4Ry7"
}OPTIONS /janus/7501368377167677 HTTP/1.1
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
Date: Fri, 26 May 2017 10:25:54 GMT

POST /janus/7501368377167677 HTTP/1.1
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

{"janus":"destroy","transaction":"doGcfk47TC5m"}HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Length: 94
Access-Control-Allow-Origin: *
Content-Type: application/json
Date: Fri, 26 May 2017 10:25:54 GMT

{
   "janus": "success",
   "session_id": 7501368377167677,
   "transaction": "doGcfk47TC5m"
}