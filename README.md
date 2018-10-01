# Bachelor Thesis Implementation

This is the implementation for my 2013 bachelor thesis about the design, implementation and experimental analysis of heuristics for the _P||Cmin_ problem. The final commits of 2013 in all branches represent the project's state in my bachelor thesis. The weird project structure stems forth from an insufficient knowledge about Git back then. At some point I copied the project folder four times and continued programming in each copy. The 2018 commits merge this code together again, using this as an opportunity for some refactoring.

Problem instances for the experiments can be created using the _instance\_creator_ submodule. _bound\_calculator_ was used to compare different algorithms for bounds of solution values computation. _start\_solution\_tester_ was used to compare different start solution algorithms and _improver_ to compare different comparison heuristics.
