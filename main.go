package main

import (
	"fmt"
	"golang.org/x/net/websocket"
	
	"log"
	"net/http"
	"net"
	"io"
	"os"
	"time"
	"bufio"
	"encoding/binary"
	"bytes"
	"io/ioutil"
)

var g_websocket *websocket.Conn ;


/////////////////////////////////////////////////////////

type connection struct {
    ws *websocket.Conn
    send chan []byte
}

func (c *connection) reader() {
   r := bufio.NewReader(c.ws)
   for {
    	_, err := r.ReadBytes('\n')
    	if err != nil {
        	break
    	}
    }    
    c.ws.Close()
}

func (c *connection) writer() {
    for message := range c.send {
        err := websocket.Message.Send(c.ws, message)
        if err != nil {
            break
        }
    }
    c.ws.Close()
}


func wsflvlive(ws *websocket.Conn) {

    c := &connection{send: make(chan []byte, 3), ws: ws}
    h.register <- c
    defer func() { h.unregister <- c }()
    go c.writer()
    c.reader()
}

type hub struct {
    connections map[*connection]bool

    broadcast chan []byte

    register chan *connection

    unregister chan *connection
}

var h = hub{
    broadcast:   make(chan []byte),
    register:    make(chan *connection),
    unregister:  make(chan *connection),
    connections: make(map[*connection]bool),
}

func (h *hub) run() {
    for {
        select {
        case c := <-h.register:
            h.connections[c] = true
        case c := <-h.unregister:
            if _, ok := h.connections[c]; ok {
                delete(h.connections, c)
                close(c.send)
            }
        case m := <-h.broadcast:
            for c := range h.connections {
                select {
                case c.send <- m:
                default:
                    delete(h.connections, c)
                    close(c.send)
                }
            }
        }
    }
}





func handleConn(c net.Conn) {
    defer c.Close()
    log.Println("new tcp conn")
    
    bufFrame := make([]byte, 1*1024*1024);
	  lenbuf := make([]byte, 4);
	    
    for {
	    
	    
	    _, err := io.ReadFull(c, lenbuf);
	    if err != nil {
	    	log.Println(err)
	    	break
	    }
	    
	    b_buf := bytes.NewBuffer(lenbuf)
	    var lenreal int32
	    binary.Read(b_buf, binary.LittleEndian, &lenreal)
	    
	    fmt.Printf("buf len is %d\n", lenreal)
	    
	    _, err = io.ReadFull(c, bufFrame[:lenreal]);
	    if err != nil {
	    	log.Println(err)
	    	break;
	    }
	    
	    h.broadcast <- bufFrame[:lenreal]
	    
	    // write to the websocket
	    /*if nil != g_websocket {
	    	err := websocket.Message.Send(g_websocket, bufFrame[:lenreal])
				if err != nil {
				   log.Println(err)
				   //break
				}
	    }*/
	    
    }
    
}

func localTcp() {
	 l, err := net.Listen("tcp", ":1984")
    if err != nil {
        fmt.Println("listen error:", err)
        return
    }

    for {
        c, err := l.Accept()
        if err != nil {
            fmt.Println("accept error:", err)
            break
        }
        // start a new goroutine to handle
        // the new connection.
        go handleConn(c)
    }
	
}



///////////////////////////////////////////////////////





func main() {

	go localTcp()
	go h.run()
		
	http.Handle("/wsflvlive", websocket.Handler(wsflvlive))
	
	http.Handle("/", http.FileServer(http.Dir("./public")))

	err := http.ListenAndServe(":80", nil)

	if err != nil {
		panic("ListenAndServe: " + err.Error())
	}
}
