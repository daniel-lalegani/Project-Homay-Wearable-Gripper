# 🦅 Project Homay: Voice-Controlled Wearable Shoulder Gripper

> *Bio-inspired, single-actuator voice-controlled wearable shoulder gripper featuring macaw zygodactyl kinematics and offline TinyML speech recognition.*

---

## 🏛️ Cultural Inspiration & Etymology
In Persian mythology, the **Homay** (Huma bird) is a legendary bird of good fortune and sovereignty, often depicted perching majestically on the shoulders of individuals. This project modernizes this mythological concept into a functional, shoulder-mounted bio-inspired mechatronic assistant.

<p align="center">
  <img src="media/Homay in Iranian mythology.png" alt="Persian Homay Sculpture" width="400"/>
  <br>
</p>

---

## 📷 System Overview & Bio-Inspiration

<p align="center">
  <img src="media/gif on shoulder (2).gif" alt="Project Homay Live Demo" width="550"/>
  <br>
</p>
<p align="center">
  <img src="media/Homay animation on shoulder.gif" alt="Project Homay Live Demo" width="550"/>
  <br>
</p>

- **Wearable Morphology:** Macaw-inspired zygodactyl clamping mechanism structured ergonomically over the shoulder.
  <p align="center">
    <img src="https://github.com/user-attachments/assets/e6facfa1-ffd3-494c-901e-8c34a0fb7f3b" alt="Scarlet Macaw Shoulder Perching" width="350"/>
    <br>
    <sub>Photo credit: <a href="https://commons.wikimedia.org/wiki/File:Scarlet_Macaw_(Ara_macao)_-Houston_Zoo-8.jpg">Kitten Fleming</a>, <a href="https://creativecommons.org/licenses/by/2.0">CC BY 2.0</a>, via Wikimedia Commons</sub>
  </p>

- **Zero-Power Holding & Adaptive Grip:** A single power-screw actuator connected to a 4-tendon **differential force distributor** (floating equalization tree), enabling dynamic force distribution and passive holding with zero electrical power consumption.
  <table border="0">
    <tr>
      <td align="center" width="50%">
        <img src="media/mechanism (2).gif" alt="Real Mechanism Demo" width="100%"/>
        <br>
        <sub><b>Mechanism Execution:</b> Physical linkage response.</sub>
      </td>
      <td align="center" width="50%">
        <img src="media/differential force distributor.gif" alt="Differential Force Distributor" width="100%"/>
        <br>
        <sub><b>Kinematic Kinetics:</b> Floating tendon differential tree.</sub>
      </td>
    </tr>
  </table>

- **Offline TinyML Speech Recognition:** Edge Impulse keyword spotter running locally on an ESP32-S3 SuperMini microcontroller for low-latency, privacy-preserving voice execution ('Grip' / 'Loose').
- **Tissue Safety & Overheat Prevention:** Closed-loop current feedback using an INA219 sensor for immediate stall detection (>210 mA threshold) to protect soft tissues and prevent motor overheating.

---

## 🛠️ Hardware & Electronics Architecture

| Component | Specification / Function |
| :--- | :--- |
| **Microcontroller** | ESP32-S3 SuperMini (TinyML inference & control) |
| **Microphone** | INMP441 Omnidirectional MEMS (I2S digital audio pipeline) |
| **Current Sensor** | INA219 High-Side DC Sensor (Closed-loop safety monitor) |
| **Motor Driver** | DRV8833 Dual H-Bridge |
| **Actuator** | Single N20 DC Gear Motor with Integrated Power-Screw |
| **Structural Frame** | PETG 3D-Printed Linkages & Clavicular Mount |

---

## 📊 Prototype v1.0 Evaluation & Engineering Roadmap

### 🟢 Strengths of Prototype v1.0
- **Adaptive Mechanical Grip:** The differential force distributor successfully conforms to irregular object geometries using a single actuator.
- **Passive Energy Efficiency:** Power-screw locking mechanism completely eliminates static power consumption when holding objects.
- **Robust Local Audio Processing:** The ESP32-S3 offline TinyML model operates reliably without needing Wi-Fi or external cloud infrastructure.

### 🔴 Limitations & Planned Engineering Solutions (v2.0)
1. **Mass & Dimensions Minimization:**
   - *Limitation:* Prototype v1.0 weighs **162 grams** with an acceptable footprint, but further footprint reduction is needed for extended daily wearability.
   - *v2.0 Solution:* Optimize frame topology, downsize battery capacity, and consolidate discrete electronics onto a single custom PCB to significantly minimize volume and mass.
2. **Grip Force Optimization without Added Weight:**
   - *Limitation:* Clamping force is constrained by current tendon leverage ratios.
   - *v2.0 Solution:* Enhance mechanical advantage **without increasing motor mass or upgrading the actuator** by relocating leg pivot axes to increase the moment arm (distance between tendon anchor and pivot fulcrum) and applying high-friction/sticky elastomeric material to claw tips.

---

## 🚀 Potential Future Applications

The lightweight, single-actuator adaptive mechanism of Project Homay provides a versatile platform across multiple domain fields:

1. **Personal Shoulder Assistant & Companion Robot:**
   - Serves as a wearable, hands-free robotic assistant perching on the shoulder to hold tools, assist in daily tasks, and interact with the immediate environment.
2. **Assistive Robotics for Disabled & Visually Impaired Users:**
   - Functions as an intelligent sensor module for blind or visually impaired individuals, providing real-time environmental monitoring, hazard detection, and active accident prevention.
3. **Aerial Robotics & Drone Perching Mechanisms:**
   - **Drone Payload Gripper:** Serves as a ultra-lightweight grasping module for aerial object pickup and delivery.
   - **Uneven Terrain Landing Gear:** Enables UAVs to securely perch on tree branches, pipes, or rough, unstructured terrain using zero-power holding mechanics.
4. **Wearable Videography & Camera Stabilization:**
   - Acts as an adaptive, voice-controlled shoulder mount for automated camera tracking and stabilization during movement.
5. **Search & Rescue / Hazardous Inspection:**
   - Serves as a perching or clamping sensor-carrying end-effector for mobile inspection robots operating in tight or hazardous structures.

---

## 👤 Author

**Daniel Lalegani**  
B.Sc. Student in Mechanical Engineering, Politecnico di Torino  
- **Email:** daniellalegani@gmail.com 
- **LinkedIn:** https://www.linkedin.com/in/daniel-lalegani-9071b2329

## 📜 License

This project is open-source and dual-licensed:
- **Software & Firmware:** Distributed under the [MIT License](LICENSE).
- **Hardware, 3D CAD Models & Schematics:** Distributed under the [Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/) License.

*Copyright (c) 2026 Daniel Lalegani. All rights reserved for original mechanism morphology and design.*
