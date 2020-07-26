package main

import (
	"fmt"
	"math"
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
			r += r/m + r/(m+1) + r/(m+2)
		}
	}
	return r
}

func testOneU64(key uint64) (float64, uint64) {
	start := time.Now()
	result := nonsenseU64(key)
	end := time.Now()
	ms := float64(end.Sub(start)/time.Nanosecond) * 1e-6
	return ms, result
}

func testU64(key uint64) {
	minMs := 1e100
	result := uint64(0)
	for i := 0; i < 5; i++ {
		ms, r := testOneU64(key)
		result = r
		minMs = math.Min(minMs, ms)
	}
	fmt.Printf("u64,%d,%f,%d\n", key, minMs, result)
}

func nonsenseF64(s0 float64) float64 {
	r := 0.0
	m := 0.0
	for i := 1.0; i < 64.0; i++ {
		m += 7
		if i < s0 {
			s := nonsenseF64(s0 - i)
			r += m / (s*s + 1.0)
		}
	}
	return r
}

func testOneF64(key float64) (float64, float64) {
	start := time.Now()
	result := nonsenseF64(key)
	end := time.Now()
	ms := float64(end.Sub(start)/time.Nanosecond) * 1e-6
	return ms, result
}

func testF64(key float64) {
	minMs := 1e100
	result := 0.0
	for i := 0; i < 5; i++ {
		ms, r := testOneF64(key)
		result = r
		minMs = math.Min(minMs, ms)
	}
	fmt.Printf("f64,%.0f,%f,%f\n", key, minMs, result)
}

func test(key uint64) {
	testU64(key)
	testF64(float64(key))
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
