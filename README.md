# cpp-inheritance-lab

## Design Choices
The main decision that made everyting else work was writing "seralizeable" as its own interface separate from "person". "Person" is abstract for its own reasons. It can;t be instantained because role had no meaningful default, but "serializable" is a second independent abstractions that happens to be implemented by "person"'s decendants.

This separation is what made "course" possible. "course" isn't a "person". It has no id, no email, and no role so it correctly uses composition rather than inheritance. Because serializable doesnt live inside person, course can still implement it directly with correct behavior.

## Where Exceptions Are Thrown
Every validation failure throws "ValidationError" and they all happen inside constructors. "Person"'s constructor throws if the id is 0 or negative, if name is empty, or if the email fails the format check. "Student" and "Instructor" both call "Person"'s constructor first before running their own checks, so "student" only gets to check if grad_year is under 2000, and "instructor" only gets to check if office is empty, after person's checks already passed. "Course" throws if title is empty.

Because all of this happens inside the constructor before the object is ever returned, a thrown exception means no object was ever created at all, not a half-broken one sitting around. That's why the catch blocks in main.cpp are safe to rely on.


## CSV Escaping
"csv_escape" follows the RFC 4180 rule. If a field has a comma, a quote, or a newline in it, the whole field gets wrapped in quotes so it doesn't break the row. If none of those show up, the field is left alone. When quoting is needed, any quote character already inside the field gets doubled, so a reader can tell it's part of the actual text and not the end of the field.

For example, the input Eve, "The Great" becomes "Eve, ""The Great""" once escaped. Every csv_row function in the project (student, instructor, course, and the polymorphic writer) runs every value through csv_escape before joining with commas, and list fields like courses get joined with ; first, then the whole joined string gets escaped as one piece.