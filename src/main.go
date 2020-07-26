package main

import (
	"fmt"
	"math/bits"
	"os"
	"strconv"
	"time"
)

func nonsenseU64(s0 uint64) uint64 {
	r := uint64(0)
	m := uint64(0)
	for i := uint64(1); i < 64; i++ {
		m += 7
		if i < s0 {
			s := 1 + nonsenseU64(s0-i)
			r += m * bits.RotateLeft64(s, int(i))
		}
	}
	return r

}

func test(key uint64) {
	start := time.Now()
	result := nonsenseU64(key)
	end := time.Now()
	ms := float64(end.Sub(start)/time.Nanosecond) * 1e-6
	fmt.Printf("%d,%f,%d\n", key, ms, result)
}

func main() {
	key, err := strconv.ParseUint(os.Args[1], 10, 64)
	if err != nil {
		panic(err)
	}
	for i := uint64(1); i < key; i++ {
		test(i)
	}
}
