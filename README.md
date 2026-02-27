# 🧮 Embedded Keypad & LCD Calculator (Bare-Metal C)

A bare-metal embedded calculator implemented in C on a microcontroller platform, using a matrix keypad for user input and an LCD module for real-time output display.

This project demonstrates low-level hardware interfacing, structured state management, and custom expression parsing without reliance on high-level libraries or operating systems.

---

## 📌 Overview

This project implements a fully functional calculator running directly on a microcontroller.

Features include:

- Matrix keypad scanning for user input  
- LCD driver integration for output display  
- Real-time expression parsing and evaluation  
- State-based input handling  
- Register-level peripheral configuration  

The system operates without an operating system, relying on direct hardware control and structured embedded C design.

---

## 🗂️ System Architecture

```
.
├── main.c              # Main control loop
├── keypad.c / .h       # Matrix keypad scanning logic
├── lcd.c / .h          # LCD driver implementation
├── parser.c / .h       # Expression parsing & evaluation
├── utils.c / .h        # Helper functions
└── README.md
```

---

## 🔧 Hardware Components

- Microcontroller TM4C123
- 4x4 Matrix Keypad
- 16x2 LCD 
- GPIO-based interface
- Development board & regulated power supply

---

## ⚙️ Core Features

### 🧾 Keypad Input Handling
- Row-column scanning technique  
- Software debouncing  
- Character mapping  
- Event-based input detection  

### 🖥 LCD Display Control
- 4-bit or 8-bit communication mode  
- Command/data register control  
- Cursor positioning  
- Display refresh handling  

### ➗ Expression Parsing
- Multi-digit number handling  
- Operator detection (+, −, ×, ÷)  
- State machine-based input management  
- Real-time evaluation  

---

## 🔁 Main Control Loop

```c
while (1) {
    scan_keypad();
    process_input();
    update_display();
}
```

The system continuously polls hardware, processes input events, and updates the LCD accordingly.

---

## 🧠 Technical Concepts Demonstrated

- Bare-metal embedded C programming  
- Direct GPIO register manipulation  
- Peripheral configuration  
- Hardware abstraction separation  
- State machine design  
- Memory-efficient implementation  
- Real-time embedded system design  

---


## ▶️ Example Operation

1. User presses keypad buttons  
2. Input appears on LCD  
3. Expression is evaluated when '=' is pressed  
4. Result is displayed  
5. 'C' clears display and resets internal state  

---

## 🚀 Potential Improvements

- Interrupt-driven keypad scanning  
- Floating-point support  
- Expression precedence handling  
- Low-power mode integration  
- UART debug interface  

---

## 👤 Author

Christoff Lotter
MEng Electronic & Electrical Engineering  
Interested in embedded systems, software development, and hardware–software integration.
