#pragma once

#include "entities.h"
#include "Systems.h"
#include "EntityManager.h"

#include <unordered_map>
#include <memory>
#include <cereal.hpp>
#include "../cereal/types/unordered_map.hpp"

class World
{
private:

	struct SystemDeleter
	{
		void operator() (SystemBase * system) const;
	};

	// Index of map is the ID og system, generated by SystemTypeId<class t>()
	// Allows only one of each system to be stored in world
	using SystemArray = std::unordered_map<TypeId, std::unique_ptr<SystemBase, SystemDeleter>>;

public:
	using EntityArray = std::vector<Entity>;

	World();
	
	// Constructs a world with a entity pool
	// the size of entitySizePool
	explicit World(std::size_t entityPoolSize);

	// Add a system type to world
	template<typename TSystem>
	void addSystem(TSystem & system);

	// Removes system of type TSystem
	template<typename TSystem>
	void removeSystem();

	// Does system of type TSystem exist?
	template<typename TSystem>
	bool doesSystemExist() const;

	// Remove all existing systems
	void removeAllSystems();

	// Create a new entity
	Entity createEntity();

	// Create a new entity with an entity template function
	//
	// Template EntityTemplateFunct - type of template function
	// fn - function to apply to entity
	// args - arguments of function
	template< typename EntityTemplateFunct, typename... Args>
	Entity createEntity(EntityTemplateFunct fn, Args&&... args) 
	{
		Entity en = createEntity();
		fn(en, std::forward<Args>(args)...);
		return en;
	}

	// Create multiple entities, returns EntityArray
	EntityArray createEntities(std::size_t number);

	// Deactivates entity and marks for removal
	void killEntity(Entity & entity);

	// Marks multiple entities for removal
	void killEntities(std::vector<Entity>& entities);

	// Mark entity active
	void activateEntity(Entity & entity);

	// Mark an entity for inactivity, not deletion
	void deactivateEntity(Entity & entity);

	// Check if an entity is active
	bool isActivated(const Entity & entity) const;

	// Determines if an entity is valid.
	bool isValid(const Entity & entity) const;

	// Refreshes world
	void refresh();

	// Removes all systems and all entities
	// from world.
	void clear();

	// Returns a count of entities attached
	// to world. Counts dead and alive entities
	std::size_t getEntityCount() const;

	// Return all entities in world
	const EntityArray& getAllEntities() const;

	EntityArray& getAllEntities();

	// Returns entity and index of idx
	Entity getEntity(std::size_t idx);

	// Mailbox message holder
	std::vector<std::unique_ptr<impl::subscription_base_t>> pubsub_holder;

	// Message Emitter, immeditely calls function pointer
	// associated with message
	template <class MSG>
	inline void emit(MSG message) {
		impl::message_t<MSG> handle(message);
		if (pubsub_holder.size() > handle.family_id) {
			for (auto &func : static_cast<impl::subscription_holder_t<MSG> *>(pubsub_holder[handle.family_id].get())->subscriptions) {
				if (std::get<0>(func) && std::get<1>(func)) {
					std::get<1>(func)(message);
				}
				else {
					// It is destined for the system's mailbox queue.
					auto finder = std::get<2>(func)->mailboxes.find(handle.family_id);
					if (finder != std::get<2>(func)->mailboxes.end()) {
						static_cast<impl::mailbox_t<MSG> *>(finder->second.get())->messages.push(message);
					}
				}
			}
		}
	}

	// Message is deffered until all messages are checked by world
	template <class MSG>
	inline void emit_deferred(MSG message) {
		impl::message_t<MSG> handle(message);
		if (pubsub_holder.size() > handle.family_id) {

			auto * subholder = static_cast<impl::subscription_holder_t<MSG> *>(pubsub_holder[handle.family_id].get());
			std::lock_guard<std::mutex> postlock(subholder->delivery_mutex);
			subholder->delivery_queue.push(message);
		}
	}

	// Deliver all messages to systems.
	// Must be called manually
	inline void deliver_messages() {
		for (auto &holder : pubsub_holder) {
			if (holder) holder->deliver_messages();
		}
	}
	
	// Save the world!
	// Saves all entities, all their components
	// does not save systems as those will likely be created
	// in the same order each time
	template<class Archive>
	void save(Archive& archive)
	{
		deliver_messages();
		archive(entityIdPool, entityAttributes, entityCache);
	}

	// Load the world!
	// Cereal cannot store raw pointers so we 
	// set the Entities world pointers here
	// Do not refresh the world before adding all systems
	// which the world that your loading had
	template<class Archive>
	void load(Archive& archive)
	{
		archive(entityIdPool, entityAttributes, entityCache);

		for (auto& e : getAllEntities())
		{
			e.setWorld(*this);
			e.activate();
		}
	}

	// If you wish to serialize data you must
	// Register each component in an identical order
	template<typename T>
	void registerComponent()
	{
		ComponentTypeId<T>();
	}

private:

	SystemArray systems;

	// Class used for accessing new and invalidated id's
	// recycles old id's
	EntityIdPool entityIdPool;

	struct EntityAttributes
	{
		struct Attributes
		{
			// Denoting whether entity is active
			bool activated;

			// A bitset that can be &'d against to test
			// whether entity has a particual system
			// or set of systems
			std::vector<bool> systems;

			template<class Archive>
			void serialize(Archive& archive)
			{
				// Systems are not serialized as
				// systems will be added when entity is re-activated on load
				archive(activated);
			}
		};

		explicit EntityAttributes(std::size_t amountOfEntities) 
			: entityManager(amountOfEntities), attributes(amountOfEntities) {};

		// Mainly storage container for entities
		// and their components. Provides some helpers
		EntityManager entityManager;

		// Attributes of each and every entity
		std::vector<Attributes> attributes;

		void resize(std::size_t amountOfEntities)
		{
			entityManager.resize(amountOfEntities);
			attributes.resize(amountOfEntities);
		}

		void clear() 
		{
			entityManager.clear();
			attributes.clear();
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(entityManager, attributes);
		}
	};

	// Holds details about whether an entity is active
	// and which systems said Entity holds
	EntityAttributes entityAttributes;

	struct EntityCache
	{
		// Contains all live Entities
		EntityArray alive;

		// Holds killed Entities, 
		// get's cleared everry refresh call
		EntityArray killed;

		// Holds just activated Entities
		// get's cleared on refresh call
		EntityArray activated;

		// Holds just deactivated Entities
		// get's cleared on refresh call
		EntityArray deactivated;

		// Clears temporary cache
		void clearTemp()
		{
			killed.clear();
			activated.clear();
			deactivated.clear();
		}

		// Clears all cache
		void clear()
		{
			alive.clear();
			clearTemp();
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(alive, killed, activated, deactivated);
		}
	};

	EntityCache entityCache;

	// Size helper functions
	void checkForResize(std::size_t amountOfEntitiesToAllocate);
	void resize(std::size_t amount);

	// System helper functions
	void addSystem(SystemBase& system, TypeId systemTypeId);
	void removeSystem(TypeId systemTypeId);
	bool doesSystemExist(TypeId systemTypeId) const;

	friend class Entity;
};

template<typename TSystem>
inline void World::addSystem(TSystem & system)
{
	addSystem(system, SystemTypeId<TSystem>());
}

template<typename TSystem>
inline void World::removeSystem()
{
	removeSystem(SystemTypeId<TSystem>());
}

template<typename TSystem>
inline bool World::doesSystemExist() const
{
	return doesSystemExist(SystemTypeId<TSystem>());
}

// Messaging
namespace impl
{
	template<class MSG>
	inline void subscribe(World &ECS, SystemBase &B, std::function<void(MSG &message)> destination) {
		MSG empty_message{};
		impl::message_t<MSG> handle(empty_message);
		if (ECS.pubsub_holder.size() < handle.family_id + 1) {
			ECS.pubsub_holder.resize(handle.family_id + 1);
		}
		if (!ECS.pubsub_holder[handle.family_id]) {
			ECS.pubsub_holder[handle.family_id] = std::move(std::make_unique<subscription_holder_t<MSG>>());
		}
		static_cast<subscription_holder_t<MSG> *>(ECS.pubsub_holder[handle.family_id].get())->subscriptions.push_back(std::make_tuple(true, destination, nullptr));
	}

	template<class MSG>
	inline void subscribe_mbox(World &ECS, SystemBase &B) {
		MSG empty_message{};
		impl::message_t<MSG> handle(empty_message);
		if (ECS.pubsub_holder.size() < handle.family_id + 1) {
			ECS.pubsub_holder.resize(handle.family_id + 1);
		}
		if (!ECS.pubsub_holder[handle.family_id]) {
			ECS.pubsub_holder[handle.family_id] = std::move(std::make_unique<subscription_holder_t<MSG>>());
		}
		std::function<void(MSG &message)> destination; // Deliberately empty
		static_cast<impl::subscription_holder_t<MSG> *>(ECS.pubsub_holder[handle.family_id].get())->subscriptions.push_back(std::make_tuple(false, destination, &B));
		B.mailboxes[handle.family_id] = std::make_unique<impl::mailbox_t<MSG>>();
	}
}