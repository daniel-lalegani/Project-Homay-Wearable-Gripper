# Project Homay — Bio-Inspired Wearable Robotic Gripper

*A lightweight, shoulder-mounted gripper built around a single actuator and a custom four-tendon mechanism.*

Homay is a robotic gripper I designed and built to explore how a compact mechanism can adapt to different object shapes while sitting on a person's shoulder. The current version is a working prototype, controlled by two offline voice commands. It is not a medical device or an autonomous flying robot.

## Demo

<p align="center">
  <img src="media/gif on shoulder (2).gif" alt="Homay prototype mounted on a shoulder" width="550" />
</p>

<p align="center">
  <img src="media/mechanism (2).gif" alt="Physical gripping mechanism in motion" width="450" />
</p>

## How it works

### Bird-inspired design

I took inspiration from the gripping arrangement of a macaw's feet when developing the four gripping arms. The structure is 3D-printed in PETG and designed to mount over the shoulder.

<p align="center">
  <img src="https://github.com/user-attachments/assets/e6facfa1-ffd3-494c-901e-8c34a0fb7f3b" alt="Macaw gripping a branch" width="340" />
</p>

<sub>Photo: [Kitten Fleming](https://commons.wikimedia.org/wiki/File:Scarlet_Macaw_(Ara_macao)_-Houston_Zoo-8.jpg), [CC BY 2.0](https://creativecommons.org/licenses/by/2.0/).</sub>

### Single-actuator, four-tendon mechanism

One N20 geared motor turns a power screw that drives the gripper. Rather than using a separate motor for each gripping arm, I designed a movable tendon-distribution assembly.

Four tendons connect near the four corners of the **yellow crossbar** in the CAD animation. They run downward through the mechanism to the gripping arms below. The crossbar is supported by a joint-like assembly that allows it to rotate about **two axes**. When an arm meets resistance from an object, the crossbar can tilt so the remaining arms can adjust to its shape.

I developed this configuration independently while working through the problem of driving four tendons with one actuator. I am not claiming that the arrangement is unique in the robotics literature, or that it produces equal tension in all four tendons; those properties have not been established by measurement.

<p align="center">
  <img src="media/differential force distributor.gif" alt="CAD animation: yellow tendon crossbar on two-axis articulated support" width="500" />
</p>

*Yellow: four tendon attachment points. The tendons route downward to the gripping arms. The support permits tilting about two rotational axes.*

The power screw can also hold the gripper's position without continuous **motor drive**. The electronics may still consume power.

### Voice control and motor shut-off

An **ESP32-S3** runs an offline keyword-recognition model developed with **Edge Impulse**. An INMP441 microphone captures the commands **“grip”** and **“loose”**, and a DRV8833 driver operates the motor.

An INA219 current sensor is used to stop the motor when the measured current crosses a configured threshold (approximately **210 mA** in v1.0). This is an experimental overload response, **not** a validated tissue-force or medical safety system.

## Hardware

| Component | Purpose |
|---|---|
| ESP32-S3 SuperMini | Microcontroller for local speech inference and motor control |
| INMP441 I2S microphone | Audio input |
| INA219 current sensor | Current monitoring for the motor shut-off rule |
| DRV8833 | DC motor driver |
| N20 geared DC motor + power screw | Single mechanical actuator and passive position holding |
| PETG 3D-printed parts | Gripping arms, articulated assembly, and shoulder-mounted structure |

**Software:** Edge Impulse for keyword-model development; firmware running locally on the ESP32-S3. Edge Impulse is a software development platform, not a physical hardware component.

**Prototype mass:** 162 g (v1.0, as measured/reported during the build).

## What works, and what needs improvement

The prototype demonstrates motion of all four gripping arms with a single motor, mechanical adaptation around different object shapes, local voice control, and position holding without continuously powering the motor.

The main limitation is **grip strength**. I plan to enhance mechanical advantage without increasing motor mass or upgrading the actuator by relocating leg pivot axes to increase the moment arm (distance between tendon anchor and pivot fulcrum) and applying high-friction/sticky elastomeric material to claw tips. The assembly could also be smaller and lighter with optimizing frame topology, downsizing battery capacity, and consolidating discrete electronics onto a single custom PCB.

These observations are qualitative demonstrations. Controlled measurements of grip performance and speech-command recognition have **not yet been reported**. The shoulder mounting, contact forces and long-term comfort have not been clinically or formally safety-tested.

## Next steps

- Improve the gripping geometry and fingertip contact materials without unnecessarily increasing motor mass.
- Reduce the assembly's footprint and consolidate the electronics.
- Measure voice-command accuracy and response latency under defined conditions.
- Explore tactile sensing and feedback to the wearer as a possible future extension; this has **not** been implemented.

A separate [vision document](VISION_HOMAY.md) explores a bird-like embodied robotic companion as a **long-term concept**, not as a capability of this prototype.

## Why “Homay”?

In Persian mythology, the **Homay (Huma)** is a legendary bird associated with good fortune and sometimes portrayed near a person's shoulder. I chose the name because it connects the project's bird-inspired form with the idea of a small shoulder-mounted assistant.

<p align="center">
  <img src="media/Homay in Iranian mythology.png" alt="Homay in Iranian mythology" width="340" />
</p>

## Author and licensing

**Daniel Lalegani** — Mechanical Engineering undergraduate, Politecnico di Torino  
[GitHub](https://github.com/daniel-lalegani) · [LinkedIn](https://www.linkedin.com/in/daniel-lalegani-9071b2329)

Firmware/software: [MIT License](LICENSE), where covered by the repository licence file. Hardware/CAD documentation: intended to be shared under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/); individual released files should identify their applicable terms. Third-party images retain their original licences.
