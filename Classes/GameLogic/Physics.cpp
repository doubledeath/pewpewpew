#include "Physics.h"

USING_NS_CC;

Physics *Physics::Create() {
    return Physics::create();
}

bool Physics::init() {
    if (!Node::init()) {
        return false;
    }

    scheduleUpdate();

    return true;
}

void Physics::update(float Delta) {
    Node::update(Delta);

    std::vector<std::pair<Collidable *, std::vector<Node *>>> CollidesToNodesChildren;

    for (auto CollidableChild : CollidableChildren) {
        if (CollidableChild->IsCollisionForceIgnored()) {
            continue;
        }

        if (auto _Node = dynamic_cast<cocos2d::Node *>(CollidableChild)) {
            auto CollidesToWorld = IsCollidesWorld(_Node);
            auto CollidesToNodes = GetCollidedNodes(_Node);

            if (CollidesToWorld) {
                CollidableChild->CollidesToWorld();
            }

            if (CollidesToNodes.size() > 0) {
                CollidesToNodesChildren.push_back(std::pair<Collidable *, std::vector<Node *>>(CollidableChild, CollidesToNodes));
            }
        }
    }

    // Another pass for nodes collision
    for (auto CollidesToNodes : CollidesToNodesChildren) {
        CollidesToNodes.first->CollidesToNodes(CollidesToNodes.second);
    }

    for (auto Child : getChildren()) {
        Child->update(Delta);
    }
}

void Physics::addChild(cocos2d::Node *Child) {
    Node::addChild(Child);

    if (auto CollidableChild = dynamic_cast<Collidable *>(Child)) {
        CollidableChildren.push_back(CollidableChild);
    }
}

void Physics::removeChild(cocos2d::Node *Child, bool CleanUp) {
    if (auto CollidableChild = dynamic_cast<Collidable *>(Child)) {
        CollidableChildren.erase(remove(CollidableChildren.begin(), CollidableChildren.end(), CollidableChild),
                                 CollidableChildren.end());
    }

    Node::removeChild(Child, CleanUp);
}

bool Physics::IsCollidesToWorld(cocos2d::Node *Node) {
    return IsCollidesWorld(Node);
}

bool Physics::IsCollidesToNode(cocos2d::Node *Node, cocos2d::Node *Target) {
    auto CollidesToNodes = GetCollidedNodes(Node);

    return std::find(CollidesToNodes.begin(), CollidesToNodes.end(), Target) != CollidesToNodes.end();
}

bool Physics::IsCollidesWorld(cocos2d::Node *Target) {
    auto WorldOrigin = Director::getInstance()->getVisibleOrigin();
    auto WorldSize = Director::getInstance()->getVisibleSize();
    auto TargetOrigin = Target->getBoundingBox().origin;
    auto TargetSize = Target->getBoundingBox().size;

    return TargetOrigin.x <= WorldOrigin.x||
           TargetOrigin.y + TargetSize.height >= WorldOrigin.y + WorldSize.height ||
           TargetOrigin.x + TargetSize.width >= WorldOrigin.x + WorldSize.width ||
           TargetOrigin.y <= WorldOrigin.y;
}

std::vector<cocos2d::Node *> Physics::GetCollidedNodes(cocos2d::Node *Target) {
    std::vector<Node *> CollidedChildren;

    for (auto Child : CollidableChildren) {
        if (auto Node = dynamic_cast<cocos2d::Node *>(Child)) {
            if (Target == Node || Child->IsCollisionForceIgnored()) {
                continue;
            }

            auto TargetOrigin = Target->getBoundingBox().origin;
            auto TargetSize = Target->getBoundingBox().size;
            auto ChildOrigin = Node->getBoundingBox().origin;
            auto ChildSize = Node->getBoundingBox().size;

            if (TargetOrigin.x < ChildOrigin.x + ChildSize.width &&
                TargetOrigin.y + TargetSize.height > ChildOrigin.y &&
                TargetOrigin.x + TargetSize.width > ChildOrigin.x &&
                TargetOrigin.y < ChildOrigin.y + ChildSize.height) {
                CollidedChildren.push_back(Node);
            }
        }
    }

    return CollidedChildren;
}
