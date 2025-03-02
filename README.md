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

### 🔹 Running the Kernel

Once the dependencies are installed, you can **flash** and **run** the kernel using **OpenOCD**.

#### 1. Flash the Kernel
To flash the kernel to your STM32 device, run the following command:

```sh
make flash
```

#### 2. View the Output
To view the kernel's output, open a serial port with the following configurations:

- **Baud rate**: 115200
- **Data bits**: 8
- **Stop bits**: 1
- **Parity**: None

---

## 💡 **Additional Notes**

- This project was designed for **educational purposes** in MTE241.  
- You may need to **modify OpenOCD configurations** based on your setup.  
- If you prefer **STM32CubeIDE**, you can import the project and build it there. I kept all the original [Eclipse IDE config files](./.stm32cube-config-files) incase they are needed.


