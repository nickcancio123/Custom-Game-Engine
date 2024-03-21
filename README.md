
This project is still a WORK-IN-PROGRESS.

Created as a semester-long programming GRADUATE PROJECT at the Florida Interactive Entertainment Academy (FIEA).

The objective of this project was to create a system for C++ CODE REFLECTION into another language, JSON, so that a game built with this engine could use JSON as a scripting language.


The core data model is constructed by a recursive pair of classes: Scope and Datum. 

DATUM is much like std::vector, but rather than evaluating the data type it contains at compile-time, Datum does so at run-time. One of the data types Datum can wrap is a pointer to a Scope.
SCOPE is a class that maps names to Datums. For example, it can map "Health" to a Datum containing a float. 
A Scope could have a Datum that points another Scope. For example, a Mage (deriving from Scope), could have a Datum that points to a Staff (deriving from Scope), and have it named "Staff".

In this manner, Scope and Datum form a heirarchical data model.

