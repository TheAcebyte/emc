BOARD = arduino:avr:uno
PORT = /dev/ttyACM0

CPP_FLAGS = -Isrc
COMPILATION_FLAGS = --build-property "compiler.cpp.extra_flags=$(CPP_FLAGS)"

all: compile

compile:
	arduino-cli compile --fqbn $(BOARD) $(COMPILATION_FLAGS) .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) -t .

monitor:
	arduino-cli monitor -p $(PORT)

gen-lsp:
	arduino-cli compile --fqbn $(BOARD) $(COMPILATION_FLAGS) --only-compilation-database --build-path build/

list:
	arduino-cli board list

clean:
	rm -rf build/
