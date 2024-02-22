# RFID-Attendance-system-design
The project revolves around the development of an RFID-based attendance management system utilizing an MFRC522 chip and TIVA C microcontroller. The system consists of a reader module, tags, and a database, aiming to efficiently mark attendance by reading RFID tags and updating information to a central database.

Key Components and Functionality:
1. Hardware Design: The schematic designs involve integrating the MFRC522 chip onto a PCB with a trace antenna and efficient match filter for noise suppression. A 3D-printed enclosure is used to house the internal circuit.
2. Software Implementation: The TIVA C microcontroller is programmed in embedded C to communicate with the MFRC522 chip via SPI. Libraries for MFRC522 are adapted for use with TIVA C. Various systems like attendance marking, registration, and de-registration are coded into the microcontroller.
3. Test Setup and Results: The system is successfully tested with a setup where RFID cards are scanned, and attendance is marked. Unregistered cards prompt registration, and registered cards mark attendance. Results indicate a 95% accuracy rate in a sample size of 20 cards.
4. Future Work Scope: Future enhancements include improving security through multi-factor authentication, enhancing portability, integrating with access control systems, and expanding functionality to cover areas like inventory management.

Conclusion:
The project achieves its objectives of developing an RFID-based attendance management system. It successfully marks attendance, registers new users, and allows for de-registration. While the system performs well in tests, there's room for improvement in security and functionality, which could be addressed in future iterations. Overall, the system is deemed ready for practical implementation in educational settings.
