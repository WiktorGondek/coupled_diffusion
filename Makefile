CC=gcc
CFLAGS=-Wall -Werror -std=c99
LDFLAGS=-lm
TARGET=coupled_diffusion
PYTHON=python3
SCRIPT=animated_plot.py
ARGS=diffusion_data

all: $(TARGET) run_c run_python

$(TARGET): $(TARGET).c
	$(CC) $(CLFAGS) -o $@ $^ $(LDFLAGS)

run_c:
	./$(TARGET)

run_python:
	$(PYTHON) $(SCRIPT) $(ARGS) $(PYTHON_FLAGS)

save: PYTHON_FLAGS=-s
save: run_python

clean:
	rm -f $(TARGET)
