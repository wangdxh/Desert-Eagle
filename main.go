package main

import (
	"fmt"
	"golang.org/x/net/websocket"

	"bufio"
	"bytes"
	"encoding/binary"
	"html/template"
	"io"
	"log"
	"net"
	"net/http"
	"sort"
	"time"
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
			// just keyframe
			if 1 != message[0] {
				continue
			}
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
	fmt.Println("ws device id %s", deviceid)
	h, exist := g_mapHub[deviceid]
	if false == exist {
		fmt.Println("in ws hub not exists %d", exist)
		h = CreateHub(deviceid)
		go h.run()
		g_mapHub[deviceid] = h
	}

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
	url         string
}

var g_mapHub map[string]*hub

func CreateHub(strurl string) *hub {
	return &hub{
		broadcast:   make(chan []byte),
		register:    make(chan *connection),
		unregister:  make(chan *connection),
		connections: make(map[*connection]bool),
		exit:        make(chan struct{}),
		flvheader:   nil,
		url:         strurl,
	}
}

func (h *hub) run() {
	var bexit bool = false
	for {
		select {
		case c := <-h.register:
			h.connections[c] = true
			//send flvheader to client
			if h.flvheader != nil {
				c.send <- *(h.flvheader)
			}
		case c := <-h.unregister:
			if _, ok := h.connections[c]; ok {
				delete(h.connections, c)
				close(c.send)
			}
			if 0 == len(h.connections) && nil == h.flvheader {
				h.close()
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
	delete(g_mapHub, h.url)
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

	h, exists := g_mapHub[strdeviceid]
	if exists {
		if h.flvheader != nil {
			fmt.Println("are u kidding me ??")
			return
		}
	} else {
		h = CreateHub(strdeviceid)
		go h.run()
		g_mapHub[strdeviceid] = h
	}

	flvheader, err := readOnceBytes(c)
	if err != nil {
		return
	}

	h.flvheader = &flvheader
	h.broadcast <- flvheader

	//bufFrame := make([]byte, 1*1024*1024)
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
		buftemp := make([]byte, lenreal)
		//_, err = io.ReadFull(c, bufFrame[:lenreal])
		_, err = io.ReadFull(c, buftemp)
		if err != nil {
			log.Println(err)
			break
		}

		h.broadcast <- buftemp
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

func HandleRoot(w http.ResponseWriter, r *http.Request) {
	fmt.Println(r.URL.RawPath, r.URL.Path)

	t, err := template.New("urllist.html").ParseFiles("./public/urllist.html")
	if err != nil {
		fmt.Println(err)
	}

	data := struct {
		Title string
		Items []string
	}{
		Title: "My page",
	}

	sorted_keys := make([]string, 0)
	for k, _ := range g_mapHub {
		sorted_keys = append(sorted_keys, k)
	}
	sort.Strings(sorted_keys)
	data.Items = sorted_keys

	err = t.Execute(w, data)
	if err != nil {
		fmt.Println(err)
	}
	//w.Write([]byte("this is good"))

}
func HandleRealplay(w http.ResponseWriter, r *http.Request) {
	strdeviceid := r.FormValue("deviceid")
	if 0 == len(strdeviceid) {
		w.Write([]byte("bad deviceid"))
		return
	}
	fmt.Println(strdeviceid)
	data := struct {
		Url string
	}{
		Url: strdeviceid,
	}
	t, err := template.New("flv.html").ParseFiles("./public/flv.html")
	if err != nil {
		fmt.Println(err)
	}
	err = t.Execute(w, data)
	if err != nil {
		fmt.Println(err)
	}

}

var input chan int

func test2() {
	time.Sleep(time.Second * 5)
	for m := range input {
		fmt.Println(m)
	}
	fmt.Println(100)
}
func test() {
	input = make(chan int, 5)
	go test2()
	input <- 1
	input <- 2

	fmt.Println(99)
	input <- 3
	input <- 4
	input <- 5
	close(input)
	select {
	case input <- 6:
		{
			fmt.Println("ok")
		}
	default:
		{
			fmt.Println("no")
		}
	}
	//input <- 7

	fmt.Println(101)
	/*for {

		select {
		case c := <-input:
			{
				fmt.Println(c)
			}

		}
	}*/
}

///////////////////////////////////////////////////////

func main() {

	g_mapHub = make(map[string]*hub)
	go localTcp()

	http.Handle("/wsflvlive", websocket.Handler(wsflvlive))

	http.Handle("/public/", http.StripPrefix("/public/", http.FileServer(http.Dir("./public"))))
	http.HandleFunc("/", HandleRoot)
	http.HandleFunc("/realplay", HandleRealplay)

	err := http.ListenAndServe(":1980", nil)

	if err != nil {
		panic("ListenAndServe: " + err.Error())
	}
}
