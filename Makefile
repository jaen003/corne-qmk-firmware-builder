.PHONY: compile
compile:
	@echo "Compiling the keyboard firmware"
	@cp -R src/* /home/vscode/qmk_firmware/keyboards/crkbd/keymaps/custom
	@qmk compile -kb crkbd -km custom
	@cp /home/vscode/qmk_firmware/crkbd_rev1_custom.hex .