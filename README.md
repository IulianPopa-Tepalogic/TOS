# Tepalogic RTOS

A lightweight, low-overhead real-time operating system (RTOS) designed for embedded applications written in **C and C++**.

Tepalogic RTOS focuses on **simplicity**, **predictability**, and **minimal system overhead**, making it a strong fit for resource-constrained environments and applications that prefer **static allocation**.

---

## Key Goals

- **Minimal runtime overhead**
- **Simple API surface**
- **Deterministic behavior**
- **Designed for static allocation workflows**
- **Easy portability to additional MCU architectures**

---

## Supported Architectures

Currently supported:
- **ARM Cortex-M4**
- **ARM Cortex-M7**

The architecture layer is intentionally kept small to make it easy to extend support to other MCU families.

---

## Features

### Task / Process Management
- Simple scheduling and task management designed for embedded systems
- Lightweight task control with minimal context-switch overhead

### Inter-Process Communication (IPC)
- IPC mechanism based on **signals**
- Designed for fast event delivery and low memory footprint

### Synchronization Primitives
- **Mutexes**
- **Semaphores**

---

## Recommended Use Cases

Tepalogic RTOS is best suited for:
- Embedded projects with **tight memory/CPU budgets**
- Applications that rely on **static allocation**
- Real-time control systems requiring predictable scheduling and minimal jitter
- Firmware requiring **simple concurrency**, without the complexity of larger RTOS frameworks

---

## License

Licensed under the **MIT License**.  
Copyright © 2019 **Tepalogic SRL**.  
See the [LICENSE](LICENSE) file for details.

---

## Commercial Support

Commercial support will be available from **Tepalogic**, including (but not limited to):
- Integration and porting to new MCUs
- Feature development
- Performance tuning and system review
- Long-term maintenance support

For inquiries, please contact Tepalogic via your preferred support channel or open an issue to request a discussion.

---

## Project Status

🚧 Active development.  
APIs and internals may evolve as the RTOS matures.

---

## Contributing

Contributions are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Commit your changes
4. Open a Pull Request

Please keep changes focused and include a short description of the motivation and impact.

---

## Contact

For questions, commercial support, or collaboration requests:

- **Iulian Popa (Tepalogic SRL)**
- Email: ipopa@tepalogic.com

---

## Acknowledgements

Developed and maintained by **Tepalogic SRL**.
