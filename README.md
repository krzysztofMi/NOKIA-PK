# SQL Lite instalacja(Linux Debian, Ubuntu, Mint etc.)

    sudo apt install sqlite3
    sudo apt install libsqlite3-dev

powinno śmigać jak zrobicie make w UE, ale jest szansa, że będziecie musieli zrobić sudo make.

# Wymagania do kompilacji projektu

    sudo apt install qt5-default cmake gcc

# Kompilacja projektu (cmake, gcc, qt5)

    cd NOKIA-PK
    cmake CMakeLists.txt

i teraz możemy poszczególne komponenty w projekcie skompilować np.

    make /UE
    make /BTS
    ...
