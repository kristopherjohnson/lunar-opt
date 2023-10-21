PYTHON=python3
PIP=pip3

all: test
.PHONY: all

# Generate graphs
run: venv
	source venv/bin/activate && $(PYTHON) src
.PHONY: graphs

# Create Python virtual environment
venv:
	$(PYTHON) -mvenv venv
	source venv/bin/activate && $(PIP) install -r requirements.txt

# Run unit tests
test: venv
	source venv/bin/activate && $(PYTHON) -m unittest
.PHONY: test

clean:
	- $(RM) -r $(OUTPUTDIR)
	- $(RM) -r $(BUILDDIR)
	- $(RM) -r src/__pycache__
.PHONY: clean
