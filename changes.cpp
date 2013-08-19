diff --git a/changes.cpp b/changes.cpp
index 022190d..e69de29 100644
--- a/changes.cpp
+++ b/changes.cpp
@@ -1,60 +0,0 @@
-diff --git a/problem.cpp b/problem.cpp
-index 24add03..896aaac 100644
---- a/problem.cpp
-+++ b/problem.cpp
-@@ -76,6 +76,16 @@ Problem::Problem (std::string filename) {
- 	input_file_stream.close();
- }
- 
-+Problem::Problem (const Problem &problem):
-+	machines_quantity (problem.get_machines_quantity ()),
-+	processes_quantity (problem.get_processes_quantity ()),
-+	process_interval (problem.get_process_interval ())
-+{
-+	machines = problem.get_machines_pointer ();
-+	processes = problem.get_processes_pointer ();
-+}
-+
-+
- Problem::~Problem () {
- 	for (unsigned short int i = 0; i < machines_quantity; i++) {
- 		delete machines[i];
-diff --git a/problem.hpp b/problem.hpp
-index 6b4dc14..c9c4894 100644
---- a/problem.hpp
-+++ b/problem.hpp
-@@ -15,6 +15,12 @@ class Problem {
- 		 */
- 		Problem (std::string filename);
- 
-+		/** A copy constructor for the Problem
-+		 *
-+		 * @param rhs The Problem instance to copy
-+		 */
-+		Problem (const Problem &problem);
-+
- 		/*! Problem's destructor */
- 		~Problem ();
- 
-@@ -37,6 +43,12 @@ class Problem {
- 		 */
- 		unsigned short int get_machines_quantity () const {return machines_quantity;}
- 
-+		/** Returns the upper bound of the processing times interval
-+		 *
-+		 * @return The maximum processing time of the Problem
-+		 */
-+		unsigned int get_process_interval () const {return process_interval;}
-+
- 		/** Returns a pointer to the Processes
- 		 *
- 		 * @return A pointer to the Processes
-@@ -55,7 +67,7 @@ class Problem {
- 		/*! Saves the Problem instance to a file */
- 		void save_problem ();
- 
--	private:
-+	protected:
- 		Machine **machines;
- 		unsigned short int machines_quantity;
- 		Process **processes;
diff --git a/machine.hpp b/machine.hpp
index cc14bcc..948252a 100644
--- a/machine.hpp
+++ b/machine.hpp
@@ -14,9 +14,6 @@ class Machine : public Object {
 		 */
 		void assign_process_to_machine (Process *process);
 
-		/*! Computes the Machine's completion time */
-		void compute_completion_time ();
-
 		/** Returns the Machine's completion time
 		 *
 		 * @return The Machine's completion time
@@ -30,6 +27,9 @@ class Machine : public Object {
 		std::vector<Process*> *get_processes ();
 		
 	private:
+		/*! Computes the Machine's completion time */
+		void compute_completion_time ();
+
 		bool changed; //!< Saves if the completion time of the Machine changed
 		unsigned int machine_completion_time; //!< The completion time of the machine
 		std::vector<Process*> v_processes; //!< A vector containing pointers to the assigned Processes
diff --git a/main.cpp b/main.cpp
index 02ecf57..b05479a 100644
--- a/main.cpp
+++ b/main.cpp
@@ -39,7 +39,7 @@ int main (int argc, char *argv[]) {
 	}
 
 	// Query the Problem's state
-	problem->query_problem_state ();
+	problem->query_state ();
 
 	Scheduler *scheduler = new Scheduler (*problem);
 	scheduler->create_start_solution (iLPT);
diff --git a/problem.cpp b/problem.cpp
index 910d6da..72e23a5 100644
--- a/problem.cpp
+++ b/problem.cpp
@@ -102,7 +102,7 @@ Problem::~Problem () {
 	return (i > j);
 }*/
 
-void Problem::query_problem_state () {
+void Problem::query_state () {
 	std::cout << "\nThe problem's state:\n";
 
 	// Output information about Processes
@@ -121,7 +121,22 @@ void Problem::query_problem_state () {
 	std::cout << "\t" << process_interval << " interval span\n";
 }
 
-void Problem::save_problem_instance (std::string *filename) {
+unsigned short int Problem::query_lowest_workload_machine () {
+	unsigned short int lowest_workload_machine = 0;
+	unsigned int lowest_workload = 0;
+	lowest_workload = machines[0]->get_completion_time ();
+	for (unsigned short int i = 1; i < machines_quantity; i++) {
+		if (machines[i]->get_completion_time () < lowest_workload) {
+			lowest_workload = machines[i]->get_completion_time ();
+			lowest_workload_machine = i;
+
+		}
+	}
+
+	return lowest_workload_machine;
+}
+
+void Problem::save_instance (std::string *filename) {
 	if (filename == nullptr) {
 		// Get the name of the file to write to
 		filename = new std::string ("problem.pbl");
diff --git a/problem.hpp b/problem.hpp
index fbd3ca1..fb34f2e 100644
--- a/problem.hpp
+++ b/problem.hpp
@@ -21,7 +21,8 @@ class Problem {
 		 */
 		Problem (const Problem &rhs);
 
-		/*! Problem's destructor */
+		/** Problem's destructor
+		 */
 		~Problem ();
 
 		/** Returns a pointer to the Machines
@@ -54,14 +55,20 @@ class Problem {
 		 */
 		unsigned short int get_processes_quantity () const {return processes_quantity;}
 
-		/*! Writes the problem to the stdout */
-		void query_problem_state ();
+		/*! Writes the Problem's state to stdout */
+		void query_state ();
+
+		/** Query the Machine with the shortest completion time
+		 *
+		 * @return The index of the Machine with the shortest completion time
+		 */
+		unsigned short int query_lowest_workload_machine ();
 
 		/** Saves the Problem instance to a file
 		 *
 		 * @param filename The filename the problem's instance data shall be written to. If not given, the user will be asked for it
 		 */
-		void save_problem_instance (std::string *filename);
+		void save_instance (std::string *filename);
 
 	protected:
 		Machine **machines; //!< A pointer to the Machines of the Problem
diff --git a/scheduler.cpp b/scheduler.cpp
index 9725fd2..9baaea7 100644
--- a/scheduler.cpp
+++ b/scheduler.cpp
@@ -48,17 +48,3 @@ void Scheduler::create_start_solution (unsigned int choice) {
 			std::cerr << "ERROR: Invalid start solution algorithm\n";
 	}
 }
-
-unsigned short int Scheduler::query_lowest_workload_machine () {
-	unsigned short int lowest_workload_machine = 0;
-	unsigned int lowest_workload = 0;
-	lowest_workload = machines[0]->get_completion_time ();
-	for (unsigned short int i = 1; i < machines_quantity; i++) {
-		if (machines[i]->get_completion_time () < lowest_workload) {
-			lowest_workload = machines[i]->get_completion_time ();
-			lowest_workload_machine = i;
-		}
-	}
-
-	return lowest_workload_machine;
-}
diff --git a/scheduler.hpp b/scheduler.hpp
index b13a862..d4f12f0 100644
--- a/scheduler.hpp
+++ b/scheduler.hpp
@@ -17,15 +17,9 @@ class Scheduler : public Problem {
 		/*! Applies the STUPID algorithm to create a starting solution for the given instance stored in the Scheduler */
 		void apply_STUPID_algorithm ();
 
-		/** Create a start solution using the given algorithm (1 = STUPID, 2 = LPT)
+		/** Create a start solution using the given algorithm (STUPID, iLPT, sLPT)
 		 *
 		 * @param algo The algorithm to use
 		 */
 		void create_start_solution (unsigned int algo);
-
-		/** Query the Machine with the shortest completion time
-		 *
-		 * @return The index of the Machine with the shortest completion time
-		 */
-		unsigned short int query_lowest_workload_machine ();
 };
