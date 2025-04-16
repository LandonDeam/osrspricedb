# This is currently a WIP, more features will be added when I have time, as I have a busy schedule.

## Requirements
Conan 2.0 or higher
MySQL Server with MySQLX plugin

## Build
Install Conan packages and setup project: `conan install . --build=missing -o:b "!cmake/*: cmake/[>=3 <4]" -s build_type=Release`
Build the project: `conan build . --build=missing -o:b "!cmake/*: cmake/[>=3 <4]" -s build_type=Release`
