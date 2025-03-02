# **Basic C STM32F401 Kernel**

This project is a **basic kernel** developed as part of an **MTE241 lab exercise** at the **University of Waterloo**, led by **Mike**. It demonstrates **concurrency via threading** on the **STM32F401RE** microcontroller. 

The demonstration code in [main.c](./Core/Src/main.c) creates two threads and attempts to create a third that is expected to fail since max threads is set to two. The first thread increments a counter object at a predefined timestep, once it counts (e.g. 20) to a value it calls a function. This thread will yield the CPU everytime it updates the counter. The second thread is responsible for displaying the state of the counter. This thread relies on preemptive scheduling to give up the CPU. The scheduling algorithm used it round robin.

---

## 🚀 **Getting Started**

### **🔹 Prerequisites**

Ensure you have the necessary toolchain installed:

#### **🛠 ARM GNU Toolchain**
- **macOS**: Install via Homebrew:  
  ```sh
  brew install --cask gcc-arm-embedded
  ```  
- **Alternative**: Download and install **Arm GNU Toolchain** directly from the [official Arm website](https://developer.arm.com/downloads/-/gnu-rm).

#### **🛠 OpenOCD (for flashing)**
- Install via Homebrew:  
  ```sh
  brew install openocd
  ```  

---

### **🔹 Running the Kernel**

Once dependencies are installed, you can flash and run the kernel using **OpenOCD**.

#### **▶ Run in Debug Mode**  
```sh
./debug.sh
```
#### **▶ Run in Release Mode**  
```sh
./release.sh
```  
These scripts **mimic** how **STM32CubeIDE** builds the project, except that CubeIDE uses **ST-Link**, whereas these scripts use **OpenOCD** for flashing.

---

## 💡 **Additional Notes**

- This project was designed for **educational purposes** in MTE241.  
- You may need to **modify OpenOCD configurations** based on your setup.  
- If you prefer **STM32CubeIDE**, you can import the project and build it there.  

---

## 📝 **License**
This project is for **educational use only**.  

---

