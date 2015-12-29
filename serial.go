package main

import (
	"fmt"
	"github.com/huin/goserial"
	"io"
	//"time"
	"bufio"
	"time"
)

type serial struct {
	serialPort io.ReadWriteCloser
}

func New() (*serial, error) {
	c := &goserial.Config{Name: "/dev/ttyACM0", Baud: 57600}
	s, err := goserial.OpenPort(c)
	if err != nil {
		return nil, err
	}
	return &serial{s}, nil
}
func (s *serial) sendCommand(command string) error {
	_, err := s.serialPort.Write([]byte(command))
	if err != nil {
		return err
	}

	return nil
}
func (s *serial) receive() ([]byte, error) {
	reader := bufio.NewReader(s.serialPort)
	var reply []byte
	var err error
	//for {
		reply, err = reader.ReadBytes('\x03') //'\x0a')
/*		if err != nil {
			fmt.Println("First err ", err)
			time.Sleep(time.Second * 1)

			reply, err = reader.ReadBytes('\x0a')
			fmt.Println("Second err ", err)
		}*/
	//}

	return reply, err
}

func main() {
	a, err := New()
	if err != nil {
		fmt.Println("Error ", err)
		return
	}
	defer a.serialPort.Close()

	err = a.sendCommand("256")
	if err!= nil {
		fmt.Println("Error receiving ", err)
		return
	}

	time.Sleep(time.Second * 5)
	data, err := a.receive()
	if err!= nil {
		fmt.Println("Error receiving ", err)
	}

	fmt.Println("Recieved ", string(data))


}
