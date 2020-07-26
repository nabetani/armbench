LEN=25

GOVER:=$(shell go version | grep -Eo 'go[0-9\.]+')
ARM64:=arm64-${GOVER}
ARM7:=arm7-${GOVER}
AMD64:=amd64-${GOVER}

ifeq ($(shell uname),Darwin)

all: ${AMD64}.csv ${ARM7} ${ARM64}

${AMD64}.csv : ${AMD64}
	./${AMD64} ${LEN} > ${AMD64}.csv

${AMD64} : main.go
	go build -o ${AMD64}

else ifeq ($(shell uname),Linux)

all: ${ARM7}.csv ${ARM64}.csv

${ARM7}.csv : ${ARM7}
	./${ARM7} ${LEN} > ${ARM7}.csv

${ARM64}.csv : ${ARM64}
	./${ARM64} ${LEN} > ${ARM64}.csv
endif

${ARM7} : main.go
	go build -o ${ARM7}

${ARM64} : main.go
	go build -o ${ARM64}

clean:
	rm -f arm7*.*
	rm -f arm64*.*
	rm -f amd64*.*
