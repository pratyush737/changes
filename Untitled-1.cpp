#include "../../header/Collision/CollisionService.h"
#include "../../header/Collision/ICollider.h"
#include <algorithm> // For std::remove

namespace Collision
{
    CollisionService::CollisionService() { }

    CollisionService::~CollisionService() { }

    void CollisionService::initialize() { }

    void CollisionService::update()
    {
        processCollision();
    }

    void CollisionService::processCollision()
    {
        for (size_t i = 0; i < collider_list.size(); i++)
        {
            for (size_t j = i + 1; j < collider_list.size(); j++)
            {
                doCollision(i, j);
            }
        }
    }

    void CollisionService::doCollision(size_t index_i, size_t index_j)
    {
        if (index_i >= collider_list.size() || index_j >= collider_list.size()) return;

        if (collider_list[index_i]->getCollisionState() == CollisionState::DISABLED ||
            collider_list[index_j]->getCollisionState() == CollisionState::DISABLED) return;

        if (hasCollisionOccurred(index_i, index_j))
        {
            if (areActiveColliders(index_i, index_j))
                collider_list[index_i]->onCollision(collider_list[index_j]);

            if (areActiveColliders(index_i, index_j))
                collider_list[index_j]->onCollision(collider_list[index_i]);
        }
    }

    bool CollisionService::hasCollisionOccurred(size_t index_i, size_t index_j)
    {
        if (index_i >= collider_list.size() || index_j >= collider_list.size()) return false;

        const sf::Sprite& collider_one_sprite = collider_list[index_i]->getColliderSprite();
        const sf::Sprite& collider_two_sprite = collider_list[index_j]->getColliderSprite();

        return collider_one_sprite.getGlobalBounds().intersects(collider_two_sprite.getGlobalBounds());
    }

    bool CollisionService::areActiveColliders(size_t index_i, size_t index_j)
    {
        if (index_i >= collider_list.size() || index_j >= collider_list.size()) return false;

        return (collider_list[index_i] != nullptr && collider_list[index_j] != nullptr);
    }

    void CollisionService::addCollider(ICollider* collider)
    {
        collider_list.push_back(collider);
    }

    void CollisionService::removeCollider(ICollider* collider)
    {
        collider_list.erase(std::remove(collider_list.begin(), collider_list.end(), collider), collider_list.end());
    }
}
