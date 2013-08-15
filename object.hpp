/*! Base class used to derive the Machine and Process classes */
class Object {
	public:
		/*! Object's constructor */
		Object (unsigned short int object_id);
		/*! Returns the Object's id */
		unsigned short int get_id () const {return id;}

	protected:
		unsigned short int id;
};
