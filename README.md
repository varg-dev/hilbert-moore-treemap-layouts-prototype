# Hilbert and Moore Treemap Layouts Prototype

This repository will contain the C++ prototype for a research paper submitted to the EuroVis 2021.
The source code, datasets, and scripts will be released with the publication of the paper.

When you use parts of this repository within your publications, consider referencing its accompanying publication:

* tbd

## Notes for Practitioners

The prototype is a command-line-based tool to load CSV-encoded text files that contain path-encoded nodes and additional attributes that can be selected as weights for the layout computation.
The tool is capable of reading the data, derive a tree structure, and perform preprocessing and layouting of Hilbert and Moore treemaps for datasets with millions of nodes.
The main output of the tool is the resulting layouts both in plain text and SVG format. The layouting process can be measured by means of run-time performance and the resulting layouts can be assessed using treemap layout techniques.

### Setup

The tool has the following dependencies:

* C++17
* CMake

### Compilation

tbd

### Steps for Reproduction of the Results

tbd

## Notes for Researchers and Collaborators

The tool is currently limited to layout creation, including the required preprocessing steps and the measurements. For this to get a full and usable visualization tool, a rendering component, a more sophisticated geometry creation, and a scene management are required, too.

If your are interested in the visualization of tree-structured data or further resources from us, feel free to contact us for a collaboration.

Contact:
 * Willy Scheibel ([willy.scheibel@hpi.de](mailto:willy.scheibel@hpi.de))
