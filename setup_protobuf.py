from setuptools import setup, Extension

protobuf_c_include_dir = "/opt/homebrew/Cellar/protobuf-c/1.5.2/include"
protobuf_c_lib_dir = "/opt/homebrew/Cellar/protobuf-c/1.5.2/lib"


setup(
    name="c_protobuf",
    ext_modules=[
        Extension(
            "c_protobuf",
            sources=["c_protobuf.c", "appsinstalled.pb-c.c"],
            libraries=["protobuf-c"],
            include_dirs=[protobuf_c_include_dir],
            library_dirs=[protobuf_c_lib_dir],
            runtime_library_dirs=[protobuf_c_lib_dir],
        )
    ]
)