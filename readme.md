Power Quality Waveform Analyser

A three-phase power quality waveform analyser written in C.

Made using CLion in C99, CMake v4.1.


========== Using the Program ==========

The input CSV file should be placed in the same directory as the executable. The program explicitly seeks the name "power_quality_log.csv".

========== Program Outputs ==========

- results.txt:
A text file containing the analysis report (RMS, peak-to-peak values, clipping detection, etc.).


- sort_phA.csv, sort_phB.csv, sort_phC.csv:
Separate CSV files for each phase (A, B, C) sorted by voltage magnitude for the given phase.

========== Building the Project ==========

Follow these instructions in developer powershell or equivalent:
1. Clone the repository
```
git clone https://github.com/o2-houston/pq-analyser.git
cd pq-analyser
```

2. Create the build directory:
```
mkdir build
cd build
```

3. Generate the build files with CMake:```cmake ..```
4. Compile the program: ```make```
5. If the executable is not yet generated, enter: ```msbuild pq_analyser.slnx /p:Configuration=Release```
  
Note: Don't forget to place the datafile into the same directory as the executable with the filename "power_quality_log.csv"
