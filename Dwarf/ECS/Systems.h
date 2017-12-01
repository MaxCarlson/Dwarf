#pragma once
#include "EntityManager.h"
#include "Filter.h"

#include <vector>

#include <queue>
#include <mutex>

class World;
class Entity;
class SystemBase;

struct base_message_t
{
	static std::size_t type_counter;
};

namespace impl
{
	template<class C>
	struct message_t : public base_message_t {
		message_t() {
			C empty;
			data = empty;
			family();
		}
		message_t(C comp) : data(comp) {
			family();
		}
		std::size_t family_id;
		C data;

		inline void family() {
			static std::size_t family_id_tmp = base_message_t::type_counter++;
			family_id = family_id_tmp;
		}
	};


	template<class MSG>
	inline void subscribe(World &ECS, SystemBase &B, std::function<void(MSG &message)> destination);

	template<class MSG>
	inline void subscribe_mbox(World &ECS, SystemBase &B);

	/*
	* Base class for storing subscriptions to messages
	*/
	struct subscription_base_t {
		virtual void deliver_messages() = 0;
	};

	/* Base class for subscription mailboxes */
	struct subscription_mailbox_t {
	};

	/* Implementation class for mailbox subscriptions; stores a queue */
	template <class C>
	struct mailbox_t : subscription_mailbox_t {
		std::queue<C> messages;
	};

	template <class C>
	struct subscription_holder_t : subscription_base_t {
		std::queue<C> delivery_queue;
		std::mutex delivery_mutex;
		std::vector<std::tuple<bool, std::function<void(C& message)>, SystemBase *>> subscriptions;

		virtual void deliver_messages() override {
			std::lock_guard<std::mutex> guard(delivery_mutex);
			while (!delivery_queue.empty()) {
				C message = delivery_queue.front();
				delivery_queue.pop();
				message_t<C> handle(message);

				for (auto &func : subscriptions) {
					if (std::get<0>(func) && std::get<1>(func)) {
						std::get<1>(func)(message);
					}
					else {
						// It is destined for the system's mailbox queue.
						auto finder = std::get<2>(func)->mailboxes.find(handle.family_id);
						if (finder != std::get<2>(func)->mailboxes.end()) {
							static_cast<mailbox_t<C> *>(finder->second.get())->messages.push(message);
						}
					}
				}
			}
		}
	};
}

class SystemBase
{
public:

	SystemBase(const Filter & filter) : world(nullptr), filter(filter) {};

	virtual ~SystemBase() {};

	// Returns a Filter object that holds two
	// bool vectors that describe required and 
	// excluded components
	const Filter& getFilter() const { return filter; };

	// Returns refrernce to world
	// this system is associated with
	World& getWorld() const { return *world; };


	// Returns all entitys of a system
	const std::vector<Entity>& getEntities() const { return entities; }

	// Holds subscribed messages
	std::unordered_map<std::size_t, std::unique_ptr<impl::subscription_mailbox_t>> mailboxes;

	// Subscribe this system to messages of class MSG
	template<class MSG>
	void subscribe(World &ECS, std::function<void(MSG &message)> destination) {
		impl::subscribe<MSG>(ECS, *this, destination);
	}

	template<class MSG>
	void subscribe(std::function<void(MSG &message)> destination) {
		subscribe<MSG>(*this->world, destination);
	}

	template<class MSG>
	void subscribe_mbox(World &ECS) {
		impl::subscribe_mbox<MSG>(ECS, *this);
	}

	template<class MSG>
	void subscribe_mbox() {
		subscribe_mbox<MSG>(*this->world);
	}

	template<class MSG>
	std::queue<MSG> * mbox() {
		impl::message_t<MSG> handle(MSG{});
		auto finder = mailboxes.find(handle.family_id);
		if (finder != mailboxes.end()) {
			return &static_cast<impl::mailbox_t<MSG> *>(finder->second.get())->messages;
		}
		else {
			return nullptr;
		}
	}

	template<class MSG>
	void each_mbox(const std::function<void(const MSG&)> &func) {
		std::queue<MSG> * mailbox = mbox<MSG>();
		while (!mailbox->empty()) {
			MSG msg = mailbox->front();
			mailbox->pop();
			func(msg);
		}
	}

	// Emit message immeidetly from within
	// this system
	template<class MSG>
	inline void emit(MSG message)
	{
		this->world->emit<MSG>(message);
	}

	template<class MSG>
	inline void emit_deffered(MSG message)
	{
		this->world->emit_deferred<MSG>(message);
	}

private:

	virtual void initialize() {};

	virtual void onEntityAdd(Entity & entity) {};

	virtual void onEntityRemoval(Entity & entity) {};

	void addEntity(Entity & entity)
	{
		entities.push_back(entity);
		onEntityAdd(entity);
	}

	void remove(Entity & entity)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		onEntityRemoval(entity);
	}

	void setWorld(World & setWorld)
	{
		world = &setWorld;
		initialize();
	}

	World * world;

	// The component filter // Bitset to & against components of entitys to test if they have correct components for system
	Filter filter;

	// All entities that this system should care about
	// or that are attached
	std::vector<Entity> entities;

	friend class World;
};

template<class RequireList, class ExcludeList = Excludes<>>
class System : public SystemBase
{
public:
	System() : SystemBase{ MakeFilter<RequireList, ExcludeList>() } {};

};

// Return a unique system ID if system is
// being generated/add for first time.
// Otherwise return the ID associated with
// system of type T
template<class T>
TypeId SystemTypeId()
{
	return ClassTypeId<SystemBase>::getTypeId<T>();
}










