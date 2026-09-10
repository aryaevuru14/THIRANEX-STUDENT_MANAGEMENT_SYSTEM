# Student Management System (C++)

A robust, enterprise-grade console application built in modern C++ to manage student records efficiently. This project demonstrates core Object-Oriented Programming (OOP) principles, rigorous input validation, and persistent data storage using file handling.

## 🚀 Key Features

* **Complete CRUD Operations:** Create, Read, Update, and Delete student records seamlessly.
* **Persistent Storage:** Automatically serializes and deserializes data to a `students_db.csv` file, ensuring no data loss between sessions.
* **Crash-Proof Input Validation:** Implements aggressive input buffer clearing (`std::cin.ignore`, `std::cin.clear`) to prevent infinite loops or crashes when users input invalid data types (e.g., entering strings instead of integers).
* **Memory Safe:** Utilizes modern C++ features like `std::vector`, RAII (Resource Acquisition Is Initialization), and standard library strings to prevent memory leaks and buffer overflows.
* **Separation of Concerns:** Strictly separates the `Student` data model from the `StudentManager` business logic.

## 🛠️ Technology Stack

* **Language:** C++ (Standard C++11 or higher)
* **Environment:** Windows (via MinGW/MSYS2 toolchain) / Linux / macOS
* **IDE/Editor:** Visual Studio Code (or any preferred text editor)
* **Compiler:** `g++` (GCC)

## 📁 Project Structure

```text
├── student.cpp        # Main application source code containing models and logic
├── students_db.csv    # Automatically generated database file (persists data)
└── README.md          # Project documentation

```

## ⚙️ Installation & Setup

**1. Clone the repository:**

```bash
git clone https://github.com/YourUsername/your-repo-name.git
cd your-repo-name

```

**2. Compile the application:**
Using the `g++` compiler, compile the source code into an executable:

```bash
g++ student.cpp -o student

```

**3. Run the application:**
Execute the compiled binary:

* **Windows:**
```powershell
.\student.exe

```


* **Linux/macOS:**
```bash
./student

```



## 💻 Usage Guide

Upon running the application, you will be greeted by the System Administration Dashboard:

```text
=====================================
  SYSTEM ADMINISTRATION DASHBOARD    
=====================================
1. Add New Student Record
2. View All Student Records
3. Update Existing Record
4. Delete Student Record
5. Terminate Session
=====================================
Select Operation (1-5):

```

1. **Add New Student:** Enter a unique ID, full name, age, and GPA (0.0 - 4.0).
2. **View All Students:** Displays a neatly formatted table of all currently enrolled students.
3. **Update Existing Record:** Prompts for a Student ID and allows modification of Name, Age, and GPA.
4. **Delete Student Record:** Removes a student from both active memory and the CSV database.
5. **Terminate Session:** Safely flushes all pending operations to the disk and exits the program safely.

## 🛡️ License

This project is licensed under the MIT License - see the LICENSE file for details.
