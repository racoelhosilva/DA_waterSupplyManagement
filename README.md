### Using custom Datasets

Custom Datasets can be used in the project. To use them follow these instructions:
1. Put the files in a folder named "dataset" in the main project directory according to the following rules:
  - The **Reservoir** file should be named "Reservoir.csv" 
  - The **Stations** file should be named "Stations.csv" 
  - The **Cities** file should be named "Cities.csv" 
  - The **Pipes** file should be named "Pipes.csv"
  - The overall structure of the CSV files should be the same as the ones in the other dataset
2. When starting the program, select the option ```[3]```in the menu to load a custom dataset

The structure should be like:
```
$ tree dataset/

  dataset
    ├──── Reservoir.csv
    ├──── Stations.csv
    ├──── Cities.csv
    └──── Pipes.csv
```
