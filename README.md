# This is currently a WIP, more features will be added when I have time, as I have a busy schedule

## Requirements

1. Conan 2.0 or higher

2. Ninja 1.12 or higher

3. MySQL Server with MySQLX plugin (provided by default in Oracle's MySQL server as of version 5.7)

4. (Optional) Doxygen

## Build

Build the project: `conan build . --build=missing -o:b "!cmake/*: cmake/[>=3 <4]" -s build_type=Release`

(Note: If Conan has fixed their issue with CMake 4 recipes, you should be able to omit the `-o:b "!cmake/*: cmake/[>=3 <4]"` portion)

## Running

The following environment variables (preferably in a `.env` file) are required to connect to your MySQL server:

```.env
MYSQL_USERNAME=username
MYSQL_PASSWORD=password
MYSQL_PORT=33060
MYSQL_HOST=localhost
```
