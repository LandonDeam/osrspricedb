all: build

install:
	conan install . --build missing

build: install
	conan build .
