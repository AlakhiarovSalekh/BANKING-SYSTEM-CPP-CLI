# Banking System — C++ CLI

[![C++](https://img.shields.io/badge/C%2B%2B-CLI-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![OpenSSL](https://img.shields.io/badge/OpenSSL-SHA--256-721412?logo=openssl&logoColor=white)](https://www.openssl.org/)
[![License](https://img.shields.io/github/license/AlakhiarovSalekh/BANKING-SYSTEM-CPP-CLI)](LICENSE)
[![Stars](https://img.shields.io/github/stars/AlakhiarovSalekh/BANKING-SYSTEM-CPP-CLI?style=social)](https://github.com/AlakhiarovSalekh/BANKING-SYSTEM-CPP-CLI/stargazers)

A terminal-based banking application written in C++ with CSV persistence, SHA-256 PIN hashing through OpenSSL, account creation, deposits, withdrawals, and masked PIN entry.

## Features

- Create banking accounts
- Generate account numbers
- Persist account data in CSV format
- Hash PINs with SHA-256
- Deposit and withdraw funds with validation
- Update stored data through a temporary-file flow
- Mask PIN entry in the terminal
- Format monetary values for display

## Concepts Demonstrated

- Classes and encapsulation
- Inheritance and virtual functions
- File I/O
- CSV persistence
- Smart pointers
- Input validation
- OpenSSL hashing
- Console input handling

## Requirements

- A C++ compiler
- OpenSSL development libraries
- The current masked-input implementation uses Windows `_getch()`; portability work may be needed on Linux/macOS

## Build & Run

```bash
git clone https://github.com/AlakhiarovSalekh/BANKING-SYSTEM-CPP-CLI.git
cd BANKING-SYSTEM-CPP-CLI
g++ BankingManagementSystem.cpp -o banking -lssl -lcrypto
./banking
```

On Windows, run the produced executable in your terminal.

> This is an educational/demo banking system, not production financial software.

## Contributing

Portability fixes, tests, input-validation improvements, documentation, and focused security improvements are welcome.

## Author

**Salekh Alakhiarov** · [GitHub](https://github.com/AlakhiarovSalekh)

## License

MIT — see [LICENSE](LICENSE).
