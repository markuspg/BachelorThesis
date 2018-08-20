#ifndef OBJECT_H
#define OBJECT_H

namespace bct {

class Object {
	public:
        Object (const unsigned short int argObjectId);

        unsigned short int GetId() const noexcept;

	protected:
        const unsigned short int id;
};

} // namespace bct

inline unsigned short int bct::Object::GetId() const noexcept {
    return id;
}

#endif // OBJECT_H
