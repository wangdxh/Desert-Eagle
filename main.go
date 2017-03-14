package main

import (
	"fmt"
	"golang.org/x/net/websocket"

	"bufio"
	"bytes"
	"encoding/binary"
	"io"
	"log"
	"net"
	"net/http"
)

var g_websocket *websocket.Conn

/////////////////////////////////////////////////////////
// think how to close the hub connection and a close tag to break

type connection struct {
	ws   *websocket.Conn
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
	fmt.Println("ws socket reader close")
	c.ws.Close()
}

func (c *connection) writer() {
	tagheader := make([]byte, 16)
	var dwtime uint32 = 0
	var firstcomin bool = true
	for message := range c.send {
		if true == firstcomin {
			firstcomin = false
			err := websocket.Message.Send(c.ws, message)
			if err != nil {
				break
			}
			// write file
		} else {
			tagheader[0] = 9
			// 1 2 3 big taglen = len(message)
			taglen := len(message)
			tagheader[1] = (byte)((taglen >> 16) & 0xff)
			tagheader[2] = (byte)((taglen >> 8) & 0xff)
			tagheader[3] = (byte)(taglen & 0xff)

			tagheader[4] = (byte)((dwtime >> 16) & 0xff)
			tagheader[5] = (byte)((dwtime >> 8) & 0xff)
			tagheader[6] = (byte)(dwtime & 0xff)
			tagheader[7] = (byte)((dwtime >> 24) & 0xff)

			iskey := message[0]
			tagheader[11] = 0x27
			if 1 == iskey {
				tagheader[11] = 0x17
			}
			tagheader[12] = 1

			err := websocket.Message.Send(c.ws, tagheader)
			if err != nil {
				break
			}
			// first send header then send message
			err = websocket.Message.Send(c.ws, message[1:])
			if err != nil {
				break
			}
			dwtime += 30
		}

	}
	fmt.Println("ws socket write close")
	c.ws.Close()
}

func wsflvlive(ws *websocket.Conn) {
	//fmt.Println("new web client")
	deviceid := ws.Request().FormValue("deviceid")
	fmt.Println("device id %s", deviceid)
	h, _ := g_mapHub[deviceid]
	/*if !exists {
		fmt.Println("hub not exists")
		h = CreateHub()
		g_mapHub[deviceid] = h
		h.run()
	}*/
	c := &connection{send: make(chan []byte, 3), ws: ws}
	h.register <- c
	defer func() { h.unregister <- c }()
	go c.writer()
	c.reader()
}

type hub struct {
	connections map[*connection]bool
	broadcast   chan []byte
	register    chan *connection
	unregister  chan *connection
	exit        chan struct{}
	flvheader   *[]byte
}

var g_mapHub map[string]*hub

func CreateHub(flvhead *[]byte) *hub {
	return &hub{
		broadcast:   make(chan []byte),
		register:    make(chan *connection),
		unregister:  make(chan *connection),
		connections: make(map[*connection]bool),
		exit:        make(chan struct{}),
		flvheader:   flvhead,
	}
}

func (h *hub) run() {
	var bexit bool = false
	for {
		select {
		case c := <-h.register:
			h.connections[c] = true
			//send flvheader to client
			c.send <- *(h.flvheader)
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
					/*delete(h.connections, c)
					  close(c.send)*/
					fmt.Println("send to soment client not that fast")
				}
			}
		case <-h.exit:
			fmt.Println("hub exit")
			bexit = true
			break
		}
		if bexit {
			break
		}
	}
	fmt.Println("hub exit2")
}
func (h *hub) close() {
	close(h.exit)
	for c := range h.connections {
		delete(h.connections, c)
		close(c.send)
	}
}

func readOnceBytes(c net.Conn) ([]byte, error) {
	var bufFrame []byte
	var err error = nil
	for {
		lenbuf := make([]byte, 4)
		_, err = io.ReadFull(c, lenbuf)
		if err != nil {
			break
		}
		b_buf := bytes.NewBuffer(lenbuf)
		var lenreal int32
		binary.Read(b_buf, binary.LittleEndian, &lenreal)

		bufFrame = make([]byte, lenreal)
		_, err = io.ReadFull(c, bufFrame)
		break
	}
	return bufFrame, err
}

func handleConn(c net.Conn) {
	defer c.Close()
	log.Println("new tcp conn")

	bufdeviceid, err := readOnceBytes(c)
	if err != nil {
		return
	}
	strdeviceid := string(bufdeviceid[:])
	fmt.Println("tcp socket url is %s", strdeviceid)

	_, exists := g_mapHub[strdeviceid]
	if exists {
		fmt.Println("name is in just return")
		return
	}

	flvheader, err := readOnceBytes(c)
	if err != nil {
		return
	}

	var h *hub = CreateHub(&flvheader)
	g_mapHub[strdeviceid] = h
	go h.run()
	bufFrame := make([]byte, 1*1024*1024)
	lenbuf := make([]byte, 4)

	for {
		_, err := io.ReadFull(c, lenbuf)
		if err != nil {
			log.Println(err)
			break
		}

		b_buf := bytes.NewBuffer(lenbuf)
		var lenreal int32
		binary.Read(b_buf, binary.LittleEndian, &lenreal)

		//fmt.Printf("buf len is %d\n", lenreal)

		_, err = io.ReadFull(c, bufFrame[:lenreal])
		if err != nil {
			log.Println(err)
			break
		}

		h.broadcast <- bufFrame[:lenreal]
	}
	h.close()
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
	g_mapHub = make(map[string]*hub)
	go localTcp()

	http.Handle("/wsflvlive", websocket.Handler(wsflvlive))

	http.Handle("/", http.FileServer(http.Dir("./public")))

	err := http.ListenAndServe(":80", nil)

	if err != nil {
		panic("ListenAndServe: " + err.Error())
	}
}
