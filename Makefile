BOARD = arduino:avr:uno
PORT = /dev/ttyACM1

all: compile

compile:
	arduino-cli compile --fqbn $(BOARD) .

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
