<h1>Bluetooth RFID Bank Account with Card & POS</h1>

<h2>Project Description</h2>
Cont bancar personal cu POS si card, controlate de un modul Arduino Nano. Placuta Arduino comunica cu o aplicatie mobila prin intermediul unui modul Bluetooth HC-05. Aplicatia ofera posibilitatea de a urmari soldul curent sau de a efectua un top-up. Cardul poate fi apropiat de POS pentru a fi validat si a se efectua platile. 

<br><br>
<center><img src="https://raw.githubusercontent.com/PacoPakkun/Android-Things-RFID/main/demo.jpeg" width="500px"></center>
<br>
<center><img src="https://raw.githubusercontent.com/PacoPakkun/Android-Things-RFID/main/circuit.png" width="500px"></center>

<h2>Hardware Requirements</h2>
<ul>
    <li>Arduino Nano - https://store.arduino.cc/products/arduino-nano</li>
    <li>HC-05 Bluetooth module - https://cleste.ro/modul-bluetooth-hc-05.html</li>
    <li>Breadboard - https://cleste.ro/breadboard-400-puncte.html</li>
    <li>Jumper wires - https://cleste.ro/65-fire-jumper.html</li>
    <li>Modul RFID cu card si tag - https://cleste.ro/modul-rfid-cu-card-si-tag.html</li>
</ul>

<h2>Software Requirements</h2>
<ul>
    <li>Script Arduino - https://github.com/PacoPakkun/Android-Things-RFID/blob/main/sketch.ino</li>
    <li>Aplicatie Android - https://github.com/PacoPakkun/Android-Things-RFID/tree/main/RFID</li>
</ul>

<h2>Setup and Build</h2>
Pentru a rula aplicatia, se vor reproduce urmatorii pasi:
<ul>
    <li>Se va incarca scriptul Arduino pe placuta Nano, folosind un cablu USB-miniUSB</li>
    <li>Se va instala aplicatia mobila pe un telefon Android compatibil</li>
    <li>Placuta Arduino trebuie sa fie conectata la o sursa de 5V</li>
    <li>Din aplicatie, utilizatorul va apasa butonul "Reconnect" pentru a se conecta la POS. Daca Bluetooth-ul este inactiv, acesta va trebui pornit.</li>
    <li>Utilizatorul poate urmari soldul din aplicatie si poate apasa butonul "Top-Up" pentru a adauga valuta.</li>
</ul>

