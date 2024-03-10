
# Numerically solving coupled diffusion equations with finite difference methods

This is work I did during my university studies. The work aims to use a time-splitting approach to solving the following set of coupled diffusion equations with periodic boundary conditons:


$$ \frac{\partial u}{\partial t} - D\frac{\partial^2 u}{\partial x^2} - v = 0 $$

$$ \frac{\partial v}{\partial t} - D\frac{\partial^2 v}{\partial x^2} + u = 0 $$


Where:
- $u$ and $v$ are dependent variables,
- $t$ is time,
- $x$ is the spatial coordinate,
- $D$ is the diffusion coefficient.

## Setup

To install the required python packages:
```
pip install -r requirements.txt
```

The `input.txt` file contains the diffusion coefficent $D$, domain length $L$, number of grid points $nx$ and the final simulation time $t_{F}$, respectively.

The initial conditions, $u(x,0), v(x,0), are free to be set on lines 75,76 in `coupled_diffusion.c`.

## Example

To compile and execute `coupled_diffusion.c`:
```
gcc -Wall -Werror -std=c99 -o coupled_diffusion coupled_diffusion.c -lm
./coupled_diffsion
```
This will create a file titled `diffusion_data` which contains numerical solutions to the coupled equations and, with columns for the time step, $x$ position, $u(x,t)$, $v(x,t)$, respectively. 
 
To run the animation plotting script:
```
./animated_plot.py diffusion_data
```
To save the animation, can also add the `-s` flag:
```
./animated_plot.py diffusion_data -s
```

Alternatively, the Makefile can also compile and execute the C script and run the plotting script:
```
make
```

To save the animation:
```
make save
```


 


