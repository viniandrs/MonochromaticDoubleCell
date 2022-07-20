# Supercell simulation
In this simulation we test a supercell of X-Arapuca devices in a cryostat filled with LAr. The goal is to determine the supercell's quantum efficienvy by combining data obtained in this simulation with experimental results.

The simulation was developed with the Geant4 toolkit and the data analysis was made with the ROOT framework.

## Running the simulation
To run the simulation, move to the "build" folder and run the command `./supercellSim ../macro/init_vis.mac <z_height>` where `<z_height>` is the z-position of the center of the alpha source, in centimeters, ranging from 1 to 48.