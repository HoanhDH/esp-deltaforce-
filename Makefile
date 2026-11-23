# Build with Clang + wasm-ld, or run: make WASM_CXX='zig c++'
WASM_CXX ?= clang++
WASM_FLAGS = --target=wasm32-freestanding -std=c++17 -Oz -nostdlib -fno-exceptions -fno-rtti -Wall -Wextra
WASM_LINK = -Wl,--no-entry -Wl,--export-memory -Wl,--strip-all

# Embed the compiled module so the demo can load like a normal local script.
scene.wasm.js: scene.wasm
	python3 -c 'import base64, pathlib; data = base64.b64encode(pathlib.Path("$<").read_bytes()).decode("ascii"); print("// Generated from src/scene.cpp by make.\nconst sceneWasmBase64 = \"" + data + "\";")' > $@

scene.wasm: src/scene.cpp src/canvas.hpp src/replay.hpp
	$(WASM_CXX) $(WASM_FLAGS) $(WASM_LINK) $< -o $@

.INTERMEDIATE: scene.wasm
.DELETE_ON_ERROR:
