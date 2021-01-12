#pragma once
#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

/////////////////////////////////////////////////////////////////////////////////
// Signature
/////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of wich entities a system is interested in.
/////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

// This is an interface
struct IComponent {
protected:
	static int nextId;
};

// Used to assign an unique id to a component type
// Cada tipo de componente de la entidad tendrá un id distinto
template <typename T> 
class Component: public IComponent {
	// Returns the unique id of Component<T>
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

class Entity {
private:
	int id;
public:
	Entity(int id) : id(id) {};  // inicializa el parámetro id con ese valor
	Entity(const Entity& entity) = default;
	int GetId() const; // no se modificará

	// Operator Overloading
	Entity& operator = (const Entity& other) = default;
	bool operator == (const Entity& other) const { return id == other.id; }
	bool operator != (const Entity& other) const { return id != other.id; }
	bool operator > (const Entity & other) const { return id > other.id; }
	bool operator < (const Entity& other) const { return id < other.id; }
	bool operator >= (const Entity& other) const { return id >= other.id; }
	bool operator <= (const Entity& other) const { return id <= other.id; }
};

/////////////////////////////////////////////////////////////////////////////////
// System
/////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
/////////////////////////////////////////////////////////////////////////////////
class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default; 
	virtual ~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	// Define the component type T that entities must have to be considered by the system
	template <typename T>
	void RequireComponent();
};

class Registry {

};

// Implementation of the function template
template <typename T>
void System::RequireComponent() {
	const auto componentId = Component<T>::GetId();
	componentSignature.set(componentId);
}