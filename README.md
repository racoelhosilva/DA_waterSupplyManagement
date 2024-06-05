<h1 align="center">DA - Water Supply Management</h1>

><p align="center">
> Calculate the water flows in a complex graph network with Reservoirs, Pumping Stations and Delivery Sites
></p>

### Running the Project

If the project is being run in the CLion terminal, make sure the option "Emulate terminal in output console" is enabled
in the Run/Debug configurations. This is necessary for capturing inputs for our program.

### Using custom Datasets

Custom Datasets can be used in the project. To use them follow these instructions:
1. Put the files in a folder named "dataset" in the main project directory according to the following rules:
    - The **Reservoir** file should be named "Reservoir.csv" 
    - The **Stations** file should be named "Stations.csv" 
    - The **Cities** file should be named "Cities.csv" 
    - The **Pipes** file should be named "Pipes.csv"
    - The overall structure of the CSV files should be the same as the ones in the other dataset
2. When starting the program, select the option ```[3]``` in the menu to load a custom dataset

The structure should be like:
```
$ tree dataset/

  dataset
    ├── Cities.csv
    ├── Pipes.csv
    ├── Reservoir.csv
    └── Stations.csv
```

---

> Class: 2LEIC15 Group: G02  
> Final Grade: 20.0  
> Professors: Liliana Ferreira  
> Created in March 2024 for DA (Desenho de Algoritmos) [FEUP-L.EIC016]  
