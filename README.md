# 🦅 Project-Homay-Wearable-Gripper
Bio-inspired, single-actuator voice-controlled wearable shoulder gripper featuring macaw zygodactyl kinematics and offline TinyML speech recognition
<p align="center">
  <img src="media/gif on shoulder.gif" alt="Project Homay Live Demo" width="550"/>
</p>

## 📷 System Overview & Demo
- **Wearable Morphology:** Macaw-inspired zygodactyl clamping mechanism structured over the clavicle.
<img width="960" height="695" alt="960px-Scarlet_Macaw_(Ara_macao)_-Houston_Zoo-8" src="https://github.com/user-attachments/assets/e6facfa1-ffd3-494c-901e-8c34a0fb7f3b" />

- **Zero-Power Holding:** Single power-screw actuator connected to a 4-tendon dynamic tension distribution tree.
<p align="center">
  <img src="media/differential force distributor.gif" alt="mechanism" width="550"/>
</p>
- **Offline TinyML:** Edge Impulse keyword spotter running locally on an ESP32-S3-supermini microcontroller.
- **Tissue Safety:** INA219 current sensor closed-loop feedback for stall detection (>210 mA threshold).

---

## 🛠️ Hardware & Electronics
- **Microcontroller:** ESP32-S3 SuperMini
- **Microphone:** INMP441 Omnidirectional MEMS (I2S interface)
- **Current Sensor:** INA219
- **Motor Driver:** DRV8833 H-Bridge
- **Structure:** PETG 3D-Printed linkages & mounting frame

---

## 👤 Author
**Daniel Lalegani**  
B.Sc. Student in Mechanical Engineering, Politecnico di Torino
- **Email:** daniellalegani@gmail.com






<a href="https://commons.wikimedia.org/wiki/File:Scarlet_Macaw_(Ara_macao)_-Houston_Zoo-8.jpg">Kitten Fleming</a>, <a href="https://creativecommons.org/licenses/by/2.0">CC BY 2.0</a>, via Wikimedia Commons




