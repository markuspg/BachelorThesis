diff --git a/problem.cpp b/problem.cpp
index 24add03..896aaac 100644
--- a/problem.cpp
+++ b/problem.cpp
@@ -76,6 +76,16 @@ Problem::Problem (std::string filename) {
 	input_file_stream.close();
 }
 
+Problem::Problem (const Problem &problem):
+	machines_quantity (problem.get_machines_quantity ()),
+	processes_quantity (problem.get_processes_quantity ()),
+	process_interval (problem.get_process_interval ())
+{
+	machines = problem.get_machines_pointer ();
+	processes = problem.get_processes_pointer ();
+}
+
+
 Problem::~Problem () {
 	for (unsigned short int i = 0; i < machines_quantity; i++) {
 		delete machines[i];
diff --git a/problem.hpp b/problem.hpp
index 6b4dc14..c9c4894 100644
--- a/problem.hpp
+++ b/problem.hpp
@@ -15,6 +15,12 @@ class Problem {
 		 */
 		Problem (std::string filename);
 
+		/** A copy constructor for the Problem
+		 *
+		 * @param rhs The Problem instance to copy
+		 */
+		Problem (const Problem &problem);
+
 		/*! Problem's destructor */
 		~Problem ();
 
@@ -37,6 +43,12 @@ class Problem {
 		 */
 		unsigned short int get_machines_quantity () const {return machines_quantity;}
 
+		/** Returns the upper bound of the processing times interval
+		 *
+		 * @return The maximum processing time of the Problem
+		 */
+		unsigned int get_process_interval () const {return process_interval;}
+
 		/** Returns a pointer to the Processes
 		 *
 		 * @return A pointer to the Processes
@@ -55,7 +67,7 @@ class Problem {
 		/*! Saves the Problem instance to a file */
 		void save_problem ();
 
-	private:
+	protected:
 		Machine **machines;
 		unsigned short int machines_quantity;
 		Process **processes;
