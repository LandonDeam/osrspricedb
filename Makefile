all: build

build: install
	conan build . --build=missing
