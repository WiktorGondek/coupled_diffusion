CC=gcc
CFLAGS=-Wall -Werror -std=c99
LDFLAGS=-lm
TARGET=coupled_diffusion
PYTHON=python3
SCRIPT=animated_plot.py
ARGS=diffusion_data
PYTHON_FLAGS=

all: $(TARGET) run_c run_python

$(TARGET): $(TARGET).c
	$(CC) $(CLFAGS) -o $@ $^ $(LDFLAGS)

run_c:
	./$(TARGET)

run_python:
	$(PYTHON) $(SCRIPT) $(ARGS) $(PYTHON_FLAGS)

save:
ifneq (,$(filter save,$(MAKECMDGOALS)))
	$(MAKE) run_python PYTHON_FLAGS=-s
endif

clean:
	rm -f $(TARGET)
