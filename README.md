# NeuronNetwork

## Overview

This project aims to model a neuron network by simulating connections and interactions between neurons. 
A network is made of a set of neurons, which create random connections. 
The program allows to create different types of neurons, which have differents and fixed cellular parameters. There are five types of neurons, distributed into two categories:
* inhibitory neurons: FS and LTS
* excitatory neurons: RS, CH and IB

Moreover, each neuron has differents properties: 
* The membrane potential
* The recovery time
* The synaptic current
Each of these three properties evolves with time.

## Goal of the program

The final aim of this program is to spot **firing** neurons. 
A neuron is **firing** when its membrane potential is more than the discharge threshold, which is 30 mV. 
At each step of the simulation, the program will make the neurone evolve and check which neurons are firing. 
The output file will contain the list of firing neurons at each time step.

## Requirements 

In order to use this program, you need to have these packages:
* CMake
* Doxygen 
* TCLAP

Moreover, the `RasterPlots.R` program will be usefull to transform the results of this program into graphical representations.

## Build and run 

### Step 1: Clone the repository

In order to use the program, you first have to clone the source code to your computer.
```
git clone https://gitlab.epfl.ch/sv_cpp_projects/team_21.git
cd team_21
```

### Step 2: Build the project

Create a build folder to run the program, then launch the commande 'cmake ..' and 'make' to build the program.
```
mkdir build
cd build
cmake ..
make
```

### Step 3: Run the program 

#### User parameters 

In order to run the program, you have to provide some parameters to configure your simulation:
* the **total number of neurons** you want in your network (-n)
* the **total time** you want your simulation to last (-t)
* the **average number of connection** a random neuron have (-c)
* the **average intensity of connection** between two random neurons (-l)
* the **dispersion model** for number of connection (-M)
* the **proportion of each type of neurons** within the network (-T)
* the length of the **interval for neuron parameters** to be picked at random in (-d)
* the **names of the three files** in which the results will be printed (-o, -s, -p)

If you don't specify these arguments when you run the program, default parameters will be taken into account. The default parameters are:
* n = 500 neurons
* t = 200 simulation steps
* c = 30 connections
* l = 20 
* M = poisson distribution
* T = 50% RS, 50% FS
* d = parameters value +/- 0.1
* o = outfile.txt
* s = sample_file.txt
* p = param_file.txt

#### Specify user parameters 

To specify user parameters, you have to specify the one letter code for the parameter you want to provide (see above) followed by the value of the parameter. 

Here is an example to specify the number of neurons:
```
-n 5000
```

To specify the proportions of each type of neurons, this is a little bit more complicated: To have 20% of FS neurons, 20% of CH neurons and 60% of RS neurons, you have to type: 
```
-T 'FS:0.2,CH:0.2'
``` 
The proportion of RS neuron is the complementary to 1 of the other proportions. 

#### Run the simulation

The program is run by the command:
```
./NeuronNetwork
``` 
followed by the parameters that you want to specify. 

For example, a typical command is:
```
./NeuronNetwork -n 5000 -t 1000 -c 70 -l 5 -T 'FS:0.2,CH:0.2'
```
This create a network of 5000 neurons, an average of 70 connections per neurons with an average intensity of 5. 20% of neurons are FS, 20% are CH and the rest (60%) are RS neurons. There are 1000 simulation time steps. 

## Results

### Output files

By running the program, three output files are generated. 
* `outfile.txt` contains a matrix of '0' and '1'. The lines corresponds to each simulation step while the columns represents all neurons. When a special neuron at a special time step is firing, a '1' is printed, otherwise this is a '0' that is printed. 
* `sample_file.txt` contains the values of neuron potential, recovery time and synaptic current of a sample of neuron at each time step. The sample contains one neuron of each type that is present in the network.
* `param_file.txt` contains the cellular properties of each neurons. 

### Raster plot generation

In order to make these results more meaningfull, you can then use the `RasterPlots.R` program to transform the output files into graphics. 

To do that, upload the program into the folder `build` and type:
```
Rscript RasterPlots.R outfile.txt sample_file.txt param_file.txt
``` 
with the names of your three output files (here the default names). This will generate three PDF files. 

The first one contains the raster plot. The dark points spots the firing neurons. 
The second ones contains graphics representing evolution of potentiel, recovery and current for the sample of neuron of `sample_file.txt`. 
The third one contains the values of parameters for each neurons of the network, as listed in `param_file.txt`.

## Generate doxygen documentation

To generate the documentation of this program, just enter the command:
```
make doc
```
This will create a folder `doc` in which html files are generated. Just click on one of these files to open documentation. You can then navigate between the different classes to discover the program conception.



