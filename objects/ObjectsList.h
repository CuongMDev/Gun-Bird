#ifndef OBJECTSLIST_H
#define OBJECTSLIST_H

#include "Object.h"
#include <list>

class ObjectsList
{
private:
    std::list<Object*> objectList;

public:
    ObjectsList();
    ~ObjectsList();

    int count();

    void add(Object *object);
    void reset();
    void renderAll();

    bool getCollisionObjects(ObjectsList& objectsList, std::list<Object*>::iterator &objectA, std::list<Object*>::iterator &objectB);
    bool getCollisionObject(const Object& objectB, std::list<Object*>::iterator &object);
    void deleteObject(const std::list<Object*>::iterator &object);
};

ObjectsList::ObjectsList()
= default;

ObjectsList::~ObjectsList()
= default;

int ObjectsList::count()
{
    return (int)objectList.size();
}


void ObjectsList::add(Object *object)
{
    objectList.push_back(object);
}

void ObjectsList::reset()
{
    //clear bats
    for (auto object : objectList) {
        delete object;
    }
    objectList.clear();
}

void ObjectsList::renderAll()
{
    //check collision before render
    for (auto object = objectList.begin(); object != objectList.end();) {
        if (!(*object)->render()) {
            delete (*object);
            object = objectList.erase(object);
        }
        else object++;
    }
}

bool ObjectsList::getCollisionObject(const Object &objectB, std::list<Object*>::iterator &object)
{
    for (auto obj = objectList.begin(); obj != objectList.end(); obj++) {
        if ((*obj)->checkCollisionObject(objectB)) {
            object = obj;
            return true;
        }
    }

    return false;
}

bool ObjectsList::getCollisionObjects(ObjectsList &objectsListB, std::list<Object*>::iterator &objectA, std::list<Object*>::iterator &objectB)
{
    for (auto objB = objectsListB.objectList.begin(); objB != objectsListB.objectList.end(); objB++) {
        std::list<Object*>::iterator objA;
        if (this->getCollisionObject(**objB, objA)) {
            objectA = objA;
            objectB = objB;
            return true;
        }
    }

    return false;
}

void ObjectsList::deleteObject(const std::list<Object *>::iterator &object)
{
    objectList.erase(object);
}

#endif //OBJECTSLIST_H
