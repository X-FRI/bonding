install:
	xmake f -m release && xmake build
	sudo cp build/linux/x86_64/release/bonding /usr/bin

uninstall:
	sudo rm /usr/bin/bonding

build.release:
	xmake f -m release && xmake build

build.debug:
	xmake f -m release && xmake build

debug:
	sudo lldb .././build/linux/x86_64/debug/bonding run

fmt:
	find src -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' | xargs clang-format -i