#ifndef OBJECT_H
#define OBJECT_H

/*! Base class used to derive the Machine and Process classes */
class Object {
	public:
		/** Object's constructor
		 *
		 * @param object_id The Object's id
		 */
		Object (unsigned short int object_id);

		/** Returns the Object's id
		 *
		 * @return The Object's id
		 */
		unsigned short int get_id () const {return id;}

	protected:
		unsigned short int id; //!< The id of the Object
};

#endif // OBJECT_H
