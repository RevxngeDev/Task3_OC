# README: Controller and Generator Programs

This README provides instructions for compiling, running, and testing the `controller` and `generator` programs, as well as how to interact with them using signals.

## **Compilation**

To compile the programs:
```bash
gcc generator.c -o generator
gcc controller.c -o controller
```

## **Execution**

### **Step 1: Grant Execution Permissions**
Before running the programs, ensure they have execution permissions:
```bash
chmod +x controller generator
```

### **Step 2: Run `controller`**
Start the `controller` program:
```bash
./controller
```
This will:
1. Start the `generator` program with a random argument between 120 and 180.
2. Send generated expressions to `bc` for evaluation.
3. Output results in the format:
   ```
   5 + 3 = 8
   7 * 2 = 14
   ```
## **Stopping the Program**

### **Using `Ctrl + C`**
To stop the `controller` program, press `Ctrl + C` in the terminal where it is running.

### **Step 1: Find the PID**
Find the process ID of the `controller` program:
```bash
ps aux | grep ./controller
```

### **Step 2: Send the Signal**
Send the `SIGUSR1` signal to the `controller` process:
```bash
kill -SIGUSR1 <PID>
```
Replace `<PID>` with the actual process ID of `controller`.
