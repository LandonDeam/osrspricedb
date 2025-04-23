# This is currently a WIP, more features will be added when I have time, as I have a busy schedule.

## Requirements
1. Conan 2.0 or higher

2. Ninja 1.12 or higher

3. MySQL Server with MySQLX plugin

## Build
Build the project: `conan build . --build=missing -o:b "!cmake/*: cmake/[>=3 <4]" -s build_type=Release`
