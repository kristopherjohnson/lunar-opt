PYTHON=python3
PIP=pip3

all: test
.PHONY: all

# Generate graphs
run: venv
	source venv/bin/activate && python src
.PHONY: graphs

# Create Python virtual environment
venv:
	$(PYTHON) -mvenv venv
	source venv/bin/activate && pip install -r requirements.txt

# Create the Cython wrapper for lunar.c
#
# This will create lunar_wrapper.c, the build directory, and an
# output library in the app directory with a name like
# lunar_wrapper.cpython-311-darwin.so.
#
# The lunar_wrapper.built file is empty, but its timestamp will be used
# to determine when dependencies change.
lunar_wrapper.built: lunar.c lunar.h lunar_wrapper.pyx
	source venv/bin/activate && python setup.py build_ext --inplace
	touch lunar_wrapper.built

# Run unit tests
test: venv lunar_wrapper.built
	source venv/bin/activate && python -m unittest
.PHONY: test

clean:
	- $(RM) -r build
	- $(RM) -r app/__pycache__
	- $(RM) -r lunar_wrapper.built
	- $(RM) -r lunar_wrapper.c
	- $(RM) -r app/*.so
	- $(RM) -r venv
.PHONY: clean
