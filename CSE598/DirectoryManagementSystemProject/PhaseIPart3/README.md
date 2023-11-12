# Directory Management System Phase I Part 3

Intellij project using Java 17. The following changes were done for Phase 3.

### Content Coupling:
Issue: Direct manipulation of totalObjects from PersonnelDirectory.
Fix: Use the Personnel for object creation and tracking.

### Common Coupling:

Issue: Shared references to Person and Employee objects between PersonnelDirectory and Personnel.
Fix: Centralize object creation and management with Personnel.

Issue: Static numObjects variable in totalObjects can be manipulated by multiple instances, causing data inconsistency.
Fix: Refactor totalObjects to use instance variables or manage object count within the PersonnelFactory.

### Stamp Coupling:
Issue: Passing entire Person objects as method parameters.
Fix: Use specific parameters or DTOs to pass only the required data.

### Control Coupling:
Issue: PersonnelDirectory knows how to print Personnel objects by manipulating a public instance of Personnel.
Fix: Refactor Personnel to more clear control methods on how to print each object and use those methods from PersonnelDirectory.

### Object-Oriented Concept Violations:
Issue: Directly manipulating the inner list of Personnel from PersonnelDirectory, and also directly manipulating FirstName, Middle Name and LastName from Person.
Fix: Use encapsulation. Provide methods in Personnel to interact with the list without directly exposing it.