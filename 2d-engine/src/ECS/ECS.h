#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>

const unsigned int MAX_COMPONENTS = 32;

/////////////////////////////////////////////////////////////////////////////////
// Signature
/////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of wich entities a system is interested in.
/////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

// This is an abstract interface, it really not exists, it's like a blueprint
// We need it to be able to create a Component list (vector) since Component 
// is in reality just a template with several potencial names
// Inheritance help us to manage that different classes used as TComponent
struct IComponent {
protected:
	static int nextId;
};

// Used to assign an unique id to a component type
// Cada tipo de componente de la entidad tendrá un id distinto
// Se transformará en múltiples clases, una para cada Component<T>
template <typename TComponent> class Component: public IComponent {
public:// Returns the unique id of Component<TComponent>
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

class Entity {
private:
	int id;
public:
	Entity(int id) : id(id) {}; // inicializa el parámetro id con ese valor
	Entity(const Entity& entity) = default;
	int GetId() const; // no se modificará

	// Operator Overloading
	Entity& operator =(const Entity& other) = default;
	bool operator ==(const Entity & other) const { return id == other.id; }
	bool operator !=(const Entity & other) const { return id != other.id; }
	bool operator >(const Entity & other) const { return id > other.id; }
	bool operator <(const Entity & other) const { return id < other.id; }

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	// Hold a pointer to the entity's owner registry
	class Registry* registry;
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

/////////////////////////////////////////////////////////////////////////////////
// Pool
/////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contigous data) of objects type T
/////////////////////////////////////////////////////////////////////////////////
class IPool { // esta interfaz vacía nos sirve para establecer un tipo en el vector inferior
public:
	virtual ~IPool(){} // el destructor virtual permite que la clase permanezca abstracta
};

template <typename T>
class Pool: public IPool {
private:
	std::vector<T> data;
public:
	Pool(int size = 100) { data.resize(size); }
	virtual ~Pool() = default;
	bool isEmpty() const { return data.empty(); }
	int GetSize() const { return data.size(); }
	void Resize(int n) { data.resize(n); }
	void Clear() { data.clear(); }
	void Add(T object) { data.push_back(object); }
	void Set(int index, T object) { data[index] = object; }
	T& Get(int index) { return static_cast<T&>(data[index]); }
	T& operator [](unsigned int index) { return data[index]; }
};

/////////////////////////////////////////////////////////////////////////////////
// Registry
/////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems, and components.
/////////////////////////////////////////////////////////////////////////////////
class Registry {
private:
	int numEntities = 0;
	// Vector of component pools, each pool contains all the data for a certain component type
	// Vector index = component type id
	// Pool index = entity id
	//std::vector<IPool*> componentPools; // como no sabes el tipo de Pool (es un template) y necesitas una forma de definir el vector, usas una interfaz vacía
	std::vector<std::shared_ptr<IPool>> componentPools;

	// Vector of component signatures
	// The signature lets us know which components are turned "on" for an entity
	// [Vector index = entity id]
	std::vector<Signature> entityComponentSignatures;

	// Map of active systems [index = system typeid]
	// std::unordered_map<std::type_index, System*> systems
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	// Entities awaiting creation in the next Registry Update()
	std::set<Entity> entitiesToBeAdded;

	// Entities awaiting destruction in the next Registry Update()
	std::set<Entity> entitiesToBeKilled;

public:
	Registry() {
		spdlog::info("Registry constructor called");
	}

	~Registry() {
		spdlog::info("Registry destructor called");
	}

	// The registry Update() finally process the entities that are waiting to be added/killed
	void Update();

	/////////////////////////////////////////////////////////////////////////////////
	// Entities management
	/////////////////////////////////////////////////////////////////////////////////
	Entity CreateEntity();

	/////////////////////////////////////////////////////////////////////////////////
	// Component management
	/////////////////////////////////////////////////////////////////////////////////
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	/////////////////////////////////////////////////////////////////////////////////
	// System management
	/////////////////////////////////////////////////////////////////////////////////
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	// Check the component signature of an entity and add the entity to the systems that are interesed in it
	void AddEntityToSystems(Entity entity);
};

/////////////////////////////////////////////////////////////////////////////////
// Templates are not real functions, right here are just placeholders
// They will be transformed in several real functions when class T is passed
// https://docs.microsoft.com/es-es/cpp/cpp/templates-cpp?view=msvc-160
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Entity template functions implementation
/////////////////////////////////////////////////////////////////////////////////

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}

/////////////////////////////////////////////////////////////////////////////////
// System template functions implementation
/////////////////////////////////////////////////////////////////////////////////
template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/////////////////////////////////////////////////////////////////////////////////
// Registry template functions implementation
/////////////////////////////////////////////////////////////////////////////////
template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
	// TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
	// of the pointer is at end of the map, the key-pair has not been found
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	// first -> key, second -> value
	return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const unsigned int componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size()) {
		// increment component pool capacity to accomodate the new componentId
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		// if this pool position is nullptr create a new component pool
		// Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	// get the component pool of that specific component type
	// Pool<TComponent>* componentPool = componentPools[componentId];
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	// if the pool is too small to accomodate the entity we resize it 
	if (entityId > componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	// now im ready to create a new component of T type
	TComponent newComponent(std::forward<TArgs>(args)...);

	// set that component pool position at that entity id index with the new component that we have created
	componentPool->Set(entityId, newComponent);

	// save it in my entity component signatures turning that component id on by setting 1 in the bit set position on the component id
	entityComponentSignatures[entityId].set(componentId);

	spdlog::info("The componentId = {0} was added to entityId = {1}", std::to_string(componentId), entityId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// turn the component id off by setting 0 in the bit set position on the component id
	entityComponentSignatures[entityId].set(componentId, false);

	spdlog::info("The componentId = {0} was removed from entityId = {1}", std::to_string(componentId), entityId);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// check the component in the bit set position on the component id
	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->Get(entityId);
}