.PHONY: build install-emu install-phone run-emu run-phone

build:
	pebble build

install-emu:
	pebble install --emulator basalt

install-phone:
	pebble install --phone 192.168.0.4

logs:
	pebble --debug logs

run-emu: build install-emu
run-phone: build install-phone

run-log-emu: build install-emu logs
run-log-phone: build install-phone logs
