# Modular Packet Processor (MPP) - Design Document

## **1. Overview**

### **1.1 Purpose**

MPP is a **minimalist, extensible, UNIX-based framework** designed for users who know exactly what they want to do. It provides **no safety rails** and expects users to operate with full control. Unlike modern frameworks that enforce rigid workflows, MPP is about **power and flexibility**, where users define their own commands and dynamically load them at runtime.

### **1.2 Key Features & Ethos**

- **Minimal Built-in Commands**: Only `quit`, `q`, `ESC`, `help`, and `h` exist.
- **User-Defined Commands**: The system does not impose any restrictions on what users can run—commands are compiled into `.so` modules and loaded at runtime.
- **No Hand-Holding**: MPP assumes users **know what they want and how to execute it**.
- **Performance First**: Designed with efficiency and raw computing power in mind.
- **Interoperability**: Users are free to implement their own plugins, IPC mechanisms, and logging strategies.
- **Command Pipeline Model**: Supports UNIX-style command chaining (`|`).

---

## **2. Requirements & Functional Specifications**

### **2.1 Functional Requirements**

- MPP must dynamically load and execute commands defined by the user.
- Commands must be implemented as compiled shared objects (`.so`).
- The system must support a runtime plugin-based execution model.
- IPC must be extensible, but not enforced.
- A lightweight, high-performance logging system should be available but optional.
- Commands must support **pipelined execution** to chain multiple commands together.

### **2.2 Non-Functional Requirements**

- **No enforced restrictions on user code**.
- **Minimal built-in functionality**, everything else is user-defined.
- **Low-latency execution**.
- **Pluggable architecture** for seamless customization.
- **Self-describing commands**: Commands should provide metadata about their capabilities and I/O expectations.

---

## **3. MPP-Core: Command Processing Model**

### **3.1 Philosophy**

MPP-Core is not a standard command processor; it acts as a **runtime loader and dispatcher** for user-defined commands. MPP does not dictate what commands should exist—it simply provides a minimal shell to execute user-provided logic.

### **3.2 Built-in Commands**

| Command              | Description                       |
| -------------------- | --------------------------------- |
| `quit` / `q` / `ESC` | Exit MPP.                         |
| `help` / `h`         | Display user-registered commands. |

All other commands **must be implemented by the user and compiled into `.so` libraries**.

### **3.3 Command Execution Model**

#### **3.3.1 User-Defined Commands**

User-defined commands form the core of MPP. These commands:
- Are compiled as shared object (`.so`) files.
- Follow a standardized interface.
- Operate independently but can be pipelined together.

#### **3.3.2 Command Interface (`ICommand`)

Each command must:
- Accept **input and output streams** (`STDIN` / `STDOUT`).
- Provide a **metadata function** (`getCapabilities()`).
- Execute with **parameters and environmental variables**.

```cpp
class ICommand {
public:
    virtual void execute(std::istream& input, std::ostream& output) = 0;
    virtual std::string getCapabilities() const = 0;
    virtual ~ICommand() {}
};
```

#### **3.3.3 Command Invocation and Execution**

Commands are loaded dynamically and executed using an invoker, implementing the **Strategy Pattern**.

```cpp
class CommandInvoker {
private:
    std::unique_ptr<ICommand> command;
public:
    CommandInvoker(std::unique_ptr<ICommand> cmd) : command(std::move(cmd)) {}
    void execute(std::istream& input, std::ostream& output) {
        command->execute(input, output);
    }
};
```

#### **3.3.4 Command Metadata & Capabilities (`getCapabilities()`)**

To make user-defined commands self-describing, each command must return **structured metadata** about its capabilities.

- **What input it accepts** (e.g., raw packets, text, structured data)
- **What output it produces** (e.g., logs, modified packets, JSON)
- **Any required arguments** (e.g., filters, capture options)
- **Execution constraints** (e.g., needs root access, exclusive execution)

Example JSON-style metadata:
```json
{
  "name": "filter.so",
  "description": "Filters packets based on protocol",
  "input_type": "network_packets",
  "output_type": "filtered_packets",
  "args": ["protocol"],
  "execution_mode": "pipeline_compatible"
}
```

This allows MPP to **dynamically query commands** for their capabilities, making command discovery and integration seamless.

#### **3.3.5 Example Pipeline Execution**

Commands work in a pipelined execution model similar to UNIX pipes (`|`). Example:

```sh
mpp start.so eth0 | filter.so "tcp" | log.so > capture.log
```

- `start.so` captures packets.
- `filter.so` processes only TCP packets.
- `log.so` writes the results to a log file.

This model ensures **functional, stateless execution** and composability.

## **3.3.6 Expanding `getCapabilities()` for Enhanced Command Discovery**

As MPP evolves, the `getCapabilities()` function will play a crucial role in making commands **self-describing and dynamically discoverable**. To expand its usefulness, the function should:

- **Support structured metadata queries** for runtime inspection.
- **Allow MPP to list and validate available commands (`mpp --list-commands`)**.
- **Provide compatibility checking for pipelining and execution.**

### **Standardized JSON Metadata Schema**

To ensure consistency, commands should return metadata in the following structure:

```json
{
  "name": "filter.so",
  "description": "Filters packets based on protocol",
  "version": "1.0.0",
  "input_type": ["network_packets"],
  "output_type": ["filtered_packets"],
  "args": [
    {
      "name": "protocol",
      "type": "string",
      "required": true,
      "description": "Protocol to filter (e.g., tcp, udp, icmp)"
    }
  ],
  "execution_mode": "pipeline_compatible",
  "permissions": ["root_required"]
}
```

### **Enhancing MPP with Query Capabilities**

To leverage this metadata, MPP will include:

- **Command Listing (`mpp --list-commands`)**
  - Queries available `.so` commands and retrieves their `getCapabilities()` metadata.

- **Capability Validation (`mpp --check-capabilities filter.so`)**
  - Ensures a command can be pipelined with another before execution.

- **Automatic Pipeline Validation**
  - Uses `input_type` and `output_type` fields to ensure commands are chainable.

### **C++ Implementation of `getCapabilities()`**

The function will return a **JSON string** using a lightweight C++ JSON library such as `nlohmann/json`:

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class FilterCommand : public ICommand {
public:
    std::string getCapabilities() const override {
        json capabilities = {
            {"name", "filter.so"},
            {"description", "Filters packets based on protocol"},
            {"version", "1.0.0"},
            {"input_type", {"network_packets"}},
            {"output_type", {"filtered_packets"}},
            {"args", json::array({
                { {"name", "protocol"}, {"type", "string"}, {"required", true}, {"description", "Protocol to filter (e.g., tcp, udp, icmp)"} }
            })},
            {"execution_mode", "pipeline_compatible"},
            {"permissions", {"root_required"}}
        };
        return capabilities.dump();
    }
};
```

This allows MPP to dynamically query command capabilities and enforce execution constraints.

---

## **7. Change Log & Future Enhancements**

| Date       | Change Summary                                     | Contributor         |
| ---------- | ------------------------------------------------- | ------------------- |
| 2025-02-10 | Rewrote MDD structure with MPP philosophy         | Partner's Assistant |
| 2025-02-10 | Added pipeline execution model                    | Partner's Assistant |
| 2025-02-10 | Defined command metadata & capabilities           | Partner's Assistant |
| 2025-02-11 | Added command execution model and interface       | Partner's Assistant |
| 2025-02-11 | Added `getCapabilities()` metadata section        | Partner's Assistant |
| 2025-02-11 | Expanded `getCapabilities()` with JSON schema & validation | Partner's Assistant |

