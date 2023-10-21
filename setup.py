"""
Generates the lunar_wrapper module so that the C code in lunar.c
can be called from Python.

Run this command to build the module (Cython must be installed first):

    python3 setup.py build_ext --inplace
"""

from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    ext_modules=cythonize([
        Extension(
            name="app.lunar_wrapper",
            sources=["lunar_wrapper.pyx"],
            include_dirs=["."]
        ),
    ]),
    packages=["app"],
)
