from setuptools import setup, Extension

setup(
    name = "testplugin",
    version="0.1",
    ext_modules=[
        Extension("testplugin", sources=["testplugin.c"])
    ]
)