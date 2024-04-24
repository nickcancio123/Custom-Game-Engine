===== ABOUT =====

This project is a WORK-IN-PROGRESS graduate project at the Florida Interactive Entertainment Academy (FIEA). 

It forms the interactive-data portion of a game engine, enabling a game developer to configure the behavior of and data contained by game objects in both C++ and JSON. 

NOTE: This project is being developed using Perforce, so this repo only contains snapshots of source from the original project.

.

===== TECHNICAL IMPLEMENTATION =====

The core data model of this engine is a recursive pair of classes: Scope and Datum. 

DATUM is a homogeneous data wrapper that resolves its type at runtime. Any single datum can wrap one of many supported data types, like integer, float, string, vector4, or a Scope (explained next). Datums can be resized, much like std::vector. So, datum can represent either a single data value or a homogeneous array. Lastly, datum can either own the data it wraps, or wrap a reference to external data.

SCOPE is a class that HAS datums, which it maps names to. For example, it can map "Health" to a Datum containing a float. Quickly, you can imagine that a scope can be configured to represent another class by having a set of named variables. 

ATTRIBUTED does just that. Attributed is an abstract subclass of scope that enables subclasses of itself to create named datums that wrap their C++ member variables. For example, a class Monster, could map the name "Health" to a datum that wraps Monster's "_health" C++ member variable. With this system, changes to the C++ "_health" variable would be reflected in the "Health" datum, and vice versa, because the two are bound. 

PARSECOORDINATOR, with the help of a third-party JSON parsing library, can read through JSON files that define objects with variables with initial values, and convert them to scope instances or attributed subclass instances.

GAMEOBJECT, is an attributed subclass that has transform information (i.e., position, rotation, and scale), can have children gameobjects, and has a virtual method Update, which it recursively calls on its children. Gameobjects also have a list of actions.

ACTIONS, inheriting from attributed, are modular units of behavior that can be added to or removed from gameobjects to configure their behavior. Actions create behavior by operating on the datums on their owner gameobjects. Gameobjects call Update on their own list of actions, thereby "behaving". 
