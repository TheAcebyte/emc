BOARD = arduino:avr:uno
PORT = /dev/ttyACM0

all: compile

compile:
	arduino-cli compile --fqbn $(BOARD) .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) -t .

gen_lsp:
	arduino-cli compile --fqbn $(BOARD) --only-compilation-database --build-path build/

monitor:
	arduino-cli monitor -p $(PORT)

clean:
	rm -rf build/
