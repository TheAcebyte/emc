BOARD = arduino:avr:uno
PORT = /dev/ttyACM0
COMPILE_FLAGS = -Isrc

all: compile

compile:
	arduino-cli compile --fqbn $(BOARD) --build-property "compiler.cpp.extra_flags=$(COMPILE_FLAGS)" .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) -t .

monitor:
	arduino-cli monitor -p $(PORT)

gen-lsp:
	arduino-cli compile --fqbn $(BOARD) --only-compilation-database --build-path build/

list:
	arduino-cli board list

clean:
	rm -rf build/
