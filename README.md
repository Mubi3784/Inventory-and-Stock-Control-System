 #  Inventory and Stock Control System

A C++ console-based application that manages products, suppliers, and stock records using data structures and algorithms from the Data Structures & Algorithms course.

---

##  Project Description

This Inventory and Stock Control System is designed to:
- Maintain a database of **products**, **suppliers**, and **stock quantities**
- Perform **insert, search, delete**, and **sort** operations
- Handle data through **files** and recover it on next use
- Implement real-world applications of **data structures and algorithms**

---

##  Features

- Add, view, search, and delete **products**
- Add and view **suppliers**
- Add and view **stock** (linked to products and suppliers)
- **Sort** products (by ID) and stock (by quantity)
- **Save** and **load** data using files
- **Exception handling** for errors (like duplicates and missing entries)
- **Menu-driven** console interface

---

##  Data Structures & Algorithms Used

| Category              | Techniques Implemented                              |
|-----------------------|-----------------------------------------------------|
| **Data Structures**   | Binary Search Tree (BST), Singly Linked List, Arrays |
| **Sorting Algorithms**| Bubble Sort (for Products/Suppliers), Merge Sort (for Stock) |
| **Searching Algorithms** | Linear Search, Binary Search                    |
| **Exception Handling**| Custom Exceptions using `runtime_error`             |
| **File Handling**     | `ifstream`, `ofstream` to load/save `.txt` files     |

---

##  Classes Overview

- `Product` – stores productID, name, price, category
- `Supplier` – stores supplierID, name, contact info
- `Stock` – stores productID, supplierID, quantity
- `ProductBST` – Binary Search Tree for products
- `SupplierList` – Singly linked list for suppliers
- `StockList` – Singly linked list for stock
- Custom Exception Classes:
  - `FileException`
  - `DuplicateIDException`
  - `NotFoundException`

---


---

##   Contributors

-  Student Mubashir Nazir — [LinkedIn] www.linkedin.com/in/mubashir-nazir-987220293
-  Student Muhammad Tahir — [LinkedIn] https://www.linkedin.com/in/muzammil-khaliq-181773327/
-  Student Muhammad Saad — [LinkedIn]  https://www.linkedin.com/in/mohammad-saad-shafique-532247332/
-  Student Muzammil Khaliq — [LinkedIn]  https://www.linkedin.com/in/muhammad-tahir-881b1132a/

 

---

## Course Details

- **Subject**: Data Structures & Algorithms (DSA)
- **Department**: Software Engineering
- **University**: International Islamic University, Islamabad (IIUI)
- **Semester**: Spring 2025

---

##  Future Improvements

- Add GUI interface
- Implement AVL or Red-Black Tree for better balancing
- Add database support instead of text files
- Add user login and access control

---

##  License

This project is part of the academic coursework for educational purposes.



