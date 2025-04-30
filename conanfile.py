from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class osrs_price_dbRecipe(ConanFile):
    name = "osrs_price_db"
    version = "0.1"
    package_type = "application"

    # Optional metadata
    license = "NA"
    author = "Landon Deam"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of osrs_price_db package here>"
    topics = ()

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def layout(self):
        cmake_layout(self)
        self.folders.build = "build"

    def requirements(self):
        self.requires("mysql-connector-cpp/9.2.0")
        self.requires("rapidjson/1.1.0")
        self.requires("poco/1.13.3")
        self.requires("doxygen/1.13.2")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja")
        tc.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
