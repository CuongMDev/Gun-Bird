#ifndef OBJECTSLIST_H
#define OBJECTSLIST_H

#include "Object.h"
#include <list>

class ObjectsList
{
private:
#define listIter std::list<Object*>::iterator

    std::list<Object*> objectList;

public:
    ObjectsList();
    ~ObjectsList();

    int count();

    void add(Object *object);
    void reset();
    void renderAll();

    bool getCollisionObjects(ObjectsList& objectsList, listIter &objectA, listIter &objectB, bool continueToFind = false);
    bool getCollisionObject(const Object& objectB, listIter &object, bool continueToFind = false);
    //return next object
    listIter deleteObject(const listIter &object);
    listIter getBegin();
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

bool ObjectsList::getCollisionObject(const Object &objectB, listIter &object, bool continueToFind)
{
    listIter obj;
    if (continueToFind) obj = object;
    else obj = objectList.begin();

    for (; obj != objectList.end(); obj++) {
        if ((*obj)->checkCollisionObject(objectB)) {
            object = obj;
            return true;
        }
    }

    return false;
}

bool ObjectsList::getCollisionObjects(ObjectsList &objectsListB, listIter &objectA, listIter &objectB, bool continueToFind)
{
    listIter objA, objB;
    if (continueToFind) {
        objA = objectA;
        objB = objectB;
    }
    else {
        objB = objectsListB.objectList.begin();
    }
    for (; objB != objectsListB.objectList.end(); objB++) {
        if (this->getCollisionObject(**objB, objA, continueToFind)) {
            objectA = objA;
            objectB = objB;
            return true;
        }
        continueToFind = false;
    }

    return false;
}

listIter ObjectsList::deleteObject(const listIter &object)
{
    return objectList.erase(object);
}

listIter ObjectsList::getBegin()
{
    return objectList.begin();
}

#endif //OBJECTSLIST_H
