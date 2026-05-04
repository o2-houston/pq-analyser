Power Quality Waveform Analyser

A three-phase power quality waveform analyser written in C.

Made using CLion in C99, CMake v4.1.


========== Using the Program ==========

The input CSV file should be placed in the root directory of the project. The program explicitly seeks the name "power_quality_log.csv".

========== Program Outputs ==========

- results.txt:
A text file containing the analysis report (RMS, peak-to-peak values, clipping detection, etc.).


- sort_phA.csv, sort_phB.csv, sort_phC.csv:
Separate CSV files for each phase (A, B, C) sorted by voltage magnitude for the given phase.

========== Building the Project ==========

1. Clone the repository
```
git clone https://github.com/o2-houston/pq-analyzer.git
cd pq-analyzer
```

2. Create the build directory:
```
mkdir build
cd build
```

3. Generate the build files with CMake:```cmake ..```
4. Compile the program: ```make```

