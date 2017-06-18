# Parallel Gaussian Blurring

A C program that takes a .ppm image and applies a gaussian blur.

#### To compile:
`make compile`

#### To clean up object files and executables:
`make clean`



### Example Usage:

To run this program, you can either specify the arguements in the command lline when the program is running when each query is printed to the terminal, or you can include it as a command line arguement.

`./blur`

`Original image name: visit0000.ppm`

`Result image name: out.ppm`

`Radius: 1`

`Sigma: 1`

OR 

`./blur visit0000.ppm out.ppm 1 1`

## Exercise:
#### Port as much of the code as your team can can to GPUs using OpenACC, and CUDA.
